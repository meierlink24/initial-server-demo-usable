#include "Server.h"

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <sys/select.h>


int main(int argc, char *argv[]) {

    if (argc != 2) {

        fprintf(stderr, "Usage: %s <port>\n", argv[0]);

        exit(EXIT_FAILURE);

    }


    int port = atoi(argv[1]);

    int server_socket = init_server(port);

    fd_set read_fds;

    int max_sd, activity;


    while (1) {

        FD_ZERO(&read_fds);

        FD_SET(server_socket, &read_fds);

        max_sd = server_socket;


        // Add child sockets to set

        for (int i = 0; i < MAX_CLIENTS; i++) {

            if (client_sockets[i] > 0) {

                FD_SET(client_sockets[i], &read_fds);

            }

            if (client_sockets[i] > max_sd) {

                max_sd = client_sockets[i];

            }

        }


        // Wait for activity on sockets

        activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);


        if (activity < 0) {

            perror("Select error");

            continue;

        }


        // Check if there is a new connection

        if (FD_ISSET(server_socket, &read_fds)) {

            int new_socket;

            struct sockaddr_in client_addr;

            socklen_t addr_len = sizeof(client_addr);


            if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) < 0) {

                perror("Accept error");

                continue;

            }


            printf("New connection: socket fd is %d\n", new_socket);


            // Add new socket to array of sockets

            for (int i = 0; i < MAX_CLIENTS; i++) {

                if (client_sockets[i] == 0) {

                    client_sockets[i] = new_socket;

                    printf("Added to list of sockets as %d\n", i);

                    break;

                }

            }

        }


        // Check for incoming messages from clients

        for (int i = 0; i < MAX_CLIENTS; i++) {

            int client_socket = client_sockets[i];

            if (FD_ISSET(client_socket, &read_fds)) {

                handle_client(client_socket);

                client_sockets[i] = 0; // Clear the socket if it was closed

            }

        }

    }


    cleanup(server_socket);

    return 0;

}
