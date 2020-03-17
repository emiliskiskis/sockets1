#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char **argv) {
  char buf[1024];
  char *userlist = calloc(1, sizeof(char));

  struct sockaddr_in saddr, caddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(8000);
  inet_aton("127.0.0.1", &saddr.sin_addr);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("Error creating socket");
    exit(1);
  }

  if (bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr))) {
    perror("Error binding to address");
    exit(2);
  }

  listen(sockfd, 0);
  while (1) {
    if (accept(sockfd, (struct sockaddr *)&caddr, sizeof(caddr))) {
      perror("Error accepting client");
      continue;
    }
    puts("Accepted");

    int n = recv(sockfd, buf, 1024, 0);
    if (n < 0) {
      perror("Error");
    }
    if (n == 0) {
      close(sockfd);
      puts("Closed");
    } else if (n > 0) {
      printf("Received %s\n", buf);
      if (!strncmp(buf, "LIST", 4)) {
        send(sockfd, userlist, strlen(userlist), 0);
      }
      if (!strncmp(buf, "ADD", 3)) {
        userlist = realloc(userlist, strlen(userlist) + strlen(buf + 4));
        strcat(userlist, buf + 4);
        char *res = calloc(20 + strlen(buf + 4), sizeof(char));
        sprintf(res, "Added %s to user list", buf + 4);
        send(sockfd, res, strlen(res), 0);
      }
    }
  }
  close(sockfd);
}