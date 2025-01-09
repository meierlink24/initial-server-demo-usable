#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CLIENTS 5
#define BUFFER_SIZE 256

int init_server(int port);
void handle_clinet(int client_socket);
void broadcast_message(const char *message, int sender_socket);
void cleanup(int server_socket);








#endif
