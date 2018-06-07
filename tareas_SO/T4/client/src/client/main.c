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
	if (argc != 5 || strcmp("-i", argv[1]) != 0 || strcmp("-p", argv[3]) != 0){
		printf("Uso: %s -i <ip_address> -p <tcp-port> \nDonde\n", argv[0]);
		printf("\t <ip_address> es la direccion IP en la cual el cliente se va a conectar\n");
		printf("\t <tcp-port> es el puerto TCP donde se establecer´an las conexiones. \n");
		return 1;
	}

	int port = atoi(argv[4]);
	char* ip_address = argv[2];

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

	    // if(send(sock , message , 4 , 0) < 0){
	    //     puts("Send failed");
	    //     return 1;
	    // }
	    // if(recv(sock , server_reply, 2000, 0) < 0){
     //        puts("recv failed");
     //    }
     //   	if (server_reply[0] == 0x02){
     //   		printf("Connection established\n");
     //        sleep(0.01);
     //   	}


	    // if(recv(sock , server_reply, 2000, 0) < 0){
     //        puts("recv failed");
     //    }
     //   	if (server_reply[0] == 0x03){
     //        char nombre[2000];
     //   		printf("Enter nickname: ");
     //    	scanf("%s" , nombre);				//TODO malo pasarlo a binario
     //        printf("largo %zu\n", strlen(nombre));
     //        int n = strlen(nombre);
     //        char* payload_size = (char*)&n;
     //        message[0] = 0x04;      //return nickname
     //        message[1] = *payload_size;
     //        memcpy(message + 2, nombre, strlen(nombre));
     //        // message
     //    	if(send(sock , message , strlen(message) , 0) < 0)
	    //     {
	    //         puts("Send failed");
	    //         return 1;
	    //     }
       	}
         
    //keep communicating with server
    while(1)
    {
        if(send(sock , message , 4 , 0) < 0){
            puts("Send failed");
            return 1;
        }
        if(recv(sock , server_reply, 2000, 0) < 0){
            puts("recv failed");
        }
        if (server_reply[0] == 0x02){
            printf("Connection established\n");
            // sleep(0.01);
        }


        // if(recv(sock , server_reply, 2000, 0) < 0){
        //     puts("recv failed");
        // }
        if (server_reply[0] == 0x03){
            char nombre[2000];
            printf("Enter nickname: ");
            scanf("%s" , nombre);               //TODO malo pasarlo a binario
            printf("largo %zu\n", strlen(nombre));
            int n = strlen(nombre);
            char* payload_size = (char*)&n;
            message[0] = 0x04;      //return nickname
            message[1] = *payload_size;
            memcpy(message + 2, nombre, strlen(nombre));
            // message
            // if(send(sock , message , strlen(message) , 0) < 0)
            // {
            //     puts("Send failed");
            //     return 1;
            // }
    }
}
    // close(sock);
    return 0;
}