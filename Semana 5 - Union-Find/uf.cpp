#include "uf.h"

grafo* criarGrafo(int V, int E) {
    grafo* g = (grafo*) malloc( sizeof(grafo) );
    g->V = V;
    g->E = E;
    g->VetorDeArestas = (aresta*) malloc(g->E * sizeof(aresta) );
    return g;
}

void destruirGrafo(grafo *g) {
    free(g->VetorDeArestas);
    free(g);
}

subset *Make_Subset(int tamanho) {
    subset *subconjuntos = (subset*) malloc(tamanho * sizeof(subset));
    for (int i=0; i<tamanho; i++) {
        Make_Set(subconjuntos, i);
    }
    return subconjuntos;
}

void Destroy_Subset(subset *s) {
    free(s);
}

void Make_Set(subset subconjuntos[], int i) {
    subconjuntos[i].pai = i;
    subconjuntos[i].rank = 0;
}

// Funcao que procura o representante (pai) do elemento i com compressao de caminho.
int Find_Set(subset subconjuntos[], int i) {
	if (i != subconjuntos[i].pai) {
		subconjuntos[i].pai = Find_Set(subconjuntos, subconjuntos[i].pai);
	} 
    return subconjuntos[i].pai;
}

// Funcao que junta os conjuntos de x e y com uniao ponderada.
void Union(subset subconjuntos[], int x, int y) {
	x = Find_Set(subconjuntos, x);
	y = Find_Set(subconjuntos, y);
	if (subconjuntos[x].rank > subconjuntos[y].rank) {
		subconjuntos[y].pai = x;
	}
	else {
		subconjuntos[x].pai = y;
		if(subconjuntos[x].rank == subconjuntos[y].rank) {
			subconjuntos[y].rank++;
		}
	}
	
}

// Funcao utilizada para verificar se o grafo tem ou nao ciclo
bool TemCiclo( grafo* g ) {
    bool tem_ciclo = false;
    subset *s = Make_Subset(g->V); //inicializo vetor unionfind

	for (int i = 0; i < g->E; i++) { //itero sobre cada aresta para ir unindo os vertices
		int pai_origem = Find_Set(s, g->VetorDeArestas[i].origem);
		int pai_destino = Find_Set(s, g->VetorDeArestas[i].destino);

		if (pai_origem == pai_destino) { //caso os vertices ja estejam unidos, há um ciclo
			tem_ciclo = true;
		}
		else {
			Union(s, pai_origem, pai_destino);
		}
	}
    Destroy_Subset(s);
    return tem_ciclo;
}

int QuantidadeComponentesConexas( grafo* g ) {
    subset *s = Make_Subset(g->V); //inicializo o conjunto unionfind de acordo com o grafo
    for(int i=0; i<g->E; i++) {
        int rx = Find_Set(s, g->VetorDeArestas[i].origem);
        int ry = Find_Set(s, g->VetorDeArestas[i].destino);
        if (rx!=ry)
        	Union(s, rx, ry);
    }

	int nc=0; //contador de quantos representantes há
	for (int i = 0; i < g->V; i++) { //percorro a o vetor do unionfind e para cada representante nc++;
		if (i == s[i].pai) {
			nc++;
		}
	}
    Destroy_Subset(s);
    return nc;
}


int MaiorComponenteConexa( grafo* g ) {
	int i; //variavel de iteracao de 3 for's

	subset *s = Make_Subset(g->V); //inicializo o vetor unionfind
    for (i = 0; i < g->E; i++) {
        int pai_origem = Find_Set(s, g->VetorDeArestas[i].origem);
        int pai_destino = Find_Set(s, g->VetorDeArestas[i].destino);

        if (pai_origem != pai_destino) {
        	Union(s, pai_origem, pai_destino);
		}
    }

	int contacomponente[g->V]; //inicializo a lista com 0
	for (i = 0; i < g->V; i++) {
		contacomponente[i] = 0;
	}

	for (int i = 0; i < g->V; i++) { //para cada componente, incremento seu representante
		int pai_i = Find_Set(s, i);
		contacomponente[pai_i]++;
	}

	int max = 0;
	for (i = 0; i < g->V; i++) { //procuro o representante com mais componentes
		if (max < contacomponente[i]) {
			max = contacomponente[i];
		}
	}
	Destroy_Subset(s);
    return max;
}

//função kruskal que gera a arvore minima, adiciona os pesos usados no ponteiro do acumulador e reorganiza a lista de arestas do grafo de forma cresente
aresta *kruskal (grafo* g, int* acumulador) {
	aresta* arvore = (aresta*) malloc(g->E * sizeof(aresta)); //inicializo a arvore minima

	subset *s = Make_Subset(g->V); //inicializo o conjunto unionfind
	for (int i = 0; i < g->V; i++) {
		Make_Set(s, i);
	}

	for(int i = 0; i < g->E; i++) { //organizo as arestas pelo menor peso
		int min = i;
		for (int j = i; j < g->E; j++) {
			if (g->VetorDeArestas[min].peso > g->VetorDeArestas[j].peso) {
				min = j;
			}
		}
		aresta tmp = g->VetorDeArestas[min];
		g->VetorDeArestas[min] = g->VetorDeArestas[i];
		g->VetorDeArestas[i] = tmp;
	}

	int a_indice = 0;
	for (int i = 0; i < g->E; i++) {//loop do algoritmo de kruskal para gerar a arvore minima
		if (Find_Set(s, g->VetorDeArestas[i].origem) != Find_Set(s, g->VetorDeArestas[i].destino)) {
			arvore[a_indice].destino = g->VetorDeArestas[i].destino;
			arvore[a_indice].origem = g->VetorDeArestas[i].origem;
			arvore[a_indice].peso = g->VetorDeArestas[i].peso;

			a_indice++;

			(*acumulador) += g->VetorDeArestas[i].peso; //soma o peso total da arvore

			Union(s, g->VetorDeArestas[i].origem, g->VetorDeArestas[i].destino);
		}
	}
	Destroy_Subset(s);
	return arvore;
}

int SomaPesoArestasDaArvoreGeradoraMinima( grafo* g ) {
	int acumulador = 0;

	//função kruskal que gera a arvore minima e adiciona os pesos usados no acumulador
	aresta* arvore_min = kruskal(g, &acumulador);

	free(arvore_min);
    return acumulador;
}