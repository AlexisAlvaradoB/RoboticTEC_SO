#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include "./node_processing.c"

int main(int argc, char * argv[]){
    if (argc != 2) {
        printf("Uso: %s <puerto>\n", argv[0]);
        return 1;
    }

    int puerto = atoi(argv[1]);

    printf("Nodo iniciado en puerto %d\n", puerto);
        
    // Crear socket servidor
    int servidor_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor_sock < 0) {
        perror("Error creando socket");
        return 1;
    }
    
    // Configurar reutilización de puerto
    int opt = 1;
    setsockopt(servidor_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(puerto);
    
    if (bind(servidor_sock, (struct sockaddr*)&direccion, sizeof(direccion)) < 0) {
        perror("Error en bind");
        close(servidor_sock);
        return 1;
    }
    
    if (listen(servidor_sock, 1) < 0) {
        perror("Error en listen");
        close(servidor_sock);
        return 1;
    }
    
    printf("Nodo escuchando en puerto %d...\n", puerto);
    
    // Aceptar conexión del servidor
    int cliente_sock = accept(servidor_sock, NULL, NULL);
    if (cliente_sock < 0) {
        perror("Error aceptando conexión");
        close(servidor_sock);
        return 1;
    }
    
    printf("Conexión aceptada del servidor principal\n");
    
    // Recibir tamaño del segmento
    int len_texto = 0;
    recv(cliente_sock, &len_texto, sizeof(long), 0);
    
    // Recibir segmento
    char* texto = (char *)malloc(sizeof(char) * len_texto);
    recv(cliente_sock, texto, len_texto, 0);
    
    printf("Recibido texto de %d caracteres\n", len_texto);
    
    // Procesar texto
    
    LetterList * dictionary = processText(texto);
    
    printf("Procesamiento completado\n");
    
    for(int i = 0; i < 26; i++){
        send(cliente_sock, &dictionary[i].length, sizeof(int), 0);
        int length = dictionary[i].length;
        if(length > 0){
            WordStruct * curr = dictionary[i].head;
            for(int j = 0; j < length; j++){   
                send(cliente_sock, &curr->length, sizeof(int), 0);
                send(cliente_sock, curr->word, sizeof(char)*curr->length, 0);
                send(cliente_sock, &curr->appearences, sizeof(int), 0);
                curr = curr->nextWord;
            }
        }
    }

    printf("Resultados enviados al servidor principal\n");

    for(int i = 0; i < 26; i++){
        printLetterList(&dictionary[i]);
        freeLetterList(&dictionary[i]);
    }

    close(cliente_sock);
    close(servidor_sock);
    free(texto);
    free(dictionary);

    return 0;
}