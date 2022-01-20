#include "hash.h"

// Funcao hash. Retorna o indice de uma chave na tabela. Nao alterar!
int hash(char chave[STR_SIZE], int tamanho, int tentativa) {
    int h = chave[0];
    for (int i = 1; chave[i] != '\0'; i++)
        h = (h * 251 * chave[i]) % tamanho;
    return (h + tentativa) % tamanho;
}

tipoHash criar(modoHash m, int t) {
    tipoHash h;
    h.tamanho = t;
    switch (h.modo = m) {
    case semColisao:
        h.tabela.aberto = (hashAberto*) malloc(sizeof(hashAberto) * t);
        for (int i = 0; i < t; i++) 
            h.tabela.aberto[i].chave[0] = h.tabela.aberto[i].valor[0] = '\0';
        break;

    case encadeamento:
        h.tabela.encadeada = (hashEncadeada*) malloc(sizeof(hashEncadeada) * t);
        for (int i = 0; i < t; i++)
            h.tabela.encadeada[i].primeiro = NULL;
        break;

    case aberto:
        h.tabela.aberto = (hashAberto*) malloc(sizeof(hashAberto) * t);
        for (int i = 0; i < t; i++) {
            h.tabela.aberto[i].excluido = false;
            h.tabela.aberto[i].chave[0] = h.tabela.aberto[i].valor[0] = '\0';
        }
        break;
    }
    return h;
}

void destruir(tipoHash h) {
    switch (h.modo) {
    case semColisao:
        free(h.tabela.aberto);
        h.tabela.aberto = NULL;
        break;

    case encadeamento:
		for (int i = 0; i < h.tamanho; i++) {
			listaEncadeada *tmp = h.tabela.encadeada[i].primeiro;
			listaEncadeada *tmp2 = tmp;
			while (tmp != NULL) {
				tmp = tmp->proximo;
				free(tmp2);
				tmp2 = tmp;
			}
		}
		free(h.tabela.encadeada);
		h.tabela.encadeada = NULL;
        break;

    case aberto:
		free(h.tabela.aberto);
		h.tabela.aberto = NULL;
        break;
    }
}


void inserir(tipoHash h, char c[STR_SIZE], char v[STR_SIZE]) {
    int idx = hash(c, h.tamanho);
    switch (h.modo) {
    case semColisao:
        strcpy(h.tabela.aberto[idx].chave, c);
        strcpy(h.tabela.aberto[idx].valor, v);
        break;

    case encadeamento:
		if (h.tabela.encadeada[idx].primeiro == NULL) {
			h.tabela.encadeada[idx].primeiro = (listaEncadeada*) malloc(sizeof(listaEncadeada));
			h.tabela.encadeada[idx].primeiro->proximo = NULL;
			strcpy(h.tabela.encadeada[idx].primeiro->chave, c);
			strcpy(h.tabela.encadeada[idx].primeiro->valor, v);
		}
		else {
			listaEncadeada *tmp = h.tabela.encadeada[idx].primeiro;
			while (tmp->proximo != NULL) {
				tmp = tmp->proximo;
			}
			tmp->proximo = (listaEncadeada*) malloc(sizeof(listaEncadeada));
			tmp = tmp->proximo;
			strcpy(tmp->chave, c);
			strcpy(tmp->valor, v);
			tmp->proximo = NULL;
		}
        break;

    case aberto:
        for (int tentativa = 0; tentativa < h.tamanho; tentativa++) {
            idx = hash(c, h.tamanho, tentativa);
			if (h.tabela.aberto[idx].chave[0] == '\0') {
				strcpy(h.tabela.aberto[idx].chave, c);
        		strcpy(h.tabela.aberto[idx].valor, v);
				break;
			}
        }
        break;
    }
}

char *buscar(tipoHash h, char c[STR_SIZE]) {
    int idx = hash(c, h.tamanho);
    switch (h.modo) {
    case semColisao:
        return strcmp(h.tabela.aberto[idx].chave, c) == 0 ? h.tabela.aberto[idx].valor : NULL;

    case encadeamento: {
		listaEncadeada *tmp = h.tabela.encadeada[idx].primeiro;
		while (tmp != NULL) {
			if (strcmp(tmp->chave, c) == 0) {
				return tmp->valor;
			}
			tmp =tmp->proximo;
		}
        return NULL;
	}
    case aberto:
		for(int tentativa = 0; tentativa < h.tamanho; tentativa++) {
			idx = hash(c, h.tamanho, tentativa);

			if (strcmp(h.tabela.aberto[idx].chave, c) == 0) {
				return h.tabela.aberto[idx].valor;
			}
		}
        return NULL;
    }
}

void apagar(tipoHash h, char c[STR_SIZE]) {
    int idx = hash(c, h.tamanho);
    switch (h.modo) {
    case semColisao:
        if (strcmp(h.tabela.aberto[idx].chave, c) == 0)
            h.tabela.aberto[idx].chave[0] = h.tabela.aberto[idx].valor[0] = '\0';
        break;

    case encadeamento: {
		listaEncadeada *tmp = h.tabela.encadeada[idx].primeiro;
		listaEncadeada *preTemp = tmp;
		while (tmp != NULL) { //enquanto nao chegar no fim da lista
			if (strcmp(tmp->chave, c) == 0) { //se encontrei o elemento a ser removido
				if (h.tabela.encadeada[idx].primeiro == tmp) { //se ele for o primeiro da lista atualizo o apontador da tabela
					h.tabela.encadeada[idx].primeiro = tmp->proximo;
					free(tmp);
				}
				else { //senao atualizo o apontador do elemento anterior ao que esta sendo removido
					preTemp->proximo = tmp->proximo;
					free(tmp);
				}
			}
			preTemp = tmp;
			tmp =tmp->proximo;
		}
        break;
	}
    case aberto:
        // DICA: Nao esquecer de atribuir true para excluido. Caso contrario, uma chave podera ser localizada.
		for(int tentativa = 0; tentativa < h.tamanho; tentativa++) {
			idx = hash(c, h.tamanho, tentativa);

			if (strcmp(h.tabela.aberto[idx].chave, c) == 0) {
				h.tabela.aberto[idx].excluido = true;
				h.tabela.aberto[idx].chave[0] = h.tabela.aberto[idx].valor[0] = '\0';
			}
		}
        break;
    }
}

int quantidade(tipoHash h) {
    int qtd = 0;
    switch (h.modo) {
    case semColisao:
    case aberto: // Igual ao semColisao, portanto, jah implementado!!!
        for (int i = 0; i < h.tamanho; i++)
            if (h.tabela.aberto[i].chave[0] != '\0')
                qtd++;
        break;

    case encadeamento:
		for(int i = 0; i < h.tamanho; i++) {
			listaEncadeada *tmp = h.tabela.encadeada[i].primeiro;
			while (tmp != NULL) {
				qtd++;
				tmp = tmp->proximo;
			}
		}
        break;
    }
    return qtd;
}