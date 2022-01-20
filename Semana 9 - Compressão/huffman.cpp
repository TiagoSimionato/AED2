#include <stdlib.h>
#include <string.h>

#include "huffman.h"

void criarAlfabeto(char *texto, int tamanho_texto, char *alfabeto[], int *frequencia[], int *tamanho)
{
	int caracteres[255] = {0};
	*tamanho = 0;
	for (int i = 0; i < tamanho_texto; i++) {
		if (caracteres[texto[i]] == 0) {
			(*tamanho)++;
		}
		caracteres[texto[i]]++;
	}
	*alfabeto = (char*) malloc(sizeof(char) * (*tamanho));
	*frequencia = (int*) malloc(sizeof(int) * (*tamanho));

	int added = 0;
	for (int i = 0; i < 255; i++) {
		if (caracteres[i] != 0) {
			(*alfabeto)[added] = i;
			(*frequencia)[added] = caracteres[i];

			added++;
		}
	}
}

struct MinHeapNode* buildHuffmanTree(char alfabeto[], int frequencia[], int tamanho)
{
	MinHeap* mheap = createAndBuildMinHeap(alfabeto, frequencia, tamanho);
	MinHeapNode* a, *b;
	while (mheap->tamanho > 1) {
		a = extractMin(mheap);
		b = extractMin(mheap);

		MinHeapNode* intermediario = newNode('$', a->frequencia + b->frequencia);
		
		intermediario->esquerda = a;
		intermediario->direita = b;

		insertMinHeap(mheap, intermediario);
	}

	a = extractMin(mheap);
	destroyMinHeap(mheap);
	
	return a;
}

void destroyHuffmanTree(struct MinHeapNode* root)
{
	if (!isLeaf(root)) {
		if (root->esquerda != NULL) {
			destroyHuffmanTree(root->esquerda);
		}
		if (root->direita != NULL) {
			destroyHuffmanTree(root->direita);
		}
	}
	free(root);
}

void reccode(struct MinHeapNode *root, int altura, int path, codes_t codes);
void reccode(struct MinHeapNode *root, int altura, int path, codes_t codes) {
	if (!isLeaf(root)) {
		if (root->esquerda != NULL) {
			reccode(root->esquerda, altura + 1, path * 10, codes);
		}
		if (root->direita != NULL) {
			reccode(root->direita, altura + 1, (path * 10) + 1, codes);
		}
	}
	else {
		codes[root->conteudo][0] = altura;
		for (int i = altura; i > 0; i--) {
			codes[root->conteudo][i] = path % 10;
			path = path / 10;
		}
	}
}

void generateCodesArray(struct MinHeapNode *root, codes_t codes)
{
	reccode(root, 0, 0, codes);
}

long sizeOfTextCompressed(char *texto, long tamanho, codes_t codes)
{
	long soma = 0;
	for (int i = 0; i < tamanho; i++) {
		soma += codes[texto[i]][0];
	}
	return soma;
}