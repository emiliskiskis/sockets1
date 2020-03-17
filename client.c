#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char **argv) {
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(8000);
  inet_aton("127.0.0.1", &saddr.sin_addr);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("Error creating socket");
    exit(1);
  }

  if (connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr))) {
    perror("Error connecting to address");
    exit(2);
  }

  char option;
  while (1) {
    puts("Choose option: ");
    puts("1) List users in system");
    puts("2) Add user to system");
    puts("3) Remove user from system");
    printf(":");
    option = getc(stdin);
    getc(stdin);  // To clear \n
    if (option == 'q') break;
    switch (option) {
      case '1':
      case '2':
        printf("Enter name of user: ");
        char query[512] = "ADD ", *name, *result;
        fgets(name, 256, stdin);
        printf("Adding...");
        send(sockfd, strcat(query, name), 3 + strlen(name) - 1, 0);
        recv(sockfd, result, 512, 0);
        puts(result);
        break;
      case '3':
        break;
      default:
        puts("Invalid option");
    }
  }

  close(sockfd);
}