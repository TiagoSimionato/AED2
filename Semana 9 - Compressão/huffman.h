#ifndef HUFFMAN_H
#define HUFFMAN_H

// Altura maxima da arvore de Huffman.
#define MAX_TREE_HT 100

// Mapa dos codigos do alfabeto gerado.
// O valor de A[c][0] contem o numero de bits do codigo do caractere c. Se 0, nao ha codificacao disponivel.
// Os valores de A[c][1..n], sendo n = A[c][0], pode ser 0 ou 1, de acordo com o codigo do caractere gerado.
typedef char codes_t[255][MAX_TREE_HT+1];

//////////////
// MIN-HEAP //

struct MinHeapNode
{
	char conteudo;                          // Caractere
	unsigned frequencia;                    // Quantidade de ocorrencias do caractere
	struct MinHeapNode *esquerda, *direita; // Ponteiro para os filhos, pois Huffman utiliza uma arvore binaria
};
 
struct MinHeap
{
	unsigned tamanho;           // Quantidade de elementos na lista
	unsigned capacidade;        // Capacidade maxima de elementos
	struct MinHeapNode **array; // Vetor de ponteiros da heap
};

// ATENCAO: As funcoes abaixo ja se encontram implementadas!
struct MinHeap*     createAndBuildMinHeap(char alfabeto[], int frequencia[], int tamanho); // Aloca e constroi a lista heap
struct MinHeapNode* newNode(char conteudo, unsigned frequencia); // Aloca um novo no, uma vez que o array de MinHeap eh uma lista de ponteiros
struct MinHeapNode* extractMin(struct MinHeap* minHeap); // Extrai o menor noh (removendo do heap)
void                insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode); // Insere um novo noh
inline int          isLeaf(struct MinHeapNode* root) {return !(root->esquerda) && !(root->direita);} // Funcao auxiliar, verifica se o noh eh uma folha
void                destroyMinHeap(struct MinHeap* root); // Desaloca toda a estrutura

// MIN-HEAP //
//////////////


void criarAlfabeto(char *texto, int tamanho_texto, char *alfabeto[], int *frequencia[], int *tamanho);
struct MinHeapNode* buildHuffmanTree(char alfabeto[], int frequencia[], int tamanho);
void destroyHuffmanTree(struct MinHeapNode* root);
void generateCodesArray(struct MinHeapNode *root, codes_t codes);
long sizeOfTextCompressed(char *texto, long tamanho, codes_t codes);

#endif