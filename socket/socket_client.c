/*
 *	Author: Oulis Evangelos
 *	Class:	Friday 2-4
 *	Subject:	Distributed Systems
 *	Description:	Sockets-C client code.
 *	File Name:	socket_client.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "packet.h"

void error(char* msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char** argv)
{
	int sockfd, portno, n, t;
	int length;
	float r;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int *data;
	
	if (argc != 3)
	{
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd < 0)
	{
		error("ERROR opening socket.");
		exit(0);
	}
		
	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		fprintf(stderr, "ERROR, no such host.\n");
		exit(0);
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	
	printf("Trying to connect...\n");
	
	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connection.");
		
	printf("Connected\n");
	int selection;
	do
	{
		//if (data != NULL)
		//	free(data);
		printf("Give the length of array:\t");
		scanf("%d", &length);
		
		data = (int *)malloc(length*sizeof(int));
		if (data == NULL) {
			error("ERROR on memory allocation!");
			exit(0);
		}
		
		printf("Give the array:\n");
		
		int j;
		for(j=0; j<length; j++)
		{
			printf("\t> ");
			scanf("%d", &data[j]);
		}
		
		//Send Length to Server
		if (!send(sockfd, &length, sizeof(int), 0))
			error("Error on sending.");
			
		//Send Data to Server
		if (!send(sockfd, data, length*sizeof(int), 0))
			error("Error on sending.");
					
		printf("Packet Sended!\n");
		
		struct socket_result1 *res;
		float mesh_timh;
		float *new_array;
		
		do{
			printf("\n\n============MENU==============\n");
			printf("1.Mesh timh dianismatos.\n");
			printf("2.Megisth kai elaxisth timh.\n");
			printf("3.To ginomeno r * x.\n");
			printf("4.Re-enter array.\n");
			printf("0.To exit.\n");
			printf("Option:\t");
			scanf("%d", &selection);
			
			//Send Selection to Server
			if ( !send(sockfd, &selection, sizeof(int), 0) )
				error("Error on sending.");
				
			switch ((int) selection) {
				case 1:
					if( !recv(sockfd, &mesh_timh, sizeof(float), 0) )
						error("Error on receiving.");
						
					printf("Response:\n\tMesh Timh = \t%.3f\n", mesh_timh);
					break;
				case 2:
					res = (struct socket_result1 *)malloc(sizeof(struct socket_result1));
					
					if ( !recv(sockfd, res, sizeof(struct socket_result1), 0))
						error("Error on receiving data.");
					
					printf("Response:\n\tMAX = %d\n\tMIN = %d\n", res->max, res->min);
					free (res);
					break;
				case 3:
					printf("Give the float r:\n\t> ");
					scanf("%f", &r);
					
					if ( !send (sockfd, &r, sizeof(float), 0) )
						error("Error on receiving data.");
					
					new_array = (float *)malloc(length * sizeof(float));
					
					if ( !recv(sockfd, new_array, length * sizeof(float), 0 ) )
						error("Error on receiving data.");
					
					printf("=>NEW ARRAY:\n");
					for (j=0; j<length; j++)
					{
						printf("\t>%.3f\n", new_array[j]);
					}
					
					free(new_array);
					break;
				case 4:
					printf("Re-enter array!\n");
					break;
				case 0:
					printf("Bye!\n");
					break;
				default:
					printf("False selection!\n");
					break;
			}
			
		} while ( selection != 4 && selection != 0 );
	}while ( selection != 0);
	
	close(&sockfd);
	free(data);
	
	return 0;
}
