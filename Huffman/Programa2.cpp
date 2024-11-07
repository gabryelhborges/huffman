#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include "tadTrabHuffman.h"


void construirRegistros(Registro **reg){
    Registro aux;
    FILE *ptrArq = fopen("registro.dat", "rb");
    do{
        fread(&aux, sizeof(Registro), 1, ptrArq);
        inserirOrdenado(&*reg, aux);
    }while(!feof(ptrArq));

}

void construirArvore(Tree **r, Registro *reg){
    Tree *aux, *aux2;
    int i;
    while(reg!=NULL){
        aux= *r;
        for(i= 0; i < strlen(reg->codHuffman); i++){
            if(*r==NULL){
                *r= criaNo(-1, -1);
                aux= *r;
            }
            if(i == (strlen(reg->codHuffman) - 1)){
                aux2= criaNo(reg->simbolo, 0);//enviando para todas as frequencias, pois nao precisamos delas
                if(reg->codHuffman[i] == '0'){
                    aux->esq= aux2;
                }
                else{
                    aux->dir= aux2;
                }
            }
            if(reg->codHuffman[i]=='0'){
                if(aux->esq == NULL){
                    aux2= criaNo(-1, -1);
                    aux->esq= aux2;
                }
                aux= aux->esq;
            }
            else{
                if(aux->dir == NULL){
                    aux2= criaNo(-1, -1);
                    aux->dir= aux2;
                }
                aux= aux->dir;
            }
        }
        reg= reg->prox;
    }
}

void buscarSimbolo(Registro *reg, int simbolo, char *palavra){
    while(reg->simbolo != simbolo){
        reg= reg->prox;
    }
    strcpy(palavra, reg->palavra);
}

void decodificarArq(Tree *r, Registro *reg, char *frase){
    FILE * ptrArq = fopen("fraseCodificada.txt", "r");
    Tree *aux= r;
    char letra, palavra[TC];
    while(!feof(ptrArq)){
        letra = fgetc(ptrArq);
        if(letra == '0'){
            aux= aux->esq;
            if(aux->simbolo != -1){
                buscarSimbolo(reg, aux->simbolo, palavra);
                //printf("%s", palavra);
                strcat(frase, palavra);
                aux= r;
            }
        }
        else{
            aux= aux->dir;
            if(aux->simbolo != -1){
                buscarSimbolo(reg, aux->simbolo, palavra);
                //printf("%s", palavra);
                strcat(frase, palavra);
                aux= r;
            }
        }
    }
    fclose(ptrArq);
}

int main(void){
    Tree *r= NULL;
    Registro *regs= NULL;
    char fraseDecodificada[TCG];
    strcpy(fraseDecodificada, "");
    construirRegistros(&regs);
    //exibirRegistro(regs);
    construirArvore(&r, regs);
    //exibeH(r);
    decodificarArq(r, regs, fraseDecodificada);
    printf("\n\n\nFrase decodificada: \n\n\n");
    printf("\n\n\n%s\n\n\n", fraseDecodificada);
}
