/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <stdlib.h>
#include <unistd.h>

 
int main(int argc , char *argv[])
{

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

    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("No se pudo crear el socket");
        return 1;
    }
     
    server.sin_addr.s_addr = inet_addr(ip_address);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("Error en la conexion");
        return 1;
    }
    else{

		message[0] = 0x01;			//para start connection
		message[1] = 0x00;
		message[2] = 0x00;
		message[3] = '\0';
        if(send(sock , message , 4 , 0) < 0){
            puts("Send failed");
            return 1;
        }
    }
         
    //keep communicating with server
    char nombre_contricante[2000];
    int largo_nombre_contrincante;
    while(1)
    {
        if(recv(sock , server_reply, 2000, 0) < 0){
            puts("recv failed");
        }
        else {
            char nombre[2000];
            int n;
            char* payload_size;

            switch(server_reply[0])
            {
                case 0x02:
                    printf("Connection established\n");
                    break;
                case 0x03:
                    printf("Enter nickname: ");
                    scanf("%s" , nombre);               //TODO malo pasarlo a binario
                    // printf("largo %zu\n", strlen(nombre));
                    n = strlen(nombre);
                    payload_size = (char*)&n;
                    message[0] = 0x04;      //return nickname
                    message[1] = *payload_size;
                    memcpy(message + 2, nombre, strlen(nombre));
                    // printf("mess: %s\n", message);
                    if(send(sock , message , 2 + n , 0) < 0){
                        puts("Send failed");
                        return 1;
                    }
                    break;

                case 0x05:
                    largo_nombre_contrincante = (int)server_reply[1];

                    memcpy(nombre_contricante, server_reply + 2, largo_nombre_contrincante);
                    nombre_contricante[largo_nombre_contrincante] = '\0';
                    printf("Opponent found\n");
                    printf("Tu contricante se llama %s\n",nombre_contricante);
                    break;
                default:
                    printf("Default error No implementado");
            }
        }
    }
    return 0;
}