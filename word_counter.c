#include "wordStruct.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

bool isValidSignedLetter(int code, int * validCodes){
    for(int i = 0; i < 7; i++){
        if(code == validCodes[i] || code == validCodes[i] -32){
            return true;
        }
    }
    return false;
}

int main(){

    clock_t start_time, end_time;
    double cpu_time_used;

    start_time = clock();

    struct LetterList * dictionary = (struct LetterList *)malloc(sizeof(struct LetterList) * 27);

    int validLetterCodes [7] = {-95,-87,-83,-77,-70,-68,-79};

    for(int i = 0; i < 26; i++){
        struct LetterList * newLetterList = &dictionary[i];
        newLetterList->head = 0;
        newLetterList->length = 0;
    }

    char fileText;
    char wordConstructor [45];
    int wordCount = 0;
    int wordAppearences = 0; 
    int letterCount = 0;
    int especialSimbol = 0;//Para tildes, eñes, díerecis
    FILE * fptr = fopen("el_quijote.txt", "r");
    while (true){
        fileText = fgetc(fptr);
        
        if(isalpha(fileText)){
            int casted = (int)fileText;
            if(casted >= 65 && casted <= 90){
                casted += 32;
                fileText = (char)casted;
            }
            wordConstructor[letterCount] = fileText;
            letterCount++; 
        }else{
            int casted = (int)fileText;
            if(casted == -61 && especialSimbol == 0){
                especialSimbol = 1;
            }else if(especialSimbol == 1){
                if(isValidSignedLetter(casted, validLetterCodes)){
                    wordConstructor[letterCount] = (char)(-61);
                    if(casted <= -100){
                        casted += 32;
                    }
                    wordConstructor[letterCount+1] = (char)casted;
                    letterCount += 2;
                    especialSimbol = 0;
                }
            }else{
                especialSimbol = 0;
                if(letterCount > 0){
                    int inicial = (int)wordConstructor[0];//65 - 90 -> mayus // 97 - 122 ->
                    if(inicial == -61){
                        inicial = (int) wordConstructor[1];
                        switch (inicial)
                        {
                        case -95://á
                            inicial = 97;
                            break;
                        case -87://é
                            inicial = 101;
                            break;
                        case -83://í
                            inicial = 105;
                            break;
                        case -77://ó
                            inicial = 111;
                            break;
                        case -70://ú
                            inicial = 117;
                            break;
                        case -68://ü
                            inicial = 117;
                            break;
                        case -79://ñ
                            inicial = 110;
                            break;
                        default:
                            break;
                        }
                    }
                    inicial -= 97;
                    struct LetterList * letterDictionary = &dictionary[inicial];
                    wordCount++;
                    if(letterDictionary->head == NULL){//Si todavía no hay palabras para esta incial
                        struct WordStruct * newWordStruct = createNewWord(wordConstructor, letterCount, 0, 0);
                        
                        letterDictionary->head = newWordStruct;
                        letterDictionary->length = 1;
                        wordAppearences++;
                    }else{
                        struct WordStruct * current = letterDictionary->head;
                        int listPos = 0;
                        int colocada = 0;
                        while(listPos < letterDictionary->length || colocada == 1){
                            if(current->length == letterCount){
                                int isEqual = 1;
                                for(int i = 0; i < letterCount; i++){
                                    if(current->word[i] != wordConstructor[i]){
                                        isEqual = 0;
                                        break;
                                    }
                                }
                                if(isEqual == 1){//Se encontró ya la palabra
                                    current->appearences++;
                                    colocada = 1;
                                    wordAppearences++;

                                    //Algoritmo de preordenamiento
                                    if(listPos > 0){
                                        if(letterDictionary->length == 2){
                                            WordStruct * head = letterDictionary->head;
                                            if(head->appearences < current->appearences){
                                                head->nextWord = 0;
                                                head->previousWord = current;
                                                current->nextWord = head;
                                                current->previousWord = 0;
                                                letterDictionary->head = current;
                                            }
                                        }
                                        if(letterDictionary->length > 2){
                                            int swappingPos = listPos - 1;
                                            WordStruct * checkedNode = current->previousWord;
                                            int needsSwapping = 0;
                                            while(swappingPos >= 0){
                                                if(checkedNode->appearences > current->appearences && needsSwapping == 0){
                                                    break;
                                                }
                                                if((checkedNode->appearences > current->appearences && needsSwapping == 1)||(checkedNode->appearences <= current->appearences && swappingPos == 0)){
                                                    WordStruct * prevCurr = current->previousWord;
                                                    WordStruct * nextCurr = current->nextWord;
                                                    
                                                    prevCurr->nextWord = nextCurr;
                                                    if(nextCurr != 0){
                                                        nextCurr->previousWord = prevCurr;
                                                    }

                                                    if(checkedNode->appearences < current->appearences && swappingPos == 0){
                                                        current->previousWord = 0;
                                                        current->nextWord = checkedNode;
                                                        checkedNode->previousWord = current;
                                                        letterDictionary->head = current;
                                                    }else{
                                                        WordStruct * pushed = checkedNode->nextWord;
                                                        current->nextWord = pushed;
                                                        pushed->previousWord = current;

                                                        checkedNode->nextWord = current;
                                                        current->previousWord = checkedNode;
                                                    }
                                                    break;
                                                }if(checkedNode->appearences < current->appearences){
                                                    needsSwapping = 1;
                                                }
                                                swappingPos--;
                                                checkedNode = checkedNode->previousWord;
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                            if((current->nextWord == 0) || (listPos == letterDictionary->length - 1)){
                                struct WordStruct * newWordStruct = createNewWord(wordConstructor, letterCount, current, 0);
                                current->nextWord = newWordStruct;
                                letterDictionary->head->previousWord = newWordStruct;
                                letterDictionary->length += 1;
                                colocada = 1;
                                wordAppearences++;
                                break;
                            }else{
                                current = current->nextWord; 
                            }
                            listPos++;
                        }
                    }
                    letterCount = 0;
                }
            }
        }
        if(fileText == EOF){
            break;
        }
    }

    end_time = clock();

    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("CPU time used: %f seconds\n", cpu_time_used);

    for(int i = 0; i < 26; i++){
        //printLetterList(&dictionary[i]);
        freeLetterList(&dictionary[i]);
    }
    
    printf("Palabras: %d\n", wordCount);
    printf("Accounted: %d\n", wordAppearences);

    fclose(fptr);

    free(dictionary);
}


