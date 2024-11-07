#define TC 50
#define TCG 1100
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
	r->frequencia=info.frequencia;
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
    while(r != NULL){
        printf("\n%d %s %d %s\n", r->simbolo, r->palavra, r->frequencia, r->codHuffman);
        r= r->prox;
    }
}