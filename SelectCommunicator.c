#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>

int readwrite(int in, int out);

int main(){
	fd_set fds;
	struct sockaddr_in address;
	struct hostent* host;
	int sock, activity, length = sizeof(address), max_clients = 20, max_fd, fd, new_socket, read_result;;
	int clients[max_clients];
	char* names[max_clients];

	char buff[1024];

	for (int i = 0; i < max_clients; i++){
		clients[i] = 0;
		names[i] = malloc(sizeof(char*));
	}

	host = gethostbyname("localhost");
	bzero(&address, length);
	address.sin_family = AF_INET;
	address.sin_addr = *(struct in_addr*) (host->h_addr_list[0]);
	address.sin_port = htons( 8000 );

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(sock, (struct sockaddr*) &address, length) < 0){
		perror("bind");
		exit(1);
	}
	listen(sock, 3);

	int opt = 1;
	if( setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
	
	printf("Waiting for connections...\n");
	while (1){
		FD_ZERO(&fds);
		FD_SET(sock, &fds);
		max_fd = sock;

		for (int i = 0; i < max_clients; i++){
			fd = clients[i];
			if (fd > 0){
				FD_SET(fd, &fds);
			}
			if (fd > max_fd){
				max_fd = fd;
			}
		}

		activity = select(max_fd + 1, &fds, NULL, NULL, NULL);

		if (FD_ISSET(sock, &fds)){
			new_socket = accept(sock, (struct sockaddr*) &address, &length);
			printf("-----------------------------------------------------------\n");
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
			write(new_socket, "Welcome!\nEnter login:\n", 23);
			printf("Welcome message sent!\n");

			for (int i = 0; i < max_clients; i++){
				if (clients[i] == 0){
					clients[i] = new_socket;
					read_result = read(new_socket, names[i], sizeof(names[i]));
					names[i][read_result - 2] = '\0';
					printf("Adding %s to list of sockets as %d\n", names[i], i);
					break;
				}
			}
			write(new_socket, "Enter messages according to a pattern - (user~message))\n", 57);
			printf("Currently logged:\n");
			for (int i = 0; i < max_clients; i++){
				if (clients[i] > 0){
					printf("%s ", names[i]);
				}
			}
			printf("\n");
		}

		for (int i = 0; i < max_clients; i++){
			fd = clients[i];

			if (FD_ISSET(fd, &fds)){
				if ((read_result = read(fd, buff, 1024)) == 0){
					printf("Host disconnected!\n");
					close(fd);
					clients[i] = 0;
				}
				else {
					buff[read_result] = '\0';
					char MSS[1024];
					char* receiver = strtok(buff, "~");
					char* msg = strtok(NULL, "~");
					snprintf(MSS, sizeof MSS, "[%s]: %s", names[i], msg);
					for (int j = 0; j < max_clients; j++){
						if (strcmp(names[j], receiver) == 0){
							write(clients[j], MSS, strlen(MSS));
							break;
						}
					}
				}
			}
		}
	}
	
	return 0;
}
