#include "Server.h"

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <fcntl.h>


 static int client_sockets[MAX_CLIENTS] = {0};

int init_server() {
	int server_socket;
	struct sockaddr_in server_addr;

	//actual socket - I  know that the code is shitty
	
        if ((server_socket = socket (AF_INET, SOCK_STREAM, 0)) == 0) {
	perror("SOCKET CREATION FAILED!");
	exit(EXIT_FAILURE);
	}	

	// server address structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_family = INADDR_ANY;
	server_addr.sin_port = htons(port);
       
       	//bind socket
	if (bind(server_socket,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
	perror("BIND FAILED");
	close(server_socket);
	exit(EXIT_FAILURE);
	}

        //listening for connections
	

        if (listen(server_socket, MAX_CLIENTS) < 0) {

        perror("Listen failed");

        close(server_socket);

        exit(EXIT_FAILURE);

    }


    printf("Server listening on port %d\n", port);

    return server_socket;

}


void handle_client(int client_socket) {

    char buffer[BUFFER_SIZE];

    int bytes_read;


    while ((bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) {

        buffer[bytes_read] = '\0'; // Null-terminate the string

        printf("Received: %s", buffer);

        broadcast_message(buffer, client_socket);

    }


    if (bytes_read == 0) {

        printf("Client disconnected\n");

    } else {

        perror("Read error");

    }


    close(client_socket);

}


void broadcast_message(const char *message, int sender_socket) {

    for (int i = 0; i < MAX_CLIENTS; i++) {

        if (client_sockets[i] != 0 && client_sockets[i] != sender_socket) {

            send(client_sockets[i], message, strlen(message), 0);

        }

    }

}


void cleanup(int server_socket) {

    close(server_socket);

    printf("Server shut down\n");

}





