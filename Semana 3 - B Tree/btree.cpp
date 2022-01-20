#include <stdlib.h>
#include "btree.h"

btNo *inserirInterno(btNo *b, int chave, int *mediana, const unsigned char ordem);
int buscarChave(int n, const int *a, int chave);
int btBuscarRec(btNo *b, int chave);
int btContaNosRec(btNo *b);
int btContaChavesRec(btNo *t);

btNo *btCriarNo(const unsigned char ordem) {
	btNo *no = (btNo*) malloc(sizeof(btNo));
	no->ehFolha = 1;
	no->numChaves = 0;
	no->chaves = (int*) malloc(sizeof(*no->chaves) * ordem);
	no->filhos = (btNo**) malloc(sizeof(*no->filhos) * (ordem + 1));
	return no;
}

bTree btCriar(const unsigned char ordem) {
    bTree b;
    b.ordem = ordem;
	b.raiz = btCriarNo(ordem);
    return b;
}

void btDestruirRec(btNo *no) {
	if (no == NULL) {
		return;
	}
	if(!no->ehFolha) {
		for (int i = 0; i < no->numChaves+1; i++) {
			btDestruirRec(no->filhos[i]);
		}
	}
	free(no->chaves);
	free(no->filhos);
	free(no);
	
}

void btDestruir(bTree b) {
	if(b.raiz != NULL) {
		btDestruirRec(b.raiz);
	}
}

void btInserir(bTree b, int chave) {
	btNo *b1;
	btNo *b2;
	int mediana;

	b2 = inserirInterno(b.raiz, chave, &mediana, b.ordem);

	if(b2) {
		b1 = btCriarNo(b.ordem);
		for( unsigned char i = 0; i < b.ordem; i++) {
			b1->chaves[i] = b.raiz->chaves[i];
			b1->filhos[i] = b.raiz->filhos[i];
		} b1->filhos[b.ordem] = b.raiz->filhos[b.ordem];
		b1->ehFolha = b.raiz->ehFolha;		b.raiz->ehFolha = 0;
		b1->numChaves = b.raiz->numChaves;  b.raiz->numChaves = 1;
		b.raiz->chaves[0] = mediana;
		b.raiz->filhos[0] = b1;
		b.raiz->filhos[1] = b2;
	}
}

btNo *inserirInterno(btNo *b, int chave, int *mediana, const unsigned char ordem) {
	int pos;
	int mid;
	btNo *b2;

	pos = buscarChave(b->numChaves, b->chaves, chave);
	if (pos < b->numChaves && b->chaves[pos] == chave) {
		return 0;
	}

	if (b->ehFolha) {
		for (int i = b->numChaves; i > pos; i--) {
			b->chaves[i] = b->chaves[i-1];
		}
		b->chaves[pos] = chave;
		b->numChaves++;
	} 
	else {
		b2 = inserirInterno(b->filhos[pos], chave, &mid, ordem);
		if (b2) {
			for(int i = b->numChaves; i > pos; i--) {
				b->chaves[i] = b->chaves[i-1];
				b->filhos[i + 1] = b->filhos[i];
			}
			b->chaves[pos] = mid;
			b->filhos[pos + 1] = b2;
			b->numChaves++;
		}
	}
	if (b->numChaves >= ordem) {
		mid = b->numChaves/2;
		*mediana = b->chaves[mid];
		b2 = btCriarNo(ordem);
		b2->numChaves = b->numChaves - mid - 1;
		b2->ehFolha = b->ehFolha;

		for (int i = mid + 1; i < b->numChaves; i++) {
			b2->chaves[i - (mid + 1)] = b->chaves[i];
		}
		if (!b->ehFolha) {
			for (int i = mid + 1; i < b->numChaves + 1; i++) {
				b2->filhos[i - (mid + 1)] = b->filhos[i];
			}
		}

		b->numChaves = mid;
		return b2;
	}
	else {
		return 0;
	}
}

int buscarChave(int n, const int *a, int chave) {
	int lo = -1, hi = n, mid;
	while (lo + 1 < hi) {
		mid = (lo + hi)/2;
		if(a[mid] == chave) {
			return mid;
		}
		else if (a[mid] < chave) {
			lo = mid;
		}
		else {
			hi = mid;
		}
	}
	return hi;
}

int btBuscarRec(btNo *b, int chave) {
	int pos;
	if (b->numChaves == 0) {
		return 0;
	}

	pos = buscarChave(b->numChaves, b->chaves, chave);

	if (pos < b->numChaves && b->chaves[pos] == chave) {
		return 1;
	}
	else {
		return(!b->ehFolha && btBuscarRec(b->filhos[pos], chave));
	}
}

int btBuscar(bTree b, int chave) {
    return btBuscarRec(b.raiz, chave);
}

int btAltura(bTree b) {
	int altura = 1;
	for(btNo *tmp = b.raiz; !tmp->ehFolha; tmp = tmp->filhos[0]) {
		altura++;
	}
    return altura;
}


int btContaNosRec(btNo *b) {
	if (b->ehFolha) {
		return 1;
	}
	int cont = 1;
	for (int i = 0; i < b->numChaves + 1; i++) {
		cont += btContaNosRec(b->filhos[i]);
	}
	return cont;
}

int btContaNos(bTree b) {
    return btContaNosRec(b.raiz);
	
}

int btContaChavesRec(btNo *t) {
	if (t->ehFolha) {
		return t->numChaves;
	}
	int cont = t->numChaves;
	for (int i = 0; i < t->numChaves + 1; i++) {
		cont += btContaChavesRec(t->filhos[i]);
	}
	return cont;
}

int btContaChaves(bTree t) {
    return btContaChavesRec(t.raiz);
}