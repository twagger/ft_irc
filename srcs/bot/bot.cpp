#include <cstdlib> 
#include <cerrno>
#include <string>
#include <cstring>
#include <iostream> 
#include <string> 
#include <unistd.h>
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 

#include "../../includes/bot.hpp"

#define BOTNAME 	"Impostor"

#define PORT 		6667

#define MAXDATASIZE 100


void closefd(int fd) {
	if (close(fd) == -1)
		printError("close error", 1, true);
	return ;	
}

bool setupClientBot(int *sockfd, const char *hostname, int port) {

	struct hostent *he;
	struct sockaddr_in their_addr;

	if ((he=gethostbyname(hostname)) == NULL) {
		printError("gethostbyame error", 1, true);
		return (false);
	}
	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printError("socket error", 1, true);
		return (false);
	}
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(port);
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);
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

int	reconnect(std::string received, int registered, int *sockfd, std::string pwd, std::string *botName) {
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

// HANDLE MESSAGES FROM THE SERVER
int handleMessage(const std::string &message, const int &fd)
{
    std::string response;
	std::string command;
	std::string target;

    // ping 
    if (message.find("PING") != std::string::npos)
        response = botPing(message);

    // other commands
    else if (message.find("PRIVMSG") != std::string::npos)
    {
        // Extract the target
        target = message.substr(1, message.find("PRIVMSG") - 1);
        // extract the command
        command = message.substr(message.find(':', message.find("PRIVMSG")));
        command = command.substr(0, command.find("\r\n"));
        if (command.compare(":QUOTE") == 0){
            // QUOTE
            response = botQuote();
        }
        else if (command.compare(":TIME") == 0){
            // TIME
            response = botTime();
        }
        if (!response.empty())
            response = std::string("PRIVMSG ").append(target)
                                              .append(":").append(response)
                                              .append("\r\n");
        std::cout << "RES : " << response << std::endl;
    }
    // Send reply
    if (!response.empty())
    {
        if (send(fd, response.c_str(), response.length(), 0) == -1)
        {
            perror("send");
            return (-1);
        }
    }
    return (0);
}

int main(int argc, char *argv[])
{
	int 		sockfd = 0;
	int 		numbytes = 1;
	char 		buf[MAXDATASIZE];
	int 		port = PORT;
	// For connection:
	std::string botName = BOTNAME;
	std::string pwd = argv[2];
	std::string received;
	int 		registered = 0;


	if (argc != 3 && argc != 4) {
		std::cerr << "usage: ./bot hostname password [PORT]" << std::endl;
		exit(1);
	}
	else if (argc == 4)
		port = std::atol(argv[3]);

	
	if (setupClientBot(&sockfd, argv[1], port) == false)
		return (1);
	if (connectServiceBot(&sockfd, pwd, botName) == false) {
		closefd(sockfd);
		return (1);
	}
	while (1) {
		
		// clear buf here before another recv
        std::memset(buf, 0, MAXDATASIZE);
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
            perror("recv");
            return (1);
        }
        else if (numbytes > 0) {
			buf[numbytes] = '\0';
			std::cout << "Received: " << buf << std::endl;
		}
        received = std::string(buf);
        if (handleMessage(received, sockfd) == -1)
        {
            close(sockfd);
            return (1);
        }
        // try to re-register if registration failed because nickname unavailable
		if (registered == 0)
			if ((registered = reconnect(received, registered, &sockfd, pwd, &botName)) == -1) {
				closefd(sockfd);
				return (1);
		}
	}
	closefd(sockfd);
	return (0);
}
