#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main(void) {
	// Declare initial values;
	int domain, sock_fd, type;
	struct sockaddr_in localAddr;
	socklen_t addr_length;
	type = SOCK_STREAM;
	domain = PF_INET;
	sock_fd = socket(domain, type, 0);
	if (sock_fd == -1) {
		printf("An exception has occured in creating socket!\nExiting...\n");
		exit(-1);
	}
	
	// Setup host address properties
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(PORT);
	localAddr.sin_addr.s_addr = 0;
	memset(&(localAddr.sin_zero), '\0', 8);

	// Bind host to local address
	if (bind(sock_fd, (struct sockaddr *) &localAddr, sizeof(struct sockaddr)) == -1) {
		printf("An error has occured in binding!\nExiting...\n");
		exit(-1);
	}
	
	// Setup listening
	listen(sock_fd, 1);
	if (sock_fd == -1) {
		printf("AN error has occured in listening!\nEXiting...\n");
		exit(-1);
	}
	
	char buffer[1024];
	int sin_size, recv_len;
	while (1) {
		int client_fd;
		struct sockaddr_in clientAddr;
		sin_size = sizeof(clientAddr);
		client_fd = accept(sock_fd, (struct sockaddr *) (&clientAddr), &sin_size);
		if (client_fd == -1) {
			printf("Could not accept new connection");
			exit(-1);
		}
		send(client_fd, "What\'s up?\n", 10, 0);
		recv_len = recv(client_fd, &buffer, 1024, 0);
	        while (recv_len > 0) {
			printf("Received %d bytes", recv_len);
			printf("Received string: %s", buffer);
			recv_len = recv(client_fd, &buffer, 1024, 0); 
		}	
		close(client_fd);
		break;
	}
	close(sock_fd);
	return 0;
}

