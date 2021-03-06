#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include <time.h>
#include "funcs.h"

 
//the thread function
void *connection_handler(void *);

int jugadores = 0;
int clientes[2];                //Guarda los sockets
char nombre_j1[2000];
char nombre_j2[2000];
int pot_j1;
int pot_j2;
int bet_j1;
int bet_j2;

int carta_superior;               //cual es el indice de la prox carta a sacar

Carta ** mazo;
Carta ** mano_j1;
Carta ** mano_j2;
int comienza;
int jugador_actual;
int verf;
// nombre_j1 = ;
// nombre_j2 = ;

int main(int argc , char *argv[]){

    if ((argc == 5) && (((strcmp("-i", argv[1]) == 0) && strcmp("-p", argv[3]) == 0) || ((strcmp("-p", argv[1]) == 0) && strcmp("-i", argv[3]) == 0))){  
    }

    else{
        printf("Uso: %s -i <ip_address> -p <tcp-port> \nDonde\n", argv[0]);
        printf("\t <ip_address> es la direccion IP que va a ocupar el servidor para iniciar\n");
        printf("\t <tcp-port> es el puerto TCP donde se establecer´an las conexiones. \n");
        return 1;
    }

    verf = 0;
    pot_j1 = 1000;
    pot_j2 = 1000;
    carta_superior = 0;
    bet_j1 = 10;
    bet_j2 = 10;
    comienza = 0;
    jugador_actual = 0;
    //TODO BORRAR
    // pot_j1 = 200;
    // pot_j2 = 200;

    mazo = inicializar_mazo();
    mano_j1 = inicializar_mano();
    mano_j2 = inicializar_mano();

    clientes[0] = -1;
    clientes[1] = -1;

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
     
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))    // && jugadores < 2)
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

    // if (jugadores >= 2){
    //     jugadores = 2;
    //     perror("Ya se tienen 2 jugadores\n");
    //     return 1;
    // }
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

    if (clientes[0] == -1){
        clientes[0] = sock;
    }
    else{
        clientes[1] = sock;
    }

    //Send some messages to the client
    // message = "Greetings! I am your connection handler\n";
    // write(sock , message , strlen(message));
     
    // message = "Now type something and i shall repeat what you type \n";
    // write(sock , message , strlen(message));
     
    //Receive a message from client
    // int n;
    // char* payload_size;
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {  	
        //Send the message back to client
            // write(sock , client_message , strlen(client_message));


        char id[8];
        memcpy(id, client_message, 8);
        id[8] = '\0';
        int id_switch;
        id_switch = binary_to_decimal(id, 8);
        printf("total %d\n", id_switch);

        char largo_payload[8];
        memcpy(largo_payload, client_message + 8, 8);
        printf("Client message: %s\n", client_message);
        largo_nombre = binary_to_decimal(largo_payload, 8);
        char nombre[largo_nombre + 1];

        int n;
        char payload_size[1000];
        char payload[2000];

        int tamano;
        int num;

        int carta = 0;
        int pinta = 0;

        switch(id_switch)
        {
            case 1:
                printf("Start  new connection \n");
                // message[0] = 0x02;          //para connection established
                // message[1] = 0x00;
                // message[2] = 0x00;
                // message[3] = '\0';

                // sleep(1);
                memcpy(message, "00000010", 8);
                memcpy(message + 8, "00000000", 8);
                memcpy(message + 16, "00000000", 8);
                sleep(1);
                if(send(sock , message , 3 * 8 , 0) < 0){
                    puts("Send failed");
                }
                // message[0] = 0x03;          //ask_nickname
                // message[1] = 0x00;
                // message[2] = 0x00;
                // message[3] = '\0';

                // sleep(1);
                memcpy(message, "00000011", 8);
                memcpy(message + 8, "00000000", 8);
                memcpy(message + 16, "00000000", 8);
                sleep(1);
                if(send(sock , message , 3 * 8 , 0) < 0){
                    puts("Send failed");
                }
                // client_message[0] = 0x00;
                break;

            case 4:
                strncpy(nombre, client_message + 2 * 8, largo_nombre);
                // printf("l1: %d\n", largo_nombre);
                nombre[largo_nombre] = '\0';
                jugadores += 1;
                num_jugador = jugadores;
                printf("El nombre del jugador %d es : %s\n", num_jugador, nombre);
                if (clientes[0] == sock){
                    memcpy(nombre_j1, nombre, largo_nombre);
                    nombre_j1[largo_nombre] = '\0';
                    // for (int i = 0; i < largo_nombre; i++){
                    //     strcpy(*nombre_j1, nombre[i]);
                    //     nombre_j1 ++;
                    // }
                }
                else{
                    memcpy(nombre_j2, nombre, largo_nombre);
                    nombre_j2[largo_nombre] = '\0';

                }
                if (jugadores == 2){
                    //TODO cambiar
                    //Opponent found
                    n = strlen(nombre_j1);
                    int_to_bits(payload_size, n, 8);
                    // printf("payload_size: %s\n", payload_size);

                    memcpy(message, "00000101", 8);
                    memcpy(message + 8, payload_size, 8);
                    memcpy(message + 16, nombre_j1, n);
                    sleep(1);
                    if(send(clientes[1] , message , 2 *8 + n , 0) < 0){
                        puts("Send failed");
                        break;
                    }
                    // printf("") 
                    n = strlen(nombre_j2);
                    int_to_bits(payload_size, n, 8);
                    // printf("payload_size: %s\n", payload_size);

                    memcpy(message, "00000101", 8);
                    memcpy(message + 8, payload_size, 8);
                    memcpy(message + 16, nombre_j2, n);
                    sleep(1);
                    if(send(clientes[0] , message , 2 * 8 + n , 0) < 0){
                        puts("Send failed");
                        break;
                    }

                    //Initial pot
                    memcpy(message, "00000110", 8);
                    memcpy(message + 8, "00000010", 8);
                    memcpy(message + 16, "00000011", 8);
                    memcpy(message + 24, "11101000", 8);
                    // message[0] = 0x06;          //Initial pot
                    // message[1] = 0x02;          //son dos bytes de payload
                    // message[2] = 0b00000011;          //1000 de pot...
                    // message[3] = 0b11101000;
                    // message[4] = '\0';

 
                    sleep(1);
                    if(send(clientes[0] , message , 4 * 8 , 0) < 0){
                        puts("Send failed");
                        break;
                    }
                    if(send(clientes[1] , message , 4 * 8 , 0) < 0){
                        puts("Send failed");
                        break;
                    }    
                    // sleep(0.5);
                    //Game start
                    memcpy(message, "00000111", 8);
                    memcpy(message + 8, "00000000", 8);
                    memcpy(message + 16, "00000000", 8);
                    memcpy(message + 24, "00000000", 8);                
                    sleep(1);
                    if(send(clientes[0] , message , 3 * 8 , 0) < 0){
                        puts("Send failed");
                        break;
                    } 
                    // sleep(2);
                    if(send(clientes[1] , message , 3 * 8 , 0) < 0){
                        puts("Send failed");
                        break;
                    } 
                     
                    //DESDE POT > 10?
                    if (pot_j1 < 10 || pot_j2 < 10){
                        //GAME END
                        memcpy(message, "00010110", 8);
                        memcpy(message + 8, "00000000", 8);
                        memcpy(message + 16, "00000000", 8); 
                        sleep(1);
                        if(send(clientes[0] , message , 3 * 8 , 0) < 0){
                            puts("Send failed");
                            break;
                        } 
                        if(send(clientes[1] , message , 3 * 8 , 0) < 0){
                            puts("Send failed");
                            break;
                        }     

                        //WINNER LOSE
                        memcpy(message, "00010100", 8);
                        memcpy(message + 8, "00000001", 8);
                        //mensaje ganador
                        memcpy(message + 16, "00000001", 8); 
                        if (pot_j1 < 10){       //el 1 perdio
                            sleep(1);
                            if(send(clientes[1] , message , 3 * 8 , 0) < 0){        //el 2 gano
                                puts("Send failed");
                                break;
                            }
                            sleep(1);
                            memcpy(message + 16, "00000010", 8);        //el 1 perdio
                            if(send(clientes[0] , message , 3 * 8 , 0) < 0){
                                puts("Send failed");
                                break;
                            }                                                   
                        }
                        else{
                            sleep(1);
                            if(send(clientes[0] , message , 3 * 8 , 0) < 0){        //el 2 gano
                                puts("Send failed");
                                break;
                            }
                            memcpy(message + 16, "00000010", 8);        //el 1 perdio
                            sleep(1);
                            if(send(clientes[1] , message , 3 * 8 , 0) < 0){
                                puts("Send failed");
                                break;
                            }     
                        }                                           
                    //TODO image  
                    }
                    else{
                        //START ROUND
                        memcpy(message, "00001000", 8);

                        tamano = obtener_tamano_bytes(pot_j1);
                        int_to_bits(payload_size, tamano, 8);       //quiero guardar en payload siz el tamano en 1B
                        memcpy(message + 8, payload_size, 8);

                        int_to_bits(payload_size, pot_j1, tamano * 8);
                        memcpy(message + 16, payload_size, tamano * 8);
                        sleep(1);
                        if(send(clientes[0] , message , 16 + tamano * 8 , 0) < 0){
                            puts("Send failed");
                            break;
                        } 

                        tamano = obtener_tamano_bytes(pot_j2);
                        int_to_bits(payload_size, tamano, 8);       //quiero guardar en payload siz el tamano en 1B
                        memcpy(message + 8, payload_size, 8);

                        int_to_bits(payload_size, pot_j2, tamano * 8);
                        memcpy(message + 16, payload_size, tamano * 8);
                        // sleep(1);
                        if(send(clientes[1] , message , 16 + tamano * 8 , 0) < 0){
                            puts("Send failed");
                            break;
                        }

                        //REPARTIR CARTAS
                        carta_superior = repartir_cartas(mazo, mano_j1, carta_superior);
                        carta_superior = repartir_cartas(mazo, mano_j2, carta_superior);

                        for (int i = 0; i < 5; i++){
                            printf("%d%d    ", mano_j1[i][0].numero, mano_j1[i][0].pinta);
                        }

                        for (int i = 0; i < 5; i++){
                            printf("%d %d\n", mano_j2[i][0].numero, mano_j2[i][0].pinta);
                        }


                        //Initial Bet
                        memcpy(message, "00001001", 8);
                        memcpy(message + 8, "00000001", 8);
                        memcpy(message + 16, "00001010", 8);  
                        sleep(1);
                        if(send(clientes[0] , message , 3 * 8 , 0) < 0){
                            puts("Send failed");
                            break;
                        }
                        if(send(clientes[1] , message , 3* 8 , 0) < 0){
                            puts("Send failed");
                            break;
                        }                       

                        //5 cards
                        memcpy(message, "00001010", 8);
                        memcpy(message + 8, "00001010", 8);
                        for (int i = 0; i < 5; i ++){
                            int_to_bits(payload_size, mano_j1[i][0].numero, 8);
                            memcpy(message + 16 + 16 * i, payload_size , 8);       //numero carta
                            int_to_bits(payload_size, mano_j1[i][0].pinta, 8);
                            memcpy(message + 24 + 16 * i, payload_size, 8);       //pinta carta                                        
                        }
                        sleep(1);
                        if(send(clientes[0] , message , 2 * 8 + 5 * 16 , 0) < 0){
                            puts("Send failed");
                            break;
                        }  

                        for (int i = 0; i < 5; i ++){
                            int_to_bits(payload_size, mano_j2[i][0].numero, 8);
                            memcpy(message + 16 + 16 * i, payload_size , 8);       //numero carta
                            int_to_bits(payload_size, mano_j2[i][0].pinta, 8);
                            memcpy(message + 24 + 16 * i, payload_size, 8);       //pinta carta                                        
                        }
                        sleep(1);
                        if(send(clientes[1] , message , 2 * 8 + 5 * 16 , 0) < 0){
                            puts("Send failed");
                            break;
                        }  

                        //WHOS FIRST
                        comienza = quien_empieza();
                        jugador_actual = comienza;
                        memcpy(message, "00001011", 8);
                        memcpy(message + 8, "00000001", 8);

                        if (comienza == 0){         //comienza el jugador 1
                            jugador_actual = 0;
                            memcpy(message + 16, "00000001", 8);
                            sleep(1);
                            if(send(clientes[0] , message , 8*3 , 0) < 0){
                                puts("Send failed");
                                break;
                            }  
                            memcpy(message + 16, "00000010", 8);
                            if(send(clientes[1] , message , 8*3 , 0) < 0){
                                puts("Send failed");
                                break;
                            }  

                            //GET CARDS TO CHANGE
                            memcpy(message, "00001100", 8);
                            memcpy(message + 8, "00000000", 8);
                            memcpy(message + 16, "00000000", 8);  
                            sleep(1); 
                            if(send(clientes[0] , message , 8*3 , 0) < 0){
                                puts("Send failed");
                                break;
                            }  
                        }

                        else if (comienza == 1){         //comienza el jugador 2
                            jugador_actual = 1;
                            memcpy(message + 16, "00000001", 8);
                            sleep(1);
                            if(send(clientes[1] , message , 8*3 , 0) < 0){
                                puts("Send failed");
                                break;
                            }  
                            memcpy(message + 16, "00000010", 8);
                            if(send(clientes[0] , message , 8*3 , 0) < 0){
                                puts("Send failed");
                                break;
                            }  
                            //GET CARDS TO CHANGE
                            memcpy(message, "00001100", 8);
                            memcpy(message + 8, "00000000", 8);
                            memcpy(message + 16, "00000000", 8);
                            sleep(1);   
                            if(send(clientes[1] , message , 8*3 , 0) < 0){
                                puts("Send failed");
                                break;
                            } 


                        }


                    }
                }
                break;

            case 13:
                memcpy(payload_size, client_message + 8, 8); 
                tamano = binary_to_decimal(payload_size, 8);
                printf("tamano OLLA: %d\n", tamano);
                // printf("jug actual: %d\n", jugador_actual);
                for (int i = 0; i < (tamano / 2); i++){
                    memcpy(payload, client_message + 16 + 16 * i, 8);
                    carta = binary_to_decimal(payload, 8);
                    memcpy(payload, client_message + 16 + 8 + 16 * i, 8);
                    pinta = binary_to_decimal(payload, 8);
                    if (jugador_actual == 0){
                        carta_superior = cambiar_carta(mazo, mano_j1, carta, pinta, carta_superior);
                    }
                    else{
                        carta_superior = cambiar_carta(mazo, mano_j2, carta, pinta, carta_superior);                        
                    }
                }
                //RETURN FIVE CARDS
                memcpy(message, "00001010", 8);
                memcpy(message + 8, "00001010", 8);
                if (jugador_actual == 0){
                    for (int i = 0; i < 5; i ++){
                        int_to_bits(payload_size, mano_j1[i][0].numero, 8);
                        memcpy(message + 16 + 16 * i, payload_size , 8);       //numero carta
                        int_to_bits(payload_size, mano_j1[i][0].pinta, 8);
                        memcpy(message + 24 + 16 * i, payload_size, 8);       //pinta carta                                        
                    }
                    sleep(1);
                    if(send(clientes[0] , message , 2 * 8 + 5 * 16 , 0) < 0){
                        puts("Send failed");
                        break;
                    }                     
                }
                else{
                    for (int i = 0; i < 5; i ++){
                        int_to_bits(payload_size, mano_j2[i][0].numero, 8);
                        memcpy(message + 16 + 16 * i, payload_size , 8);       //numero carta
                        int_to_bits(payload_size, mano_j2[i][0].pinta, 8);
                        memcpy(message + 24 + 16 * i, payload_size, 8);       //pinta carta                                        
                    }
                    sleep(1);
                    if(send(clientes[1] , message , 2 * 8 + 5 * 16 , 0) < 0){
                        puts("Send failed");
                        break;
                    } 
                }

                //VER a quien le toca
                printf("comienza : %d\n", comienza );
                printf("jugador_actual : %d\n", jugador_actual );
                if (comienza == jugador_actual){            //le toca hace lo mismo al otro
                    if (jugador_actual == 1){
                        jugador_actual = 0;
                    }
                    else{
                        jugador_actual = 1;
                    }
                    //GET CARDS TO CHANGE
                    memcpy(message, "00001100", 8);
                    memcpy(message + 8, "00000000", 8);
                    memcpy(message + 16, "00000000", 8); 
                    sleep(1);  
                    if(send(clientes[jugador_actual] , message , 8*3 , 0) < 0){
                        puts("Send failed");
                        break;
                    } 
                }
                else{
                    jugador_actual = comienza;
                    //GET BET
                    memcpy(message, "00001110", 8);
                    memcpy(message + 8, "00000100", 8);
                    //se envian los IDS de bets NO vallores
                    // memcpy(message + 16, "00000001", 8);         //NO mando fold
                    memcpy(message + 16, "00000010", 8);   
                    memcpy(message + 24, "00000011", 8);   
                    memcpy(message + 32, "00000100", 8);   
                    memcpy(message + 40, "00000101", 8);
                    sleep(1);
                    if(send(clientes[jugador_actual] , message , 8*6 , 0) < 0){
                        puts("Send failed");
                        break;
                    } 
                }
                break;

            case 15:
                memcpy(payload, client_message + 8, 8);
                tamano = binary_to_decimal(payload, 8);
                memcpy(payload, client_message + 16, tamano * 8);
                if (jugador_actual == 0){
                    bet_j1 = binary_to_decimal(payload, tamano * 8);
                    bet_j1 = obtener_bet(bet_j1);
                    //TODO if bet_j1 == -1 FOLD
                    if (pot_j1 >= bet_j1){
                        //OK BET
                        memcpy(message, "00010001", 8);
                        memcpy(message + 8, "00000000", 8); 
                        memcpy(message + 16, "00000000", 8);
                        sleep(1);  
                        if(send(clientes[jugador_actual] , message , 8*3 , 0) < 0){
                            puts("Send failed");
                            break;
                        } 
                        if (comienza == 0 && verf != 1){
                            jugador_actual = 1;
                            tamano = 2;
                            if (bet_j1 == 200){
                                tamano += 1;
                            }
                            if (bet_j1 == 100){
                                tamano += 2;
                            }
                            if (bet_j1 == 0){
                                tamano += 3;
                            }
                            //se envian los IDS de bets NO vallores
                            //GET BET para el 2
                            int_to_bits(payload_size, tamano, 8);
                            memcpy(message, "00001110", 8);
                            memcpy(message + 8, payload_size, 8);
                            memcpy(message + 16, "00000001", 8);        //FOLD
                            num = obtener_id_bet(bet_j1);  
                            int_to_bits(payload, num, 8);
                            memcpy(message + 24, payload, 8);        //El bet_j1

                            for (int i = 0; i < tamano - 2; i ++){
                                num ++;
                                int_to_bits(payload, num, 8);
                                memcpy(message + 32 + i*8, payload, 8);        //El bet_j1
                            }
                            sleep(1);
                            if(send(clientes[jugador_actual] , message , 4*8 + 8 * (tamano - 2) , 0) < 0){
                                puts("Send failed");
                                break;
                            } 
                        }
                        else{
                            if (bet_j1 == -1 || bet_j1 <= bet_j2 || verf == 1){
                                //GO TO END
                                //END ROUND
                                verf = 0;
                                memcpy(message, "00010010", 8);
                                memcpy(message + 8, "00000000", 8); 
                                memcpy(message + 16, "00000000", 8); 
                                if(send(clientes[0] , message , 8*3 , 0) < 0){
                                    puts("Send failed");
                                    break;
                                } 
                                if(send(clientes[1] , message , 8*3 , 0) < 0){
                                    puts("Send failed");
                                    break;
                                } 
                                //SHOW OPPONENT CARDS
                                memcpy(message, "00010011", 8);
                                memcpy(message + 8, "00001010", 8);
                                for (int i = 0; i < 5; i ++){
                                    int_to_bits(payload_size, mano_j1[i][0].numero, 8);
                                    memcpy(message + 16 + 16 * i, payload_size , 8);       //numero carta
                                    int_to_bits(payload_size, mano_j1[i][0].pinta, 8);
                                    memcpy(message + 24 + 16 * i, payload_size, 8);       //pinta carta                                        
                                }
                                sleep(1);
                                if(send(clientes[1] , message , 2 * 8 + 5 * 16 , 0) < 0){
                                    puts("Send failed");
                                    break;
                                }  

                                for (int i = 0; i < 5; i ++){
                                    int_to_bits(payload_size, mano_j2[i][0].numero, 8);
                                    memcpy(message + 16 + 16 * i, payload_size , 8);       //numero carta
                                    int_to_bits(payload_size, mano_j2[i][0].pinta, 8);
                                    memcpy(message + 24 + 16 * i, payload_size, 8);       //pinta carta                                        
                                }
                                sleep(1);
                                if(send(clientes[0] , message , 2 * 8 + 5 * 16 , 0) < 0){
                                    puts("Send failed");
                                    break;
                                }  
                       //RETURN WINNER LOSE
                                memcpy(message, "00010100", 8);
                                memcpy(message + 8, "00000001", 8);
                                //mensaje ganador
                                memcpy(message + 16, "00000001", 8); 
                                if (bet_j1 == -1){          //HIZO FOLD PERDIO
                                    sleep(1);
                                    if(send(clientes[1] , message , 3 * 8 , 0) < 0){        //el 2 gano
                                        puts("Send failed");
                                        break;
                                    }
                                    sleep(1);
                                    memcpy(message + 16, "00000010", 8);        //el 1 perdio
                                    if(send(clientes[0] , message , 3 * 8 , 0) < 0){
                                        puts("Send failed");
                                        break;
                                    }  
                                    // pot_j2 += bet_j2;
                                } 
                                if (quien_gano(mano_j1, mano_j2) == 2){       //el 1 perdio
                                    sleep(1);
                                    if(send(clientes[1] , message , 3 * 8 , 0) < 0){        //el 2 gano
                                        puts("Send failed");
                                        break;
                                    }
                                    sleep(1);
                                    memcpy(message + 16, "00000010", 8);        //el 1 perdio
                                    if(send(clientes[0] , message , 3 * 8 , 0) < 0){
                                        puts("Send failed");
                                        break;
                                    } 
                                    pot_j1 -= bet_j1;
                                    pot_j2 += bet_j2;  
                                    // pot_j1 = actualizar_pot(pot_j1, bet_j1); 
                                    // pot_j2 = actualizar_pot(pot_j2, bet_j2);                                                
                                }
                                else{
                                    sleep(1);
                                    if(send(clientes[0] , message , 3 * 8 , 0) < 0){        //el 2 gano
                                        puts("Send failed");
                                        break;
                                    }
                                    memcpy(message + 16, "00000010", 8);        //el 1 perdio
                                    sleep(1);
                                    if(send(clientes[1] , message , 3 * 8 , 0) < 0){
                                        puts("Send failed");
                                        break;
                                    }     
                                    pot_j2 -= bet_j2;
                                    pot_j1 += bet_j1; 
                                }
                                //UPDATE POT
                                printf("pot_j1: %d\n", pot_j1);
                                printf("pot_j2: %d\n", pot_j2);
                                memcpy(message, "00010101", 8);
                                tamano = obtener_tamano_bytes(pot_j1);
                                printf("tamano: %d\n", tamano);
                                int_to_bits(payload_size, tamano, 8);
                                memcpy(message + 8, payload_size, 8);
                                int_to_bits(payload, pot_j1, 8 * tamano);
                                memcpy(message + 16, payload, 8 * tamano);   
                                printf("Mess1: %s", message);
                                printf("III\n");
                                sleep(1);
                                
                                if(send(clientes[0] , message , 2 * 8 + 8 * tamano , 0) < 0){
                                    puts("Send failed");
                                    break;
                                } 

                                tamano = obtener_tamano_bytes(pot_j2);
                                                                printf("tamano: %d\n", tamano);

                                int_to_bits(payload_size, tamano, 8);
                                memcpy(message + 8, payload_size, 8);
                                int_to_bits(payload, pot_j2, tamano * 8);
                                memcpy(message + 16, payload, tamano * 8); 
                                printf("Mess2: %s", message);  
                                printf("III\n");
                                sleep(1);
                                
                                if(send(clientes[1] , message , 2 * 8 + tamano * 8 , 0) < 0){
                                    puts("Send failed");
                                    break;
                                }      
                            }
                            else{
                                //MANDARLE AL 2 MAURO
                                tamano = 2;
                                //GET BET
                                //se envian los IDS de bets NO vallores
                                int_to_bits(payload_size, tamano, 8);
                                memcpy(message, "00001110", 8);
                                memcpy(message + 8, payload_size, 8);
                                memcpy(message + 16, "00000001", 8);        //FOLD
                                num = obtener_id_bet(bet_j1);  
                                int_to_bits(payload, num, 8);
                                memcpy(message + 24, payload, 8);        //El bet_j2

                                sleep(1);
                                if(send(clientes[1] , message , 4*8, 0) < 0){
                                    puts("Send failed");
                                    break;
                                } 
                                jugador_actual = 1;
                                verf = 1;
                            }
                        }
                    }
                    else{
                        //ERROR BET
                        memcpy(message, "00010000", 8);
                        memcpy(message + 8, "00000000", 8); 
                        memcpy(message + 16, "00000000", 8);  
                        sleep(1);
                        if(send(clientes[jugador_actual] , message , 8*3 , 0) < 0){
                            puts("Send failed");
                            break;
                        }                       
                    }
                }
                else{
                    bet_j2 = binary_to_decimal(payload, tamano * 8);
                    bet_j2 = obtener_bet(bet_j2);
                    //TODO if bet_j1 == -1 FOLD
                    if (pot_j2 >= bet_j2){
                        //OK BET
                        memcpy(message, "00010001", 8);
                        memcpy(message + 8, "00000000", 8); 
                        memcpy(message + 16, "00000000", 8);  
                        sleep(1);
                        if(send(clientes[jugador_actual] , message , 8*3 , 0) < 0){
                            puts("Send failed");
                            break;
                        } 
                        if (comienza == 1 && verf != 1){
                            jugador_actual = 0;
                            //GET BET para el 2
                            tamano = 2;
                            if (bet_j2 == 200){
                                tamano += 1;
                            }
                            if (bet_j2 == 100){
                                tamano += 2;
                            }
                            if (bet_j2 == 0){
                                tamano += 3;
                            }
                            //se envian los IDS de bets NO vallores
                            int_to_bits(payload_size, tamano, 8);
                            memcpy(message, "00001110", 8);
                            memcpy(message + 8, payload_size, 8);
                            memcpy(message + 16, "00000001", 8);        //FOLD
                            num = obtener_id_bet(bet_j2);  
                            int_to_bits(payload, num, 8);
                            memcpy(message + 24, payload, 8);        //El bet_j2

                            for (int i = 0; i < tamano - 2; i ++){
                                num ++;
                                int_to_bits(payload, num, 8);
                                memcpy(message + 40 + i*8, payload, 8);        //El bet_j2
                            }
                            sleep(1);
                            if(send(clientes[jugador_actual] , message , 4*8 + 8 * (tamano - 2)  , 0) < 0){
                                puts("Send failed");
                                break;
                            } 
                        }
                        else{
                            if (bet_j2 == -1 || bet_j2 <= bet_j1 || verf == 1){
                                //GO TO END
                                //END ROUND
                                verf = 0;
                                memcpy(message, "00010010", 8);
                                memcpy(message + 8, "00000000", 8); 
                                memcpy(message + 16, "00000000", 8); 
                                sleep(1);
                                if(send(clientes[0] , message , 8*3 , 0) < 0){
                                    puts("Send failed");
                                    break;
                                } 
                                if(send(clientes[1] , message , 8*3 , 0) < 0){
                                    puts("Send failed");
                                    break;
                                } 
                                //SHOW OPPONENT CARDS
                                memcpy(message, "00010011", 8);
                                memcpy(message + 8, "00001010", 8);
                                for (int i = 0; i < 5; i ++){
                                    int_to_bits(payload_size, mano_j1[i][0].numero, 8);
                                    memcpy(message + 16 + 16 * i, payload_size , 8);       //numero carta
                                    int_to_bits(payload_size, mano_j1[i][0].pinta, 8);
                                    memcpy(message + 24 + 16 * i, payload_size, 8);       //pinta carta                                        
                                }
                                sleep(1);
                                if(send(clientes[1] , message , 2 * 8 + 5 * 16 , 0) < 0){
                                    puts("Send failed");
                                    break;
                                }  

                                for (int i = 0; i < 5; i ++){
                                    int_to_bits(payload_size, mano_j2[i][0].numero, 8);
                                    memcpy(message + 16 + 16 * i, payload_size , 8);       //numero carta
                                    int_to_bits(payload_size, mano_j2[i][0].pinta, 8);
                                    memcpy(message + 24 + 16 * i, payload_size, 8);       //pinta carta                                        
                                }
                                sleep(1);
                                if(send(clientes[0] , message , 2 * 8 + 5 * 16 , 0) < 0){
                                    puts("Send failed");
                                    break;
                                } 
                                //RETURN WINNER LOSE
                                memcpy(message, "00010100", 8);
                                memcpy(message + 8, "00000001", 8);
                                //mensaje ganador
                                memcpy(message + 16, "00000001", 8);
                                if (bet_j2 == -1){          //HIZO FOLD PERDIO
                                    sleep(1);
                                    if(send(clientes[0] , message , 3 * 8 , 0) < 0){        //el 1 gano
                                        puts("Send failed");
                                        break;
                                    }
                                    memcpy(message + 16, "00000010", 8);        //el 2 perdio
                                    sleep(1);
                                    if(send(clientes[1] , message , 3 * 8 , 0) < 0){
                                        puts("Send failed");
                                        break;
                                    }  
                                    // pot_j1 += bet_j1;
                                } 
                                else if (quien_gano(mano_j1, mano_j2) == 2){       //el 1 perdio
                                    sleep(1);
                                    if(send(clientes[1] , message , 3 * 8 , 0) < 0){        //el 2 gano
                                        puts("Send failed");
                                        break;
                                    }
                                    sleep(1);
                                    memcpy(message + 16, "00000010", 8);        //el 1 perdio
                                    if(send(clientes[0] , message , 3 * 8 , 0) < 0){
                                        puts("Send failed");
                                        break;
                                    } 
                                    pot_j1 -= bet_j1;
                                    pot_j2 += bet_j2;  
                                    // pot_j1 = actualizar_pot(pot_j1, bet_j1); 
                                    // pot_j2 = actualizar_pot(pot_j2, bet_j2);                                                
                                }
                                else{
                                    sleep(1);
                                    if(send(clientes[0] , message , 3 * 8 , 0) < 0){        //el 1 gano
                                        puts("Send failed");
                                        break;
                                    }
                                    memcpy(message + 16, "00000010", 8);        //el 2 perdio
                                    sleep(1);
                                    if(send(clientes[1] , message , 3 * 8 , 0) < 0){
                                        puts("Send failed");
                                        break;
                                    }     
                                    pot_j2 -= bet_j2;
                                    pot_j1 += bet_j1; 
                                }
                                //UPDATE POT
                                printf("pot_j1: %d\n", pot_j1);
                                printf("pot_j2: %d\n", pot_j2);

                                memcpy(message, "00010101", 8);
                                tamano = obtener_tamano_bytes(pot_j1);
                                                                printf("tamano: %d\n", tamano);

                                int_to_bits(payload_size, tamano, 8);
                                memcpy(message + 8, payload_size, 8);
                                int_to_bits(payload, pot_j1, tamano * 8);
                                memcpy(message + 16, payload, tamano * 8);   
                                printf("Mess1: %s", message);
                                printf("III\n");
                                sleep(1);
                                
                                if(send(clientes[0] , message , 2 * 8 + tamano * 8 , 0) < 0){
                                    puts("Send failed");
                                    break;
                                } 

                                tamano = obtener_tamano_bytes(pot_j2);
                                                                printf("tamano: %d\n", tamano);

                                int_to_bits(payload_size, tamano, 8);
                                memcpy(message + 8, payload_size, 8);
                                int_to_bits(payload, pot_j2, tamano * 8);
                                memcpy(message + 16, payload, tamano * 8);  
                                printf("Mess2: %s", message); 
                                printf("III\n");
                                sleep(1);

                                if(send(clientes[1] , message , 2 * 8 + tamano * 8 , 0) < 0){
                                    puts("Send failed");
                                    break;
                                }                                  
                            }
                            else{
                                //MANDARLE AL 2 MAURO
                                tamano = 2;
                                //GET BET
                                //se envian los IDS de bets NO vallores
                                int_to_bits(payload_size, tamano, 8);
                                memcpy(message, "00001110", 8);
                                memcpy(message + 8, payload_size, 8);
                                memcpy(message + 16, "00000001", 8);        //FOLD
                                num = obtener_id_bet(bet_j2);  
                                int_to_bits(payload, num, 8);
                                memcpy(message + 24, payload, 8);        //El bet_j2

                                sleep(1);
                                if(send(clientes[0] , message , 4*8, 0) < 0){
                                    puts("Send failed");
                                    break;
                                } 
                                jugador_actual = 0;
                                verf = 1;
                            }
                        }
                    }
                    else{
                        //ERROR BET
                        memcpy(message, "00010000", 8);
                        memcpy(message + 8, "00000000", 8); 
                        memcpy(message + 16, "00000000", 8);  
                        sleep(1);
                        if(send(clientes[jugador_actual] , message , 8*3 , 0) < 0){
                            puts("Send failed");
                            break;
                        }                       
                    }
                }
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