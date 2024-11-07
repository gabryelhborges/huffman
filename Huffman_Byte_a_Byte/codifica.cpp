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
	Byte B;
	int i, tamPalavra, resto;
	char codHuff[TC];
	FILE *ptrArq = fopen("fraseCodificada.dat", "rb+");
	if(ptrArq == NULL){
		ptrArq= fopen("fraseCodificada.dat", "wb");
	}
	fseek(ptrArq, 0, 2);//Ou SEEK_END
	while(R != NULL && stricmp(R->palavra, palavra)!= 0){
		R= R->prox;
	}
	if(R != NULL){
		strcpy(codHuff, R->codHuffman);
		//
		while(strlen(codHuff) % 8 != 0){
			strcat(codHuff, "0");//"0" cod do espaco em huffman
		}
		i= 0;
		while(i < strlen(codHuff)){
			B.b.b0= codHuff[i++];
			B.b.b1= codHuff[i++];
			B.b.b2= codHuff[i++];
			B.b.b3= codHuff[i++];
			B.b.b4= codHuff[i++];
			B.b.b5= codHuff[i++];
			B.b.b6= codHuff[i++];
			B.b.b7= codHuff[i++];
		
			fwrite(&B.info, sizeof(char), 1, ptrArq);
		}
	}
	fclose(ptrArq);
}

void criaArqCodificado(Registro *reg, char frase[]){
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
	char frase[TCG];
	remove("fraseCodificada.dat");
	remove("registro.dat");
	//strcpy(frase, "No final de tarde, tarde que se despede com cores quentes no horizonte, horizonte que abraca a cidade. Cidade agitada, agitada como um formigueiro, formigueiro humano em constante movimento. Movimento de carros, carros que se deslocam pelas ruas estreitas e movimentadas. Movimentadas como a vida que pulsa em cada esquina, esquina onde historias se cruzam. Cruzam-se olhares, olhares que se encontram e se perdem na multidao. Multidao de sonhos, sonhos que flutuam como baloes coloridos no ceu. Ceu vasto, vasto como o oceano que se estende ate o horizonte distante. Distante, porem presente em cada respirar, respirar profundo que preenche os pulmoes de esperanca. Esperanca que guia os passos, passos que deixam pegadas na areia da praia tranquila. Tranquila como a noite que se aproxima, aproxima-se com sua calmaria e estrelas cintilantes. Cintilantes como diamantes no veu escuro da noite, noite que sussurra segredos aos coracoes atentos. Atentos ao ciclo eterno da vida, vida que tece sua teia com fios de experiencias e emocoes, emocoes que ecoam como melodias em um concerto infinito.");
	//strcpy(frase, "O cachorro correu atras do seu proprio rabo, cachorro esse girando em circulos cada vez mais apertados, ate que o cahorro finalmente alcancou e agarrou firmemente o seu proprio rabo com os dentes, entao o cachorro continua dando voltas e mais voltas sem parar, sem pensar em nada alem de seu rabo.");
	strcpy(frase, "O cachorro era um cachorro muito muito muito raivoso, seu dono era raivoso.");//Frase pra teste
	//strcpy(frase, "Amar e sonhar, sonhar e viver, viver e curtir, curtir e amar. Cada um tera uma escolha, cada um fara a escolha, cada um escolhe a sua escolha. Levaremos um tempo para crescer, levaremos um tempo para amadurecer, levaremos um tempo para entender, levaremos um tempo para envelhecer, levaremos um tempo para morrer.");
	
	strlwr(frase);
	
	leFrase(&regs, frase);
	
	gerarFloresta(&floresta, regs);
	
	exibeFloresta(floresta);
	
	gerarArvore(&tree, floresta);
	
	//exibeH(tree);
	
	criaCodigo(&regs, tree, "");
	
	printf("\n\n");
	exibirRegistro(regs);
	
	printf("\n\nEXIBE ARVORE: \n");
	exibeH(tree);
	
	criaArqCodificado(regs, frase);
	
	criaArqBin(regs);
	
	printf("\n\n\n\nOs arquivos foram gerados!\n");
	return 0;
}
