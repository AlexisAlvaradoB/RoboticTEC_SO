#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include <ctype.h>
#include "./wordStruct.c"

#define NODOS 4

#define PUERTO_SERVIDOR 8080
#define PUERTO_A 8081
#define PUERTO_B 8082
#define PUERTO_C 8083
#define PUERTO_D 8084

typedef struct NodoTexto
{
    int puerto;
    long len;
    char * text;
    LetterList * diccionario;
} NodoTexto;


bool isValidSignedLetter(int code, int * validCodes){
    for(int i = 0; i < 7; i++){
        if(code == validCodes[i] || code == validCodes[i] -32){
            return true;
        }
    }
    return false;
}



int validLetterCodes [7] = {-95,-87,-83,-77,-70,-68,-79};

int fsize(FILE * file){
    long prev = ftell(file);
    fseek(file, 0L, SEEK_END);  
    int sz = ftell(file);
    fseek(file, prev, SEEK_SET);
    return sz;
}

long * divideFile(long fileSize, FILE * fp){
    long * limits = (long *)malloc(sizeof(long)*NODOS);
    long division = fileSize/NODOS;
    char fileChar;
    fseek(fp, division, SEEK_SET);
    bool isPotentialAccent = false;
    int divisionNumber = 0;
    while (divisionNumber < NODOS && division < fileSize)
    {   
        fileChar = fgetc(fp);
        
        if(fileChar == EOF){
            limits[divisionNumber] = division;
            break;
        }
        if(!isalpha(fileChar)){
            int code = (int)fileChar;
            if(code == -61 && isPotentialAccent == false){
                isPotentialAccent = true;
            }else if(isValidSignedLetter(code, validLetterCodes) && isPotentialAccent == true){
                isPotentialAccent = false;
                division += 2;
            }else{
                limits[divisionNumber] = division;
                isPotentialAccent = false;
                divisionNumber++;
                if(divisionNumber >= NODOS){
                    break;
                }
                division += (fileSize - division)/(NODOS - divisionNumber);
                
                fseek(fp, division, SEEK_SET);
            }
        }else{
            division++;
        }
    }

    limits[NODOS-1] = fileSize;
    fseek(fp, 0, SEEK_SET);

    return limits;
}

int conectar_nodo(int puerto){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creando socket");
        return -1;
    }
    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(puerto);
    direccion.sin_addr.s_addr = inet_addr("127.0.0.1");

    int intentos = 0;
    while (connect(sock, (struct sockaddr*)&direccion, sizeof(direccion)) < 0 && intentos < 10) {
        printf("Intentando conectar con nodo en puerto %d... (intento %d)\n", puerto, intentos + 1);
        sleep(1);
        intentos++;
    }
    
    if (intentos >= 10) {
        printf("No se pudo conectar con nodo en puerto %d\n", puerto);
        close(sock);
        return -1;
    }
    
    printf("Conectado con nodo en puerto %d\n", puerto);
    return sock;
}

void * processText(void * arg){
    NodoTexto * nodo = (NodoTexto *)arg;

    int sock = conectar_nodo(nodo->puerto);
    if(sock < 0){
        return NULL;
    }

    LetterList * dictionary = (LetterList *)malloc(sizeof(LetterList) * 26);

    int elementsInList = 0;

    send(sock, &nodo->len, sizeof(long), 0);

    send(sock, nodo->text, nodo->len, 0);

    for(int i = 0; i < 26; i++){
        recv(sock, &elementsInList, sizeof(int), 0); //Recibo la longitud de cada lista
        if(elementsInList > 0){
            WordStruct * previous;
            for(int j = 0; j < elementsInList; j++){
                int wordLength = 0;
                int appears = 0;
                recv(sock, &wordLength, sizeof(int), 0);
                char * word = (char *)malloc(sizeof(char)*wordLength); 
                recv(sock, word, wordLength, 0);
                recv(sock, &appears, sizeof(int), 0);
                if(j == 0){
                    dictionary[i].head = createNewWord(word, wordLength, 0, 0);
                    dictionary[i].head->appearences = appears;
                    previous = dictionary[i].head;
                }else{
                    WordStruct * wordStr = createNewWord(word, wordLength, previous, 0);
                    wordStr->appearences = appears;
                    previous->nextWord = wordStr;
                }
            }
        }
    }
    
    for(int i = 0; i < 26; i++){
        freeLetterList(&dictionary[i]);
    }


    printf("Cliente del puerto %d completó su proceso\n", nodo->puerto);

    close(sock);

}

char * getFullTextToProcess(FILE * file, long start, long end){
    char * message = (char *)malloc(sizeof(char) * (end - start));
    fseek(file, start, SEEK_SET);
    long character = start;
    while (character < end)
    {
        message[character - start] = fgetc(file);
        character++;
    }
    return message;
}

int main(int argc, char**argv){
    
    FILE * fp = fopen("../el_quijote.txt", "r");
    if(fp != NULL){
        long * limits = divideFile(fsize(fp), fp);

        int puertos[] = {PUERTO_A, PUERTO_B, PUERTO_C, PUERTO_D};
        NodoTexto * listaNodos = (NodoTexto *)malloc(sizeof(NodoTexto)*NODOS);
        for(int i = 0; i < NODOS; i++){
            listaNodos[i].puerto = puertos[i];
            if(i == 0){
                listaNodos[i].len = limits[0];
                listaNodos[i].text = getFullTextToProcess(fp, 0, limits[0]);
            }else{
                listaNodos[i].len = limits[i] - limits[i-1];
                listaNodos[i].text = getFullTextToProcess(fp, limits[i-1], limits[i]);
            } 
        }

        pthread_t threads[NODOS];
        
        for(int i = 0; i < NODOS; i++){
            if (pthread_create(&threads[i], NULL, processText, &listaNodos[i]) != 0) {
                perror("Error creando thread");
                return 1;
            }
        }

        for (int i = 0; i < NODOS; i++) {
            pthread_join(threads[i], NULL);
        }



        //Para cuando finaliza, libera las cosas
        for(int i = 0; i < NODOS; i++){
            free(listaNodos[i].text);
        }
        free(listaNodos);
        free(limits);
        fclose(fp);
    }else{
        printf("No se pudo abrir el archivo.\n");
    }

    return 0;
}