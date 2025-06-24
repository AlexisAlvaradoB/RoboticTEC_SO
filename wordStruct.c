#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct WordStruct
{
    char * word;
    int length;
    int appearences;
    struct WordStruct * nextWord;
    struct WordStruct * previousWord;
} WordStruct;

typedef struct LetterList
{
    int length;
    struct WordStruct * head;
} LetterList;

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

void printWord(char * word, int length){
    for(int i = 0; i < length; i++){
        printf("%c", word[i]);
    }
}

void printWordStruct(struct WordStruct * wordStruct){
    if(wordStruct->appearences > 1){
        printf("Palabra: ");
        printWord(wordStruct->word, wordStruct->length);
        printf(" Apariciones: %d\n", wordStruct->appearences);
    }
    
}

void freeWordStruct(struct WordStruct * wordStruct){
    int len = wordStruct->length;
    //free(wordStruct->appearences);
}

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
