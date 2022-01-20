#include "trie.h"
#include <stdlib.h>
#include <stdio.h>

// DICA: Estude o arquivo trie.h antes de iniciar a sua implementacao!

no *criarNo(void) {
    // IMPLEMENTAR !!!
    // Dica: Aloca um no, atribua o tipo como 'I' e todos os filhos como NULL.
	no* novo = (no*) malloc(sizeof(no));

	novo->tipo = 'I';

	for (int i = 0; i < TAMANHO_ALFABETO; i++) {
		novo->filho[i] = NULL;
	}
    return novo;
}

void apagarArvore(no *raiz) {
    // IMPLEMENTAR !!!
    // Dica 1: Desaloque todos os nos filhos e depois desaloca o no atual.
    // Dica 2: Usar recursao facilita!
	for (int i = 0; i < TAMANHO_ALFABETO; i++) {
		if (raiz->filho[i] != NULL) {
			apagarArvore(raiz->filho[i]);
		}
	}
	free(raiz);
}

void adicionarPalavra(char *palavra, no *raiz) {
    // IMPLEMENTAR !!!
    // Dica 1: Use um laco iterativo de 0 ate strlen(palavra)
    // Dica 2: Criar o no com a funcao criarNo (caso nao existir)
    // Dica 3: Utilize a macro CHAR_TO_INDEX (ver trie.h) para converter o caractere para o indice filho
    // Dica 4: Nao esqueca de mudar o tipo para 'P' no ultimo noh que representa o ultimo caractere
	no* tmp = raiz;
	int indice;
	for (int letra = 0; letra < strlen(palavra); letra++) {
		indice = CHAR_TO_INDEX(palavra[letra]);
		if (tmp->filho[indice] == NULL) {
			tmp->filho[indice] = criarNo();
		}
		tmp = tmp->filho[indice];
	}
	tmp->tipo = 'P';
}

int buscaPalavra(char *palavra, no *raiz) {
    // IMPLEMENTAR !!!
    // Dica 1: Funcao similar ao adicionarPalavra
    // Dica 2: Se o ultimo noh que representa o ultimo caractere for do tipo 'I', a palavra nao existe
    // IMPORTANTE:
    //   retorne 0 se a palavra nao exite
    //   retorne 1 se a palavra existir
	no* tmp = raiz;
	int letra, indice;
	for (letra = 0; letra < strlen(palavra); letra++) {
		indice = CHAR_TO_INDEX(palavra[letra]);

		if (tmp->filho[indice] != NULL) {
			tmp = tmp->filho[indice];
		}
		else {
			break;	
		}
	}
	if (palavra[letra] == '\0' && tmp->tipo == 'P') {
		return 1;
	}
	else {
		return 0;
	}
}

int numeroDeNos(no *r) {
    // IMPLEMENTAR !!!
    // Dica: A funcao pode ser muito simples se utilizar recursao
	int somador = 0;
	for (int i = 0; i < TAMANHO_ALFABETO; i++) {
		if (r->filho[i] != NULL) {
			somador += numeroDeNos(r->filho[i]);
		}
	}
	if (somador == 0) {
		return 1;
	}
	else {
		return 1 + somador;
	}
    
}

int numeroDePalavras(no *r) {
    // IMPLEMENTAR !!!
    // Dica: Similar a funcao numeroDeNos, mas contabilize apenas os tipos 'P', que representa as palavras
	int somador = 0;
	for (int i = 0; i < TAMANHO_ALFABETO; i++) {
		if (r->filho[i] != NULL) {
			somador += numeroDePalavras(r->filho[i]);
		}
	}
	if (somador == 0 && r->tipo == 'P') {
		return 1;
	}
	else {
		if (r->tipo == 'P') {
			return 1 + somador;
		}
		else {
			return somador;
		}
	}
}

int altura(no *r) {
    // IMPLEMENTAR !!!
    // Dica: A funcao pode ser muito simples se utilizar recursao
	int max = 0, indice = 0;
	for (int i = 0; i < TAMANHO_ALFABETO; i++) {
		if (r->filho[i] != NULL) {
			if (max < altura(r->filho[i])) {
				max = altura(r->filho[i]);
				indice = i;
			}
		}
	}
	if (r->filho[indice] != NULL) {
		return 1 + altura(r->filho[indice]);
	}
	else {
		return 0;
	}
}

void removerrec(char *palavra, no* raiz, int letra, int *concluido) {
	int indice = CHAR_TO_INDEX(palavra[letra]);
	if ( (letra + 1) <= strlen(palavra)) {
    	removerrec(palavra, raiz->filho[indice], letra + 1, concluido);
	}
	int filhos = 0;
	for (int i = 0; i < TAMANHO_ALFABETO; i++) {
		if (raiz->filho[i] != NULL) {
			filhos++;
		}
	}
	if (filhos <= 1 && *concluido == 0) {
		free(raiz);
	}
	else if (*concluido == 0) {
		*concluido = 1;
		raiz->filho[indice] = NULL;
	}
	return;
}

void removerPalavra(char *palavra, no *raiz) {
    // IMPLEMENTAR !!!
    // Nota: Esta funcao eh a mais elaborada. Recomendo criar funcoes auxiliares e utilizar recursao
	int indice, i;
	no *tmp = raiz;
	for (int letra = 0; letra < strlen(palavra); letra++) {
		indice = CHAR_TO_INDEX(palavra[letra]);

		if (tmp->filho[indice] == NULL) {
			return; //palavra nao esta na arvore
		}
		tmp = tmp->filho[indice];
	}
	for (i = 0; i < TAMANHO_ALFABETO; i++) { // palavra a ser removida está contida em outra
		if (tmp->filho[i] != NULL) {
			tmp->tipo = 'I';
			return; //palavra parcial removida
		}
	}
	
	int concluido = 0;
	removerrec(palavra, raiz, 0, &concluido);
	return;
}