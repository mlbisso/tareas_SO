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

    if ((argc == 5) && (((strcmp("-i", argv[1]) == 0) && strcmp("-p", argv[3]) == 0) || ((strcmp("-p", argv[1]) == 0) && strcmp("-i", argv[3]) == 0)))
    {  
    }
    else{
        printf("Uso: %s -i <ip_address> -p <tcp-port> \nDonde\n", argv[0]);
        printf("\t <ip_address> es la direccion IP que va a ocupar el servidor para iniciar\n");
        printf("\t <tcp-port> es el puerto TCP donde se establecer´an las conexiones. \n");
        return 1;
    }

    int port;
    char* ip_address;
    if (strcmp("-i", argv[1]) == 0){
       port = atoi(argv[4]);
       ip_address = argv[2];        
    }
    else{
       port = atoi(argv[2]);
       ip_address = argv[4];        
    }


    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    //char message[1000] , client_reply[2000];

     
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
    char message[2000];
    char client_message[2000];
    int largo_nombre;

    //Send some messages to the client
    // message = "Greetings! I am your connection handler\n";
    // write(sock , message , strlen(message));
     
    // message = "Now type something and i shall repeat what you type \n";
    // write(sock , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {  	
        //Send the message back to client
            // write(sock , client_message , strlen(client_message));
        largo_nombre = (int)client_message[1];
        char nombre[largo_nombre + 1];

        switch(client_message[0])
        {
            case 0x01:
                jugadores += 1;
                num_jugador = jugadores;
                printf("Start connection client %d\n", num_jugador);
                message[0] = 0x02;          //para connection established
                message[1] = 0x00;
                message[2] = 0x00;
                message[3] = '\0';
                if(send(sock , message , 4 , 0) < 0){
                    puts("Send failed");
                }
                message[0] = 0x03;          //ask_nickname
                message[1] = 0x00;
                message[2] = 0x00;
                message[3] = '\0';
                if(send(sock , message , 4 , 0) < 0){
                    puts("Send failed");
                }
                // client_message[0] = 0x00;
                break;
            case 0x04:
                strncpy(nombre, client_message + 2, largo_nombre);
                // printf("l1: %s", client_message);
                nombre[largo_nombre] = '\0';
                printf("El nombre del jugador %d es : %s\n", jugadores, nombre);
                jugadores += 1;
                num_jugador = jugadores;
                break;
            default:
                printf("Default en switch client message");
        }
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