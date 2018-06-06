#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
 
//the thread function
void *connection_handler(void *);

int jugadores = 0;
 
int main(int argc , char *argv[]){

	if (argc != 5 || strcmp("-i", argv[1]) != 0 || strcmp("-p", argv[3]) != 0)
	{
		printf("Uso: %s -i <ip_address> -p <tcp-port> \nDonde\n", argv[0]);
		printf("\t <ip_address> es la direccion IP que va a ocupar el servidor para iniciar\n");
		printf("\t <tcp-port> es el puerto TCP donde se establecer´an las conexiones. \n");
		return 1;
	}

	int port = atoi(argv[4]);
	char* ip_address = argv[2];

    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    char message[1000] , client_reply[2000];
    int sock;
    sock = socket(AF_INET , SOCK_STREAM , 0);

     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip_address);
    server.sin_port = htons(port);
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
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
    int num_jugador;
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
    
    if(recv(sock, client_message, 2000, 0) < 0){
        puts("recv failed");
    } 
    printf("lo que recibi\n");
    if (client_message[0] == 0x01){
    	jugadores += 1;
    	num_jugador = jugadores;
    	printf("Start connection client %d\n", num_jugador);
		client_message[0] = 0x02;			//para connection established
		client_message[1] = 0x00;
		client_message[2] = 0x00;
		client_message[3] = '\0';

	    if(send(sock , client_message , 4 , 0) < 0){
	        puts("Send failed");
	    }

	   	client_message[0] = 0x03;			//ask_nickname
		client_message[1] = 0x00;
		client_message[2] = 0x00;
		client_message[3] = '\0';

	    if(send(sock , client_message , 4 , 0) < 0){
	        puts("Send failed");
	    }
    }
    //Send some messages to the client
    // message = "Greetings! I am your connection handler\n";
    // write(sock , message , strlen(message));
     
    // message = "Now type something and i shall repeat what you type \n";
    // write(sock , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {  	
        //Send the message back to client
        write(sock , client_message , strlen(client_message));
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