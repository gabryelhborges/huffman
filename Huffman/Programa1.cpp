#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include "tadTrabHuffman.h"

void limpaPalavra(char *palavra)
{
	char aux[TC];
	int i = 0, j = 0;
	strcpy(aux, "");
	while (i < strlen(palavra))
	{
		if (palavra[i] == ',' || palavra[i] == '.' || palavra[i] == '?' || palavra[i] == '/' || palavra[i] == '!' || palavra[i] == '#')
		{
			i++;
		}
		else
		{
			aux[j++] = palavra[i];
		}
		i++;
	}
	aux[j] = '\0';
	strcpy(palavra, aux);
}

void leFrase(Registro **R, char *frase)
{
	int i = 0, posLetra;
	char palavra[TC], letra;

	while (i < strlen(frase))
	{
		posLetra = 0;
		strcpy(palavra, "");
		letra = frase[i];
		while (i < strlen(frase) && letra != ' ')
		{
			palavra[posLetra++] = letra;
			letra = frase[++i];
		}
		palavra[posLetra] = '\0';
		limpaPalavra(palavra);
		insereRegistro(&*R, palavra);
		if (letra == ' ')
			insereRegistro(&*R, " ");
		i++;
	}
}

void gerarFloresta(Floresta **F, Registro *regs)
{
	int i = 0;
	Tree *no;
	while (regs != NULL)
	{
		no = criaNo(i, regs->frequencia);
		insereFloresta(&(*F), no);
		regs->simbolo = i++;
		regs = regs->prox;
	}
}

void gerarArvore(Tree **t, Floresta *f){
	Tree *aux, *aux2, *juntar;
	while(f->prox != NULL){
		aux= excluirFloresta(&f);
		aux2= excluirFloresta(&f);
		juntar = criaNo(-1, aux->frequencia + aux2->frequencia);
		juntar->esq= aux;
		juntar->dir= aux2;
		insereFloresta(&f, juntar);
	}
	*t= f->no;
}

void atribuirCodigo(Registro *R, int simbolo, char cod[]){
	while(R->simbolo != simbolo){
		R= R->prox;
	}
	strcpy(R->codHuffman, cod);
}

void criaCodigo(Registro **R, Tree *raiz, char cod[]){
	char codEsq[TC], codDir[TC];
	if(raiz!=NULL){
		if(raiz->esq == NULL && raiz->dir == NULL){
			atribuirCodigo(*R, raiz->simbolo, cod);
		}
		else{
			strcpy(codEsq, cod);
			criaCodigo(&(*R), raiz->esq, strcat(codEsq, "0"));
			strcpy(codDir, cod);
			criaCodigo(&(*R), raiz->dir, strcat(codDir, "1"));
		}
	}
}

void criaArqBin(Registro *R){
	FILE *ptrArq = fopen("registro.dat", "wb");
	while(R != NULL){
		fwrite(R, 1, sizeof(Registro), ptrArq);
		R= R->prox;
	}
	fclose(ptrArq);
}

void inserirCodificado(Registro *R, char palavra[]){
	FILE *ptrArq = fopen("fraseCodificada.txt", "r+");
	if(ptrArq == NULL){
		ptrArq= fopen("fraseCodificada.txt", "w");
	}
	fseek(ptrArq, 0, 2);//Ou SEEK_END
	while(R != NULL && stricmp(R->palavra, palavra)!= 0){
		R= R->prox;
	}
	fputs(R->codHuffman, ptrArq);
	fclose(ptrArq);
}

void criaArqTxt(Registro *reg, char frase[]){
	char letra, palavra[TC];
	int i=0, posLetra;
	while(i < strlen(frase)){
		posLetra= 0;
		strcpy(palavra, "");
		letra = frase[i];
		while(i < strlen(frase) && letra != ' '){
			palavra[posLetra++]= letra;
			letra= frase[++i];
		}
		palavra[posLetra]= '\0';
		limpaPalavra(palavra);
		inserirCodificado(reg, palavra);
		if(letra == ' '){
			inserirCodificado(reg, " ");
		}
		i++;
	}
}

int main(void)
{
	Tree *tree = NULL;
	Floresta *floresta = NULL;
	Registro *regs = NULL;
	char frase[TCG];//TCG(Tamanho Char Grande) se encontra no tad, pode ser necessario aumentar seu tamanho caso a frase seja muito longa
	remove("fraseCodificada.txt");
	remove("registro.dat");
	strcpy(frase, "No final de tarde, tarde que se despede com cores quentes no horizonte, horizonte que abraca a cidade. Cidade agitada, agitada como um formigueiro, formigueiro humano em constante movimento. Movimento de carros, carros que se deslocam pelas ruas estreitas e movimentadas. Movimentadas como a vida que pulsa em cada esquina, esquina onde historias se cruzam. Cruzam-se olhares, olhares que se encontram e se perdem na multidao. Multidao de sonhos, sonhos que flutuam como baloes coloridos no ceu. Ceu vasto, vasto como o oceano que se estende ate o horizonte distante. Distante, porem presente em cada respirar, respirar profundo que preenche os pulmoes de esperanca. Esperanca que guia os passos, passos que deixam pegadas na areia da praia tranquila. Tranquila como a noite que se aproxima, aproxima-se com sua calmaria e estrelas cintilantes. Cintilantes como diamantes no veu escuro da noite, noite que sussurra segredos aos coracoes atentos. Atentos ao ciclo eterno da vida, vida que tece sua teia com fios de experiencias e emocoes, emocoes que ecoam como melodias em um concerto infinito.");
	strlwr(frase); // deixando a string inteira em letras minúsculas
	leFrase(&regs, frase);
	gerarFloresta(&floresta, regs);
	//exibeFloresta(floresta);
	gerarArvore(&tree, floresta);
	
	//exibeH(tree);
	criaCodigo(&regs, tree, "");
	//exibirRegistro(regs);
	//exibeH(tree);

	criaArqTxt(regs, frase);
	criaArqBin(regs);
	printf("Arquivos gerados com sucesso!");
	return 0;
}
