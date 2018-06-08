/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

 
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

		// message[0] = 0x01;			//para start connection
		// message[1] = 0x00;
		// message[2] = 0x00;
		// message[3] = '\0';

                            memcpy(message, "00000001", 8);
                    memcpy(message + 8, "00000000", 8);
                    memcpy(message + 16, "00000000", 8);
        if(send(sock , message , 3 * 8 , 0) < 0){
            puts("Send failed");
            return 1;
        }
    }
         
    //keep communicating with server
    char nombre_contricante[2000];
    int largo_nombre_contrincante;
    uint16_t bet = 0;
    while(1)
    {
        if(recv(sock , server_reply, 2000, 0) < 0){
            puts("recv failed");
        }
        else {
            char nombre[2000];
            int n;
            char* payload_size;
            char* name_with_extension;
            char otherString[8]; // note 6, not 5, there's one there for the null terminator
            char otherString_2[8]; // note 6, not 5, there's one there for the null terminator
                    int total = 0;
                    int i;
                    char* start;

            char id[8];
            memcpy(id, server_reply, 8);
            id[8] = '\0';
            char* start_2;
            start_2 = &id[0];
            int total_2;
            while (*start_2)
            {
                total_2 *= 2;
                if (*start_2++ == '1') total_2 += 1;
            }
            printf("total %d\n", total_2);

            switch(total_2)
            {
                case 2:
                    printf("Connection established\n");
                    break;
                case 3:
                    printf("Enter nickname: ");
                    scanf("%s" , nombre);               //TODO malo pasarlo a binario
                    // printf("largo %zu\n", strlen(nombre));
                    n = strlen(nombre);
                    payload_size = (char*)&n;
                    message[0] = 0x04;      //return nickname
                    message[1] = *payload_size;
                    memcpy(message, "00000100", 8);
                    memcpy(message + 8, payload_size , 8);
                    memcpy(message + 16, nombre, strlen(nombre));
                    // printf("mess: %s\n", message);
                    if(send(sock , message , 2 + n , 0) < 0){
                        puts("Send failed");
                        return 1;
                    }
                    break;

                case 5:
                    largo_nombre_contrincante = (int)server_reply[1];

                    memcpy(nombre_contricante, server_reply + 2, largo_nombre_contrincante);
                    nombre_contricante[largo_nombre_contrincante] = '\0';
                    printf("Opponent found\n");
                    printf("Tu contricante se llama %s\n",nombre_contricante);
                    break;

                case 6:
                    // printf("ESta bien? %d\n", server_reply[1]);
                    // bet = ((server_reply[2] << 8) | server_reply[3]);
                    // bet = (int)server_reply[3]; 
                    strncpy(otherString, server_reply + 16, 8);
                    strncpy(otherString_2, server_reply + 24, 8);
                    otherString[8] = '\0'; // place the null terminator
                    otherString_2[8] = '\0'; // place the null terminator
                    name_with_extension = malloc(8+1+8); /* make space for the new string (should check the return value ...) */
                    strcpy(name_with_extension, otherString); /* copy name into the new var */
                    strcat(name_with_extension, otherString_2); /* add the extension */
                    printf("Initial bet: %s\n", name_with_extension);
                    start = &name_with_extension[0];
                    while (*start)
                    {
                        total *= 2;
                        if (*start++ == '1') total += 1;
                    }
                    // int character_bin_int(char binario[])
                    // if (strlen(name_with_extension) == 1){
                        printf("NUMERO: %d \n", total);
                    // }
                    // for (i=0; i < strlen(name_with_extension)-3; i++)
                    // {
                    //     if (name_with_extension[i] == 49) // 49 es un 1 en asci
                    //     {
                    //         total += pow(2, strlen(name_with_extension)-4-i);
                    //     }
                    // }
                    // printf("NUMERO: %d \n", total);



                default:
                    printf("Default error No implementado\n");
            }
        }
    }
    return 0;
}