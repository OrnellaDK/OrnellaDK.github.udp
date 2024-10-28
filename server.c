#include "network.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PORT1 8081
#define PORT2 8082
#define BUFFER_SIZE 1024

int main(void) {

  printf("************Server UDP***************\n");
  /*initialisation*/
  SOCKET Socket1, Socket2;
  SOCKADDR_IN serverAddr, clientAddr1, clientAddr2;
  char buffer[BUFFER_SIZE];
  int clientAddrSize1 = sizeof(clientAddr1);
  int clientAddrSize2 = sizeof(clientAddr2);
  int receivedBytes;

  initNetwork();

  Socket1 = socket(AF_INET, SOCK_DGRAM, 0);
  Socket2 = socket(AF_INET, SOCK_DGRAM, 0);
  if ((Socket1 == INVALID_SOCKET) || (Socket2 == INVALID_SOCKET)) {
    perror("socket() failed");
    cleanNetwork();
    return EXIT_FAILURE;
  }
  else
  {
    printf("SOCKETS are created \n");
  }

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); /*permet d'écouter sur toutes les interfaces*/

  serverAddr.sin_port = htons(PORT1);
  /*Attribu un port libre au socket 1*/
  if (bind(Socket1, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
    perror("bind() failed");
    closesocket(Socket1);
    cleanNetwork();
    return EXIT_FAILURE;
  }

  serverAddr.sin_port = htons(PORT2);
  /*Attribu un port libre au socket2*/
  if (bind(Socket2, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
    perror("bind() failed");
    closesocket(Socket2);
    cleanNetwork();
    return EXIT_FAILURE;
  }

  printf("UDP server is running on port %d and %d\n", PORT1, PORT2);

  int maxfd, activity, valread, connect =0, cond = 0;
  fd_set rd;

  char ack1[] = "ack1\n";
  char ack2[] = "ack2\n";

  /*boucle infini qui permet d'attendre un message */
  while (1) {

    FD_ZERO(&rd);
    FD_SET(Socket1, &rd);
    FD_SET(Socket2, &rd);
    maxfd = (Socket1 > Socket2) ? Socket1 : Socket2;
    activity = select(maxfd + 1, &rd, NULL, NULL, NULL);
    if (activity < 0)
      perror("select error");
    

    if (FD_ISSET(Socket1, &rd))
    {
      valread = recvfrom(Socket1, buffer, BUFFER_SIZE - 1, 0, (SOCKADDR*)&clientAddr1, &clientAddrSize1);
      buffer[valread] = '\0';
      printf("%s DU CLIENT 1 RECU\n", &buffer);
      if (strcmp(buffer, "INIT") == 0)
      {
        connect++;
      
        if (connect == 2) /*client 2 s'est connecté*/
        {
          sendto(Socket1, ack2, sizeof(ack2), 0, (SOCKADDR*)&clientAddr1, clientAddrSize1);
          sendto(Socket2, ack1, sizeof(ack1), 0, (SOCKADDR*)&clientAddr2, clientAddrSize2);
        }
        else
          printf(" en attente de la connexion de l'autre client\n");
      }
    }

    if (FD_ISSET(Socket2, &rd))
    {
      valread = recvfrom(Socket2, buffer, BUFFER_SIZE - 1, 0, (SOCKADDR*)&clientAddr2, &clientAddrSize2);
      buffer[valread] = '\0';
      printf("%s DU CLIENT 2 RECU\n", &buffer);
      if (strcmp(buffer, "INIT") == 0)
      {
        connect++;
        if (connect == 2) /*client 2 pas encore connecté*/
        {

          sendto(Socket2, ack2, sizeof(ack2), 0, (SOCKADDR*)&clientAddr2, clientAddrSize2);
          sendto(Socket1, ack1, sizeof(ack1), 0, (SOCKADDR*)&clientAddr1, clientAddrSize1);
        }
        else
          printf(" en attente de la connexion de l'autre client\n");
      }
    }
  }

  /* fermeture du socket*/
  closesocket(Socket1);
  closesocket(Socket1);
  cleanNetwork();

  return EXIT_SUCCESS;
}
