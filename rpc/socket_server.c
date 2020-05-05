/*
 *	Author: Oulis Evangelos
 *	Class:	Friday 2-4
 *	Subject:	Distributed Systems
 *	Description:	Sockets-C server code.
 *	File Name:	socket_server.c
 */
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include "packet.h"
#include "remoteproc.h"

//Threaded server code.
void *connectionHandler (void *args);
void error(char *msg);
void errorThread(char *msg);
void timeLog(void);

struct threadArg {
	int newsock;
	char *host;
};

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen, *newsock;
     float r;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 3) {
         fprintf(stderr,"ERROR, no port provided\n");
         fprintf(stderr, "ERROR, no rpc host provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0); 
     if (sockfd < 0) 
         error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]);

     serv_addr.sin_family = AF_INET;

     serv_addr.sin_port = htons(portno);

     serv_addr.sin_addr.s_addr = INADDR_ANY;

     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
                  error("ERROR on binding");

     listen(sockfd,5);

   for (;;)  {
     timeLog();
     printf("Waiting for a connection...\n");
     clilen = sizeof(struct sockaddr_in);
     
     if ( newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen) ) {
     	timeLog();
     	printf("Request accepted!\n");
     }
     else
     	error("Request aborted!");
     
     pthread_t pthread_id;
     newsock = (int *)malloc(sizeof(int));
     *newsock = newsockfd;
     
     struct threadArg *th;
     th = (struct threadArg *)malloc(sizeof(struct threadArg));
     th->newsock = *newsock;
     th->host = argv[2];
     
     if ( pthread_create(&pthread_id, NULL, connectionHandler, (void *) th) < 0) {
     	error("Could not create thread!");
     	exit(1);
     }
     
   }
            
     return 0; 
}



void *connectionHandler (void *args)
{
	struct threadArg th = *(struct threadArg *) args;
	char *host = th.host;
	int newsockfd = th.newsock;
	int *_data, length, n;

        //RPC Identifiers
	CLIENT *clnt;
	float *result_1;
	data meshtimh_1_arg;
	result1 *result_2;
	data maxmin_1_arg;
	new_array *result_3;
	argument1 powerarray_1_arg;
	
	int selection;
        do
	{       //Receive length from client.
        	if( !recv(newsockfd, &length, sizeof(int), 0) ) {
			errorThread("Something went wrong on receiving!");
		}
		
		_data = (int *)malloc(length*sizeof(int));
			
		if (_data == NULL) {
			errorThread("Could not allocate memory.");
		}
		//Receive _data from client.		
		if ( !recv(newsockfd, _data, length*sizeof(int), 0) )
			errorThread("Server could not receive message.");
			
		clnt = clnt_create (host, MATHPROG, MATHVERBS, "udp");
		if (clnt == NULL) {
			clnt_pcreateerror (host);
			errorThread("Couldn't connect to rpc server!");
		}
		
		int j, sum=0, count=0;
		int max, min;
		float mesh_timh;
		float r, *_new_array;
		struct socket_result1 *res;
		do{
			//Receive Client Selection from Client.
			if ( !recv(newsockfd, &selection, sizeof(int), 0) )
        			errorThread("Server could not receive message.");
        		
			switch ((int) selection) {
				case 1:
					//To Send data (length & data) table to rpc server.
					meshtimh_1_arg.data_len = length;
					meshtimh_1_arg.data_val = (int *)malloc(length * sizeof(int));
					
					*meshtimh_1_arg.data_val = *_data;
					
					//Copie Data.
					for (j=0; j<length; j++)
						meshtimh_1_arg.data_val[j] = _data[j];
					result_1 = meshtimh_1(&meshtimh_1_arg, clnt);
					
					//Call rpc
					if (result_1 == (float *) NULL) {
						clnt_perror (clnt, "RPC call failed");
					}
					
					mesh_timh = (float) *result_1;
					if ( !send (newsockfd, &mesh_timh, sizeof(float), 0) )
						errorThread("Error on send response to client.");
					free(meshtimh_1_arg.data_val);
					break;
				case 2:
					//For rquest data table.
					maxmin_1_arg.data_len = length;
					maxmin_1_arg.data_val = (int *)malloc(length * sizeof(int));
					
					//Copie Data.
					for (j=0; j<length; j++)
						meshtimh_1_arg.data_val[j] = _data[j];
					//Call rpc.
					result_2 = maxmin_1(&maxmin_1_arg, clnt);
					if (result_2 == (result1 *) NULL) {
						clnt_perror (clnt, "RPC call failed");
					}
					
					res = (struct socket_result1 *) result_2;
					
					if ( !send (newsockfd, res, sizeof(struct socket_result1), 0) )
						errorThread("Error on send response to client.");
					
					free(maxmin_1_arg.data_val);
					//free(res);
					break;
				case 3:
					if ( !recv (newsockfd, &r, sizeof(float), 0) )
						errorThread("Error on receive data from client.");
						
					_new_array = (float *)malloc(length * sizeof(float));
					
					powerarray_1_arg.data.data_len = length;
					powerarray_1_arg.data.data_val = (int *)malloc(length * sizeof(int));
					//Copie Data.
					for (j=0; j<length; j++)
						powerarray_1_arg.data.data_val[j] = _data[j];
					powerarray_1_arg.r = r;
					
					//Call rpc.
					result_3 = powerarray_1(&powerarray_1_arg, clnt);
					if (result_3 == (new_array *) NULL) {
						clnt_perror (clnt, "call failed");
					}
					
					for (j=0; j<result_3->new_array_len; j++)
						_new_array[j] = result_3->new_array_val[j];
						
					if ( !send (newsockfd, _new_array, length * sizeof(float), 0) )
						errorThread("Error on send response to client.");
						
					free(_new_array);
					free(powerarray_1_arg.data.data_val);
					break;
				case 4:
					break;
				case 0:
					clnt_destroy (clnt);
					free(_data);
					free((struct threadArg *)args);
					timeLog();
					printf("A connection closed!\n");
					close(&newsockfd);
					break;
				default:
					break;
			}
			
		} while ( selection != 4 && selection != 0 );
	}while ( selection != 0);
	
	//pthread_exit(NULL);
}


void error(char *msg)
{
    timeLog();
    perror(msg);
    exit(1);
}


void errorThread(char *msg)
{
	timeLog();
	perror(msg);
	pthread_exit(NULL);
}

void timeLog(void)
{
	time_t s = time(NULL);
	struct tm *current_time;
	current_time = localtime(&s);
	
	printf("[%02d:%02d:%02d]\t:\t", current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
}
