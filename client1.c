#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SERVER_PORT 8081
#define SERVER_ADDR "127.0.0.1"
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
  printf("**********************Client1 UDP****************************\n");
  /*initialisation du socket et des buffer*/
  SOCKET udpSocket;
  SOCKADDR_IN serverAddr;
  char message[BUFFER_SIZE];
  char receiveBuffer[BUFFER_SIZE];
  int serverAddrSize = sizeof(serverAddr);
  unsigned short int port = DEFAULT_SERVER_PORT;


  /*verification du bon nombre d'arguments question c)*/
  if (argc > 1) {
    port = (unsigned short int)atoi(argv[1]);
  }

  initNetwork();

  udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (udpSocket == INVALID_SOCKET) {
    perror("socket() failed");
    return EXIT_FAILURE;
  }
  else
  {
    printf("SOCKET create\n");
  }

  //memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

  /*envoi du message client*/
  printf("entrer le message de connection INIT: ");
  fgets(message, sizeof(message), stdin);
  if (sendto(udpSocket, message, 4, 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
    perror("sendto() failed");
    closesocket(udpSocket);
    cleanNetwork();
    return EXIT_FAILURE;
  }
 
  /*reception de la reponse du serveur*/
  if (recvfrom(udpSocket, receiveBuffer, BUFFER_SIZE, 0, (SOCKADDR*)&serverAddr, &serverAddrSize) == SOCKET_ERROR) {
    perror("recvfrom() failed");
    closesocket(udpSocket);
    cleanNetwork();
    return EXIT_FAILURE;
  }

  printf("Server response: %s\n", receiveBuffer);

  closesocket(udpSocket);
  cleanNetwork();

  return EXIT_SUCCESS;
}