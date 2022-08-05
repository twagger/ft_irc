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

#define BOTNAME 	"Impostor"

#define PORT 		6667    /* Le port où le client se connectera */

#define MAXDATASIZE 100 /* Tampon d'entrée */


void closefd(int fd) {
	if (close(fd) == -1)
		printError("close error", 1, true);
	return ;	
}

bool setupClientBot(int *sockfd, const char *hostname, int port) {

	struct hostent *he;
	struct sockaddr_in their_addr; /* Adresse de celui qui se connecte */

	if ((he=gethostbyname(hostname)) == NULL) {  /* Info de l'hôte */
		printError("gethostbyame error", 1, true);
		return (false);
	}
	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printError("socket error", 1, true);
		return (false);
	}
	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(port);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero pour le reste de struct */
	if (connect(*sockfd, (struct sockaddr *)&their_addr, \
										sizeof(struct sockaddr)) == -1) {
		printError("connect error", 1, true);
		closefd(*sockfd);
		return (false);
	}
	return (true);
}

bool connectServiceBot(int *sockfd, std::string password, std::string botName) {
	
	std::string registration;

	registration = "PASS ";
	registration.append(password)
	.append("\r\n")
	.append("SERVICE ")
	.append(botName)
	.append(" * *.42paris.fr 0 0 :Our bot offer amazing services such as quotes messaging.")
	.append("\r\n");
	if (send(*sockfd, registration.c_str(), registration.size(), 0) == -1) {
		printError("send", 1, true);
		return (false);
	}
	std::cout << "IRCbot connection request: " << std::endl << registration << std::endl;
	return (true);
}

int	connect(std::string received, int registered, int *sockfd, std::string pwd, std::string *botName) {
	if (registered == 0 && received.find("383") != std::string::npos)
		return (1);
	else if (registered == 0 && received.find("433") != std::string::npos) {
		*botName += "_";
		if (connectServiceBot(sockfd, pwd, *botName) == false) {
			return (-1);
		}
	}
	return (0);
}


int main(int argc, char *argv[])
{
	int sockfd = 0;
	int numbytes = 1;
	int port;
	char buf[MAXDATASIZE];
	std::string pwd = argv[2];
	std::string received;
	std::string botName = BOTNAME;
	int 		registered = 0;

	if (argc != 3 && argc != 4) {
		std::cerr << "usage: ./bot hostname password [PORT]" << std::endl;
		exit(1);
	}
	else if (argc == 4)
		port = std::atol(argv[3]);
	else
		port = PORT;
	
	if (setupClientBot(&sockfd, argv[1], port) == false)
		return (1);
	if (connectServiceBot(&sockfd, pwd, botName) == false) {
		closefd(sockfd);
		return (1);
	}
	while (1) {
		// try to register if registration failed because nickname unavailable
		if (registered == 0)
			if ((registered = connect(received, registered, &sockfd, pwd, &botName)) == -1) {
				closefd(sockfd);
				return (1);
		}
		// receive server replies
		if ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
			printError("recv", 1, true);
			return (1);
		}
		if (numbytes > 0) {
			buf[numbytes] = '\0';
			received = buf;
			std::cout << received << std::endl;
		}
	}
	closefd(sockfd);
	return (0);
}