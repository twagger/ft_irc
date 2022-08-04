#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h>
#include <string>
#include <iostream> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include "./includes/utils.hpp"

#define PORT 6667    /* Le port où le client se connectera */

#define MAXDATASIZE 100 /* Tampon d'entrée */


void setupClientBot(int *sockfd, const char *hostname, int port) {

	struct hostent *he;
	struct sockaddr_in their_addr; /* Adresse de celui qui se connecte */

	if ((he=gethostbyname(hostname)) == NULL) {  /* Info de l'hôte */
		printError("gethostbyame error", 1, true);
		exit(1);
	}

	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printError("socket error", 1, true);
		exit(1);
	}

	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(port);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero pour le reste de struct */

	if (connect(*sockfd, (struct sockaddr *)&their_addr, \
										sizeof(struct sockaddr)) == -1) {
		printError("connect error", 1, true);
		exit(1);
	}
}

bool connectServiceBot(int *sockfd) {
	
	std::string str_to;

	str_to = "PASS pwd\r\nUSER amongus 16 * :IRC bot\r\nNICK Impostor\r\n";
	if (send(*sockfd, str_to.c_str(), str_to.size(), 0) == -1)
		printError("send", 1, true);
	std::cout << "IRCbot connection request: " << std::endl << str_to << std::endl;
	return true;
}

int main(int argc, char *argv[])
{
	int sockfd = 0;
	int numbytes = 1;
	int port;
	char buf[MAXDATASIZE];
	std::string str_to;

	if (argc != 3 && argc != 4) {
		std::cerr << "usage: ./bot hostname password [PORT]" << std::endl;
		exit(1);
	}
	else if (argc == 3)
		port = std::atol(argv[2]);
	else
		port = PORT;

	setupClientBot(&sockfd, argv[1], port);
	connectServiceBot(&sockfd);

	while (1) {

		if ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
			printError("recv", 1, true);
			exit(1);
		}
		else if (numbytes > 0) {
			buf[numbytes] = '\0';
			printf("%s", buf);
		}
	}
	close(sockfd);
	return 0;
}