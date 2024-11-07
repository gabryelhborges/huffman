#define TC 50
#define TCG 1100

/*
	ALUNO: Gabryel Henrique Borges
	RA: 102215626
*/

struct bits{
	//menos significativo
	unsigned char b7:1;
	unsigned char b6:1;
	unsigned char b5:1;
	unsigned char b4:1;
	unsigned char b3:1;
	unsigned char b2:1;
	unsigned char b1:1;
	unsigned char b0:1;
	//mais significativo
};
typedef struct bits Bits;

union byte1{
	Bits b;
	unsigned char info;
};
typedef union byte1 Byte;

struct tree
{
    int simbolo, frequencia;
    struct tree *esq, *dir;
};
typedef struct tree Tree;

struct floresta
{
    struct floresta *prox;
    Tree *no;
};
typedef struct floresta Floresta;

struct registro
{
    char palavra[TC], codHuffman[TC];
    int simbolo, frequencia;
    struct registro *prox;
};
typedef struct registro Registro;


Byte byteZero(){
	Byte B;

	B.b.b0= 0;
	B.b.b1= 0;
	B.b.b2= 0;
	B.b.b3= 0;
	B.b.b4= 0;
	B.b.b5= 0;
	B.b.b6= 0;
	B.b.b7= 0;

	return B;
}

void exibeByte(Byte B){
	printf("%d", B.b.b0);
	printf("%d", B.b.b1);
	printf("%d", B.b.b2);
	printf("%d", B.b.b3);
	printf("%d", B.b.b4);
	printf("%d", B.b.b5);
	printf("%d", B.b.b6);
	printf("%d", B.b.b7);

	printf("\n\n");
}

Floresta *criaArvore(Tree *no)
{
    Floresta *f = (Floresta *)malloc(sizeof(Floresta));
    f->no = no;
    f->prox = NULL;
    return f;
}

void insereFloresta(Floresta **F, Tree *no)
{
	Floresta *aux = criaArvore(no), *atual = *F, *ant = NULL;
	while (atual != NULL && no->frequencia > atual->no->frequencia)
	{
		ant = atual;
		atual = atual->prox;
	}
	if (ant == NULL)
	{
		aux->prox = *F;
		*F = aux;
	}
	else
	{
		ant->prox = aux;
		aux->prox = atual;
	}
}

void exibeFloresta(Floresta *F)
{
	printf("EXIBE FLORESTA: \n\n");
	while (F != NULL)
	{
		printf("%d \t%d\n", F->no->simbolo, F->no->frequencia);
		F = F->prox;
	}
}

Tree *criaNo(int simbolo, int freq){
	Tree *no = (Tree *)malloc(sizeof(Tree));
	no->simbolo = simbolo;
	no->frequencia = freq;
	no->dir = no->esq = NULL;
	return no;
}

Tree *excluirFloresta(Floresta **F){
	Tree *aux = (*F)->no;
	Floresta *auxRemover = *F;
	*F = (*F)->prox;
	free(auxRemover);
	return aux;
}

void inserirArvore(Tree **raiz, int simbolo, int freq)
{
	if (*raiz == NULL){
		*raiz = criaNo(simbolo, freq);
	}
	else{
		if (freq < (*raiz)->frequencia){
			inserirArvore(&(*raiz)->esq, simbolo, freq);
		}
		else{
			inserirArvore(&(*raiz)->dir, simbolo, freq);
		}
	}
}

void exibeH(Tree *raiz){
	static int n = -1;
	
	if(raiz!=NULL){
		n++;
		exibeH(raiz->dir);
		for(int i=0; i < 5*n; i++){
			printf(" ");
		}
		printf("|%d,%d|\n", raiz->simbolo, raiz->frequencia);
		exibeH(raiz->esq);
		n--;
	}
}

Registro *criaRegistro(char palavra[]){
	Registro *reg = (Registro *)malloc(sizeof(Registro));
	strcpy(reg->palavra, palavra);
	reg->frequencia = 1;
	reg->prox = NULL;
	return reg;
}

void insereRegistro(Registro **R, char palavra[])
{
	Registro *atual, *ant, *reg;
	atual = *R;
	ant = NULL;
	if (atual == NULL)
	{
		reg = criaRegistro(palavra);
		*R = reg;
	}
	else
	{
		while (atual != NULL && stricmp(atual->palavra, palavra) != 0)
		{
			ant = atual;
			atual = atual->prox;
		}
		if (atual == NULL)
		{
			reg = criaRegistro(palavra);
			ant->prox = reg;
		}
        else{
            atual->frequencia++;
        }
	}
}


//Decodificar
Registro *criaRegistroDec(Registro info){
	Registro *r = (Registro*)malloc(sizeof(Registro));
	strcpy(r->palavra, info.palavra);
	//r->frequencia=info.frequencia;
	r->simbolo=info.simbolo;
	strcpy(r->codHuffman,info.codHuffman);
	r->prox=NULL;
	return r;
}

void inserirOrdenado(Registro **r, Registro info){
    Registro *reg= criaRegistroDec(info);
    Registro *atual= *r, *ant= NULL;
    while(atual !=NULL && strlen(reg->codHuffman) > strlen(atual->codHuffman)){
        ant = atual;
        atual= atual->prox;
    }
    if(ant == NULL){
        reg->prox= *r;
        *r= reg;
    }
    else{
        ant->prox= reg;
        reg->prox= atual;
    }
}

void exibirRegistro(Registro *r){
	printf("EXIBIR REGISTROS\n\n");
    while(r != NULL){
        printf("\n%d %s %d %s\n", r->simbolo, r->palavra, r->frequencia, r->codHuffman);
        r= r->prox;
    }
}
