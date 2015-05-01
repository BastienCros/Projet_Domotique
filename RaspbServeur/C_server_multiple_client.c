/*
    C socket server example, handles multiple clients using threads
*/
 
#include<stdio.h>
#include<string.h>    	//strlen
#include<stdlib.h>    	//strlen
#include<sys/socket.h>
#include<arpa/inet.h> 	//inet_addr
#include<unistd.h>    	//write
#include<pthread.h> 	//for threading , link with pthread : gcc -pthread file.c -o ../*directory*/Name_Programme
#include "jsmn.h" 

//the thread function
void *connection_handler(void *);

static int jsoneq(const char *json, jsmntok_t *tok, const char *s);
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
     
    //Create socket using socket( Address family : ipv4, Type : TCP protocol, Protocol : IP )
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;			// Addresse Ipv4
    server.sin_addr.s_addr = INADDR_ANY;		// Supporte toutes les addresses
    server.sin_port = htons( 8888 );			// Port 8888 (serveur)
     
    //Bind : bound the sockaddr_in server to the socket socket_desc
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen : mark socket_desc as passive socket, used to accept connection
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
	//Create a thread which call connection_handler(new_sock) : This will handle connection for each client
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];
	char *message2;

    	//variable pour json parser
    	int i_Json;
	int r_Json;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	//init parser
	jsmn_init(&p);
     
	//Send some messages to the client
	/*message2 = "Greetings! I am your connection handler\n";
	write(sock , message2 , strlen(message2));

	message2 = "Now type something and i shall repeat what you type \n";
	write(sock , message2 , strlen(message2));*/

	//Receive a message from client
	while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
	{

		//unparsed the message
		r_Json = jsmn_parse(&p, client_message, strlen(client_message), t, sizeof(t)/sizeof(t[0]));
		if (r_Json < 0) {
			printf("Failed to parse JSON: %d\n", r_Json);
		}

		/* Assume the top-level element is an object */
		if (r_Json< 1 || t[0].type != JSMN_OBJECT) {
			printf("Object expected\n");
		}

		/* Loop over all keys of the root object */
		for (i_Json = 1; i_Json < r; i_Json++) {
			if (jsoneq(JSON_STRING, &t[i], "name") == 0) {
				/* We may use strndup() to fetch string value */
				printf("- Name: %.*s\n", t[i+1].end-t[i+1].start,
						JSON_STRING + t[i+1].start);
				i++;
			} else if (jsoneq(JSON_STRING, &t[i], "score") == 0) {
				/* We may additionally check if the value is either "true" or "false" */
				printf("- Score: %.*s\n", t[i+1].end-t[i+1].start,
						JSON_STRING + t[i+1].start);
				i++;
			} else if (jsoneq(JSON_STRING, &t[i], "current") == 0) {
				/* We may want to do strtol() here to get numeric value */
				printf("- Current: %.*s\n", t[i+1].end-t[i+1].start,
						JSON_STRING + t[i+1].start);
				i++;
			} else if (jsoneq(JSON_STRING, &t[i], "nickname") == 0) {
				/* We may want to do strtol() here to get numeric value */
				printf("- Nickname: %.*s\n", t[i+1].end-t[i+1].start,
						JSON_STRING + t[i+1].start);
				i++;
			} else {
				printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
						JSON_STRING + t[i].start);
		}

		//Send the message back to client
		int i;
		write(sock , client_message , strlen(client_message));
		//And print the client's message to the terminal (does not inform about the client)
		puts("\nMessage du client : ");
		for (i=0; i<strlen(client_message); i++)
		{
			printf("0x%x", client_message[i]);
		}
			puts("\n");
	}

	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
	 
	//Free the socket pointer
	free(socket_desc);

	return 0;
}

// test l'égalité d'un token (d'une trame Json) avec un string
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
