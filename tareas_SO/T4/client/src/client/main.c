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
    uint16_t pot = 0;
    uint16_t bet = 0;
    char payload_size[8];
    char large[8];
    char payload[2000];
    int tamano = 0;
    int carta = 0;
    int pinta = 0;
    int mi_turno = 0;
    wchar_t pinta_unicode;
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
            id_switch = binary_to_decimal(id, 8);
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
                    int_to_bits(payload_size, n, 8);
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
                    largo_nombre_contrincante = binary_to_decimal(large, 8);

                    memcpy(nombre_contricante, server_reply + 2 * 8, largo_nombre_contrincante);
                    nombre_contricante[largo_nombre_contrincante] = '\0';
                    printf("Opponent found\n");
                    printf("Tu contrincante se llama %s\n",nombre_contricante);
                    break;

                case 6:
                    name_with_extension = concatenate_two_strings(server_reply);
                    // sleep(1);
                    memcpy(large, server_reply + 8, 8); 
                    tamano = binary_to_decimal(large, 8);
                    memcpy(payload, server_reply + 16, tamano * 8);
                    pot = binary_to_decimal(payload, tamano * 8);
                    // pot = binary_to_decimal(name_with_extension, 8);
                    // pot = 1;
                    printf("Initial pot: %d\n", pot);
                    // sleep(0.5);
                    break;

                case 7:
                    printf("Comienza el juego\n");
                    break;

                case 8:
                    printf("Comienza una nueva ronda\n");
                    memcpy(large, server_reply + 8, 8); 
                    tamano = binary_to_decimal(large, 8);
                    memcpy(payload, server_reply + 16, tamano * 8);
                    pot = binary_to_decimal(payload, tamano * 8);
                    printf("Tu pot actual es: %d\n", pot);
                    break;

                case 9:
                    memcpy(large, server_reply + 8, 8); 
                    tamano = binary_to_decimal(large, 8);
                    memcpy(payload, server_reply + 16, tamano * 8);
                    bet = binary_to_decimal(payload, tamano * 8);
                    printf("Initial bet: %d\n", bet);
                    break;

                case 10:
                    printf("Estas son tus cartas\n");
                    memcpy(large, server_reply + 8, 8); 
                    tamano = binary_to_decimal(large, 8);
                    for (int i = 0; i < (tamano / 2); i++){
                        memcpy(payload, server_reply + 16 + 16 * i, 8);
                        carta = binary_to_decimal(payload, 8);
                        if (carta == 1){
                            printf("A");
                        }
                        else if (carta == 11){
                            printf("J");
                        }                        
                        else if (carta == 12){
                            printf("Q");
                        }                        
                        else if (carta == 13){
                            printf("K");
                        }
                        else{
                            printf("%d", carta);
                        }
                        memcpy(payload, server_reply + 16 + 8 + 16 * i, 8);
                        pinta = binary_to_decimal(payload, 8);
                        // if (pinta == 1){
                        //     pinta_unicode = 'U+2665';
                        // }
                        // if (pinta == 2){
                        //     pinta_unicode = 'U+2666';
                        // }                        
                        // if (pinta == 3){
                        //     pinta_unicode = 'U+2663';
                        // }                        
                        // if (pinta == 4){
                        //     pinta_unicode = 'U+2660';
                        // }
                        // printf("%c  ", pinta_unicode);
                        printf("%d  ", pinta);

                    }
                    printf("\n");
                    break;

                case 11:
                    memcpy(large, server_reply + 8, 8); 
                    tamano = binary_to_decimal(large, 8);
                    for (int i = 0; i < tamano; i++){
                        memcpy(payload, server_reply + 16 + 8 * i, 8);
                        mi_turno = binary_to_decimal(payload, 8);
                        if (mi_turno == 1){
                            printf("Comienzas jugando tu\n");
                        }
                        else if (mi_turno == 2){
                            printf("Comienza jugando tu contrincante\n");
                        }
                    }                    
                    break;

                case 12:
                    printf("Que cartas deseas cambiar?\n");
                    break;

                case 20:
                    memcpy(payload, server_reply + 16, 8);
                    if (binary_to_decimal(payload, 8) == 1){
                        printf("Ganaste :)\n");
                    }
                    else if(binary_to_decimal(payload, 8) == 2){
                        printf("Perdiste :( \n");
                    }

                case 22:
                    printf("El juego termino\n");
                    break;

                default:
                    printf("Default error No implementado\n");

                    // break;
            }
        }
    }
    return 0;
}