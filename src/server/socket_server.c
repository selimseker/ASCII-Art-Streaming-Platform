#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "socket_server.h"
#include "stream.h"
#define MAX 80
#define SA struct sockaddr
#define TRUE 1
#define FALSE 0

Server *init_server(Stream **streams, int port)
{

	Server *server = malloc(sizeof(Server));
	server->channels = streams;
	server->PORT = port;
	//initialise all client_socket[] to 0 so not checked
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		server->client_socket[i] = 0;
	}

	//create a master socket
	if ((server->master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	int opt = TRUE;
	if (setsockopt(server->master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
				   sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	//type of socket created
	server->address.sin_family = AF_INET;
	server->address.sin_addr.s_addr = INADDR_ANY;
	server->address.sin_port = htons(server->PORT);

	return server;
}

Worker *init_worker(int socket_disc, int channel_id, char **buffer_pointer)
{
	Worker *worker = malloc(sizeof(Worker));
	worker->socket_disc = socket_disc;
	worker->channel_id = channel_id;
	worker->message_length = 0;
	worker->channel_buffer_pointer = buffer_pointer;
	sem_init(&(worker->mutex), 0, 0);
	return worker;
}

void serve(Server *server)
{

	//bind the socket to localhost port 8888
	if (bind(server->master_socket, (struct sockaddr *)&(server->address), sizeof(server->address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", server->PORT);

	//try to specify maximum of 3 pending connections for the master socket
	if (listen(server->master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//accept the incoming connection
	int addrlen = sizeof(server->address);
	puts("Waiting for connections ...");
	int max_socket_desc;
	int temp_socket_desc;
	int new_socket;
	char *message = "welcome! \r\n";
	int read_length;
	int channel_id;
	int thread_id;
	while (TRUE)
	{
		//clear the socket set
		FD_ZERO(&(server->readfds));

		//add master socket to set
		FD_SET(server->master_socket, &(server->readfds));
		max_socket_desc = server->master_socket;

		//add child sockets to set
		for (int i = 0; i < MAX_CLIENT; i++)
		{
			//socket descriptor
			temp_socket_desc = server->client_socket[i];

			//if valid socket descriptor then add to read list
			if (temp_socket_desc > 0)
				FD_SET(temp_socket_desc, &(server->readfds));

			//highest file descriptor number, need it for the select function
			if (temp_socket_desc > max_socket_desc)
				max_socket_desc = temp_socket_desc;
		}

		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		int activity = select(max_socket_desc + 1, &(server->readfds), NULL, NULL, NULL);

		if ((activity < 0))
		{
			printf("select error");
		}

		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(server->master_socket, &(server->readfds)))
		{
			if ((new_socket = accept(server->master_socket, (struct sockaddr *)&(server->address), (socklen_t *)&addrlen)) < 0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa((server->address).sin_addr), ntohs((server->address).sin_port));

			//send new connection greeting message
			if (send(new_socket, message, strlen(message), 0) != strlen(message))
			{
				perror("send");
			}

			puts("Welcome message sent successfully");

			//add new socket to array of sockets
			for (int i = 0; i < MAX_CLIENT; i++)
			{
				//if position is empty
				if (server->client_socket[i] == 0)
				{
					server->client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n", i);

					break;
				}
			}
		}

		//else its some IO operation on some other socket
		for (int i = 0; i < MAX_CLIENT; i++)
		{
			temp_socket_desc = server->client_socket[i];

			if (FD_ISSET(temp_socket_desc, &(server->readfds)))
			{
				//Check if it was for closing , and also read the
				//incoming message
				if ((read_length = read(temp_socket_desc, server->buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(temp_socket_desc, (struct sockaddr *)&(server->address),
								(socklen_t *)&addrlen);
					printf("Host disconnected , ip %s , port %d \n",
						   inet_ntoa((server->address).sin_addr), ntohs((server->address).sin_port));

					//Close the socket and mark as 0 in list for reuse
					close(temp_socket_desc);
					server->client_socket[i] = 0;
				}

				//Echo back the message that came in
				else
				{
					// read the message, if that is a first message then it should be the channel
					// if not, then ignore it

					if (server->worker_threads[i] != 0)
					{
						continue;
					}
					else
					{
						// else if it is a close message
					}

					server->buffer[read_length] = '\0';
					channel_id = atoi(server->buffer);
					printf("CHANNEL %d\n", channel_id);
					Worker *worker = init_worker(temp_socket_desc, channel_id, &(server->channels[channel_id]->consumer->out_buffer));

					server->channels[channel_id]->consumer->workers[server->channels[channel_id]->consumer->active_workers] = worker;
					server->channels[channel_id]->consumer->active_workers += 1;

					if ((thread_id = pthread_create(&(server->worker_threads[i]), NULL, &work, worker)))
					{
						printf("Thread creation failed: %d\n", thread_id);
					}
				}
			}
		}
	}

	for (int i = 0; i < MAX_CLIENT; i++)
	{
		if (server->worker_threads[i] != NULL)
		{
			pthread_join(server->worker_threads[i], NULL);
		}
	}

	return 0;
}

void work(Worker *worker)
{
	int counter = 0;
	while (1)
	{
		sem_wait(&(worker->mutex));
		send(worker->socket_disc , *(worker->channel_buffer_pointer), strlen(*(worker->channel_buffer_pointer)) ,0);
	}
}
