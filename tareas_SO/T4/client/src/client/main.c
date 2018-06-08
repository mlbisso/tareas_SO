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
#include "funcs.h"

 
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
        //Start connection
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
    char payload_size[8];
    char large[8];
    while(1)
    {
        if(recv(sock , server_reply, 2000, 0) < 0){
            puts("recv failed");
        }
        else {
            char nombre[2000];
            int n;
            char* name_with_extension;
            // char otherString[8];
            // char otherString_2[8]; 
            char id[8];
            memcpy(id, server_reply, 8);
            id[8] = '\0';
            int id_switch;
            id_switch = binary_to_decimal(id);
            // char* start_2;
            // start_2 = &id[0];
            // int total_2;
            // while (*start_2)
            // {
            //     total_2 *= 2;
            //     if (*start_2++ == '1') total_2 += 1;
            // }
            // printf("total %d\n", id_switch);
            printf("id_switch %d\n", id_switch);

            switch(id_switch)
            {
                case 2:
                    printf("Connection established\n");
                    break;
                case 3:
                    printf("Enter nickname: ");
                    scanf("%s" , nombre);               //TODO malo pasarlo a binario
                    // printf("largo %zu\n", strlen(nombre));
                    n = strlen(nombre);
                    int_to_bits(payload_size, n);
                    // printf("payload_size: %s\n", payload_size);
                    // message[0] = 0x04;      //return nickname
                    // message[1] = *payload_size;
                    memcpy(message, "00000100", 8);
                    memcpy(message + 8, payload_size, 8);
                    memcpy(message + 16, nombre, strlen(nombre));
                    // printf("mess: %s\n", message);
                    if(send(sock , message , 2 * 8 + n , 0) < 0){
                        puts("Send failed");
                        return 1;
                    }
                    break;

                case 5:
                    memcpy(large, server_reply + 8, 8);
                    largo_nombre_contrincante = binary_to_decimal(large);

                    memcpy(nombre_contricante, server_reply + 2 * 8, largo_nombre_contrincante);
                    nombre_contricante[largo_nombre_contrincante] = '\0';
                    printf("Opponent found\n");
                    printf("Tu contrincante se llama %s\n",nombre_contricante);
                    break;

                case 6:
                    // printf("ESta bien? %d\n", server_reply[1]);
                    // bet = ((server_reply[2] << 8) | server_reply[3]);
                    // bet = (int)server_reply[3]; 
                    // strncpy(otherString, server_reply + 16, 8);
                    // strncpy(otherString_2, server_reply + 24, 8);
                    // otherString[8] = '\0'; // place the null terminator
                    // otherString_2[8] = '\0'; // place the null terminator
                    // name_with_extension = malloc(8+1+8); /* make space for the new string (should check the return value ...) */
                    // strcpy(name_with_extension, otherString); /* copy name into the new var */
                    // strcat(name_with_extension, otherString_2); /* add the extension */
                    name_with_extension = concatenate_two_strings(server_reply);
                    printf("Initial bet: %s\n", name_with_extension);
                    // start = &name_with_extension[0];
                    // while (*start)
                    // {
                    //     total *= 2;
                    //     if (*start++ == '1') total += 1;
                    // }
                    // int character_bin_int(char binario[])
                    // if (strlen(name_with_extension) == 1){
                        // printf("NUMERO: %d \n", binary_to_decimal(name_with_extension));
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