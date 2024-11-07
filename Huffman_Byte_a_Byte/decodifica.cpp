#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include "tadHuffman.h"

/*
	ALUNO: Gabryel Henrique Borges
	RA: 102215626
*/

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
    FILE * ptrArq = fopen("fraseCodificada.dat", "rb");//ponteiro comeca do inicio
    Tree *aux= r;
    Byte B;
    char letra, palavra[TC], stringByte[9], achou= 0;
    int i;
    
    //Entra lido, sai lido
    fread(&B.info, sizeof(char), 1, ptrArq);
    while(!feof(ptrArq)){
        
        sprintf(stringByte, "%d%d%d%d%d%d%d%d", B.b.b0, B.b.b1, B.b.b2, B.b.b3, B.b.b4, B.b.b5, B.b.b6, B.b.b7);
        i= 0;
        achou= 0;
        while(i < strlen(stringByte) && !achou){//Flag achou para que nao sejam exibidos espacos entre as palavras da frase
        	if(stringByte[i] == '0'){
	            aux= aux->esq;
	            if(aux->simbolo != -1){
	                buscarSimbolo(reg, aux->simbolo, palavra);
	                //printf("%s", palavra);
	                strcat(frase, palavra);
	                aux= r;
	                achou= 1;
	            }
	        }
	        else{
	            aux= aux->dir;
	            if(aux->simbolo != -1){
	                buscarSimbolo(reg, aux->simbolo, palavra);
	                //printf("%s", palavra);
	                strcat(frase, palavra);
	                aux= r;
	                achou= 1;
	            }
	        }
	        i++;
        }
        fread(&B.info, sizeof(char), 1, ptrArq);
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
    exibeH(r);
    decodificarArq(r, regs, fraseDecodificada);
    printf("\n\n\nFrase decodificada: \n\n\n");
    printf("\n\n\n%s\n\n\n", fraseDecodificada);
}
