#include <stdio.h>

void MinHeapify (int A[], int m, int i)
{
// IMPLEMENTAR
// DICA: Igual ao MaxHeapify, mas com os sinais invertidos
	int esq = 2 * i;
	int dir = 2 * i + 1;
	int min;
	if (esq <= m && A[esq] < A[i]) {
		min = esq;
	}
	else {
		min = i;
	}
	if (dir <= m && A[dir] < A[min]) {
		min = dir;
	}
	if (min != i) {
		int aux = A[i];
		A[i] = A[min];
		A[min] = aux;
		MinHeapify(A, m, min);
	}
}
void BuildMinHeap(int A[], int n)
{
// IMPLEMENTAR
// DICA: Igual ao BuildMaxHeap, mas chama o MinHeapify
	for (int i = n/2; i > 0; i--) {
		MinHeapify(A, n, i);
	}
}

void PartialHeapSortInverse(int A[], int n, int k)
{
// IMPLEMENTAR
// IMPORTANTE: Considere que o A[0] contem o tamanho da lista, e os elementos estao nas posicoes de 1 ate n (inclusive)
// DICA: Construa o MinHeap e insira os menores elementos no fim da lista (muito similar ao HeapSort, mas com os menores elementos)
//       Lembre-se de parar com k elementos!
//       A LISTA FICARA INVERTIDA, ou seja, o menor elemento eh o ultimo, depois o penultimo... e assim por diante...
	BuildMinHeap(A, n);
	int m = n;
	for (int i = n; i > n - k; i--) {
		long int aux = A[1];
		A[1] = A[i];
		A[i] = aux;

		m--;
		MinHeapify(A, m, 1);
	}
}

void PartialHeapSort(int A[], int n, int k) {
// IMPLEMENTAR
// IMPORTANTE: Diferente das funcoes anteriores, os elementos comecam na posicao 0, e a saida nao sera invertida
// DICA: 1) Crie um novo array com tamanho n+1
//       2) Copie todos os elementos deslocado em um para o novo array
//       3) Chame o PartialHeapSortInverse
//       4) Copie novamente os elementso para o array original, invertendo a ordem e iniciando na posicao 0
	int aux[n + 1];
	aux[0] = n;
	for (int i = 1; i < n + 1; i++) {
		aux[i] = A[i - 1];
	}
	PartialHeapSortInverse(aux,  n,  k);

	for (int i = 0; i < n; i++) {
		A[i] = aux[n - i];
	}
}