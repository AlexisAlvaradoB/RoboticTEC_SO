#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Estructura para las palabras individuales
 * 
 * @param word Palabra que guarda
 * @param length Longitud de la palabra. Incluye como parte del tamaño los carácteres especiales.abort
 * @param appearences Cuántas veces ha aparecido la palabra
 * @param nextWord Siguiente palabra en la lista. 0 si ninguna
 * @param previousWord Anterior palabra en la lista. 0 si ninguna
 */
typedef struct WordStruct
{
    char * word;
    int length;
    int appearences;
    struct WordStruct * nextWord;
    struct WordStruct * previousWord;
} WordStruct;


/**
 * @brief Lista doblemente enlazada para las palabras con una misma inicial
 * 
 * @param length longitud de la lista
 * @param head cabeza de la lista enlazada
 */
typedef struct LetterList
{
    int length;
    struct WordStruct * head;
} LetterList;


/**
 * @brief Crea un nuevo nodo de palabra. Reserva el espacio en el heap
 * 
 * @param wordConstructor puntero con el arreglo al buffer para palabras
 * @param length longitud de la palabra
 * @param prev puntero al nodo anterior. 0 si ninguno
 * @param next puntero al nodo siguiente. 0 si ninguno
 * 
 * @return estructura con el nodo de palabra nuevo
 */
WordStruct * createNewWord(char * wordConstructor, int length, struct WordStruct * prev, struct WordStruct * next){
    struct WordStruct * newWordStruct = (struct WordStruct *)malloc(sizeof(struct WordStruct));
    char * word = (char *)malloc(sizeof(char) * length);
    for(int i = 0; i < length; i++){
        word[i] = wordConstructor[i];
    }
    newWordStruct->word = word;
    newWordStruct->appearences = 1;
    newWordStruct->length = length;
    newWordStruct->nextWord = next;
    newWordStruct->previousWord = prev;
    return newWordStruct;
}

/**
 * @brief Printea una palabra (cadena char) en sí
 * 
 * @param word palabra a imprimir
 * @param length longitud de la palabra
 */
void printWord(char * word, int length){
    for(int i = 0; i < length; i++){
        printf("%c", word[i]);
    }
}

/**
 * @brief Imprime la información de una estructura wordStruct
 * 
 * @param wordStruct nodo a imprimir
 */
void printWordStruct(struct WordStruct * wordStruct){
    if(wordStruct->appearences > 1){
        printf("Palabra: ");
        printWord(wordStruct->word, wordStruct->length);
        printf(" Apariciones: %d\n", wordStruct->appearences);
    }
    
}

/**
 * @brief Libera la memoria resevada para el nodo
 * 
 * @param wordStruct estructura a liberar
 */
void freeWordStruct(struct WordStruct * wordStruct){
    free(wordStruct->word);
}

/**
 * @brief imprime una lista entera
 * 
 * @param letterList lista a imprimir
 */
void printLetterList(LetterList * letterList){
    printf("+++++++++++++++++++\n");
    if(letterList->length > 0){
        struct WordStruct * word = letterList->head;
        int i = 0;
        while(i < letterList->length){
            printWordStruct(word);
            word = word->nextWord;
            i++;
        }
    }else{
        printf("Lista vacía\n");
    }
    printf("------------------\n");
}

/**
 * @brief libera una lista entera
 * 
 * @param letterList lista a liberar de memoria
 */
void freeLetterList(LetterList * letterList){
    if(letterList->length > 0){
        struct WordStruct * word = letterList->head;
        int i = 0;
        while(i < letterList->length){
            freeWordStruct(word);
            word = word->nextWord;
            i++;
        }
    }
}

int compareStrings(char * w1, int lenW1, char * w2, int lenW2){
    if(lenW1 == lenW2){
        for(int i = 0; i < lenW1; i++){
            if(w1[i] != w2[i]){
                return 0;
            }
        }
        return 1;
    }else{
        return 0;
    }
}


struct WordStruct * findStruct(LetterList * list, char * word, int len){
    if(list->length > 0){
        struct WordStruct * curr = list->head;
        int pos = 0;
        while(pos < list->length){
            if(compareStrings(word, len, curr->word, curr->length) == 1){
                return curr;
            }else{
                curr = curr->nextWord;
                if(curr == NULL){
                    return NULL;
                }
            }
            pos++;
        }
        return NULL;
    }else{
        return NULL;
    }
    return NULL;
}


