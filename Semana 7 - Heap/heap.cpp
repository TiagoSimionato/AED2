#include "heap.h"
//#include "internal.h"

// ATENCAO!!! ATENCAO!!! ATENCAO!!!
// ATENCAO!!! ATENCAO!!! ATENCAO!!!
// ATENCAO!!! ATENCAO!!! ATENCAO!!!
// --- CONSIDERE O INDICE ZERO (A[0]) A QUANTIDADE DE ELEMENTOS
// --- CONSIDERE QUE O VETOR VAI DE 1 ATE N (E NAO DE 0 ATE N-1)
// --- CONSIDERE QUE O VETOOR JA TEM UM TAMANHO SUFICIENTEMENTE GRANDE PARA A INSERCAO

void MaxHeapify (tipoChave A[], int i)
{
    // IMPLEMENTAR
    // OBS.: O indice i vai de 1 ate A[0]
	int esq = 2 * i;
	int dir = 2 * i + 1;
	int max;
	if (esq <= A[0] && A[esq] > A[i]) {
		max = esq;
	}
	else {
		max = i;
	}
	if (dir <= A[0] && A[dir] > A[max]) {
		max = dir;
	}
	if (max != i) {
		long int aux = A[i];
		A[i] = A[max];
		A[max] = aux;
		MaxHeapify(A, max);
	}	
}

void BuildMaxHeap(tipoChave A[])
{
    // IMPLEMENTAR
    // OBS.: Chamar a funcao MaxHeapify
	for (int i = A[0]/2; i > 0; i--) {
		MaxHeapify(A, i);
	}
}

void HeapSort(tipoChave A[])
{
    // IMPLEMENTAR
    // DICA: Como a funcao MaxHeapify nao tem o argumento tamanho (como nos slides),
    //       armazene o tamanho atual (t = A[0]), decremente A[0] antes de chamar
    //       MaxHeapify, entao restaure o valor A[0] = t no final.
	BuildMaxHeap(A);
	int t = A[0];
	for (int i = t; i > 1; i--) {
		long int aux = A[1];
		A[1] = A[i];
		A[i] = aux;

		A[0]--;
		MaxHeapify(A, 1);
	}
	A[0] = t;
}

tipoChave HeapMaximum(tipoChave A[])
{
    // IMPLEMENTAR
    // OBS.: Nao esqueca que o primeiro elemento eh o A[1]
    return A[1];
}

tipoChave HeapExtractMax(tipoChave A[])
{
    // IMPLEMENTAR
    // OBS.: Nao precisa tratar o caso do vetor vazio!
	long int max = A[1];
	A[1] = A[A[0]];
	A[0]--;
	MaxHeapify(A, 1);
	return max;
}

void HeapIncreaseKey(tipoChave A[], int i, tipoChave key)
{
    // IMPLEMENTAR
    // OBS.: Nao precisa tratar o caso da chave alterada for menor que a chave atual!
	A[i] = key;
	while (i > 1 && A[i/2] < A[i]) {
		long int aux = A[i];
		A[i] = A[i/2];
		A[i/2] = aux;

		i = i/2;
	}
}

void MaxHeapInsert(tipoChave A[], tipoChave key)
{
    // IMPLEMENTAR
    // OBS.: Nao esquecer de aumentar a quantidade de elementos, ou seja, A[0]++
	A[0]++;
	A[A[0]] = 0;
	HeapIncreaseKey(A, A[0], key);
}