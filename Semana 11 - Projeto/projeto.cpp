#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "projeto.h"

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
			tmp = tmp->proximo;
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

conversa* criaconversa(char id[64]) {
	conversa* novo = (conversa*) malloc(sizeof(conversa));

	strcpy(novo->ids, id);
	novo->inicio = NULL;
	novo->prox = NULL;

	return novo;
}

Mensagem* criamensagemstruct(int tamanho) {
	Mensagem* novo = (Mensagem*) malloc(sizeof(Mensagem));

	novo->conteudo = (int*) malloc(sizeof(int) * tamanho);
	novo->tamanho = tamanho;
	novo->prox = NULL;

	return novo;
}

void destruirConversas(conversa* primeira) {
	if (primeira != NULL) {
		destruirConversas(primeira->prox);

		destruirMensagens(primeira->inicio);

		free(primeira);
	}
}

void destruirMensagens(Mensagem* primeira) {
	if (primeira != NULL) {
		destruirMensagens(primeira->prox);
		free(primeira->conteudo);
		free(primeira);
	}
}

void criarlogin(tipoHash *logins) {
	char usuario[32]; 
	char senha1[32];
	char senha2[32];
	char avancar = 0;

	printf("\nVocê escolheu criar uma nova conta. Digite o nome de usuario desejado.\n");
	while (avancar == 0) {
		scanf(" %s", usuario);
		if (buscar(*logins, usuario) == NULL) {
			printf("\nO nome escolhido está disponível! Agora digite sua senha.\n");
			avancar = 1;
			
		}
		else {
			printf("\nO nome de usuário escolhido já está em uso. Tente Outro.\n");
		}
		while (avancar == 1) {
			scanf(" %s", senha1);
			printf("\nDigite novamente sua senha para confirmação.\n");
			scanf(" %s", senha2);
			if (strcmp(senha1, senha2) == 0) {
				avancar = 2;
				inserir(*logins, usuario, senha1);
				printf("\nSua conta foi criada!\n");
			}
			else {
				printf("\nSenhas divergem. Tente novamente.\n");
			}
		}	
	}
}

int logar(tipoHash *logins, char *usuario) {
	char senha[32];
	
	printf("\nDigite seu nome de usuario.\n");
	scanf(" %s", usuario);
	char *verifica = buscar(*logins, usuario);
	if (verifica == NULL) {
		printf("\nUsuario não encontrado.\n");
		return 0;
	}

	printf("\nDigite sua senha.\n");
	scanf(" %s", senha);
	if (strcmp(senha, verifica) != 0) {
		printf("\nSenha incorreta!\n");
		return 0;
	}

	return 1;
}

void criarMsg(conversa** primeira, char origem[32], char destino[32]) {
	char pegamensagem[50];
	char mensagem[84];
	printf("\nDigite sua mensagem.\n");
	scanf(" %49[^\n]", pegamensagem);

	//assim será exibido quem escreveu a mensagem ao visualizar o histórico
	strcpy(mensagem, origem);
	strcat(mensagem, ": ");
	strcat(mensagem, pegamensagem);

	//no id da conversa, tanto origem como destino podem estar concatenados primeiro, a depender de quem mandou a primeira mensagem.
	char id1[65];
	char id2[65];

	strcpy(id1, origem);
	strcat(id1, "|");
	strcat(id1, destino);

	strcpy(id2, destino);
	strcat(id2, "|");
	strcat(id2, origem);

	conversa* conversa;
	//procuro a conversa certa. Caso ela não exista, a crio
	if (*primeira == NULL) {
		*primeira = criaconversa(id1);
		conversa = *primeira;
	}
	else {
		conversa = *primeira;
		//busco a conversa certa, que pode estar identificada tanto por id1 como pro id2
		while (conversa->prox != NULL) {
			if (strcmp(conversa->ids,id1) == 0 || strcmp(conversa->ids,id2) == 0) {
				break;
			}
			conversa = conversa->prox;
		}
		//se a conversa não existir, a crio
		if (conversa->prox == NULL && strcmp(conversa->ids,id1) != 0 && strcmp(conversa->ids,id2) != 0) {
			conversa->prox = criaconversa(id1);
			conversa = conversa->prox;
		}
	}

	//a conversa certa foi encontrada, agora adiciono a nova mensagem na lista.
	if (conversa->inicio == NULL) {
		conversa->inicio = comprime(mensagem);
	}
	else {
		Mensagem* aux = conversa->inicio;
		//vou até o final da lista de mensagens e a adiciono
		while (aux->prox != NULL) {
			aux = aux->prox;
		}
		aux->prox = comprime(mensagem);
	}
	printf("\nMensagem enviada!\n");
}

void lerMsg(conversa* primeira, char origem[32], char destino[32]) {
	//no id da conversa, tanto origem como destino podem estar concatenados primeiro, a depender de quem mandou a primeira mensagem.
	char id1[65];
	char id2[65];

	strcpy(id1, origem);
	strcat(id1, "|");
	strcat(id1, destino);

	strcpy(id2, destino);
	strcat(id2, "|");
	strcat(id2, origem);
	conversa* conversa = primeira;

	//busco a conversa certa, que pode estar identificada tanto por id1 como pro id2
	while (conversa->prox != NULL) {
		if (strcmp(conversa->ids,id1) == 0 || strcmp(conversa->ids,id2) == 0) {
			break;
		}
		conversa = conversa->prox;
	}

	if (strcmp(conversa->ids,id1) != 0 && strcmp(conversa->ids,id2) != 0) {
		printf("\nNão há histórico de mensagens com esse usuário.\n");
	}
	else {
		Mensagem* aux = conversa->inicio;
		printf("\n");
		while (aux != NULL) {
			char descomprimido[84] = {'\0'};
			descomprime(descomprimido, aux);
			printf("%s\n", descomprimido);
            aux = aux->prox;
		}
		printf("\n");
	}
}

dicionario* adicionaDicionario(int valor, char representante[84]) {
	dicionario *novo = (dicionario*) malloc(sizeof(dicionario));

	novo->prox = NULL;
	novo->valor = valor;
	strcpy(novo->representante, representante);

	return novo;
}

Mensagem* comprime(char mensagem[84]) {
	dicionario *comeco = NULL;
	dicionario *aux, *anteriorAux;
	codificacao *novamsg = NULL, *novamsgfim;
	int proxvalor = 258;

	char novaEntrada[84];
	novaEntrada[0] = '\0';

	for (int i = 0; i < strlen(mensagem); i++) {
		int len = strlen(novaEntrada);
		novaEntrada[len] = mensagem[i];
		novaEntrada[len + 1] = '\0';
		
		//novaEntrada é adicionada no dicionario, caso necessário.
		if (strlen(novaEntrada) > 1) {
			if (!comeco) {
				//o procedimento também envia uma parte da msg
				adicionaNoDicionario(&comeco, &comeco, &novamsg, &proxvalor, novaEntrada);
				novamsgfim = novamsg;
				aux = comeco;
				novaEntrada[0] = novaEntrada[strlen(novaEntrada) - 1];
				novaEntrada[1] = '\0';
			}
			else {
				int adicionar = 1;

				while (aux != NULL) {
					if (strcmp(novaEntrada, aux->representante) == 0) {
						adicionar = 0;
						break;
					}
					anteriorAux = aux;
					aux = aux->prox;
				}

				if (adicionar) {
					adicionaNoDicionario(&comeco, &anteriorAux, &novamsgfim, &proxvalor, novaEntrada);
					aux = comeco;
					novaEntrada[0] = novaEntrada[strlen(novaEntrada) - 1];
					novaEntrada[1] = '\0';
				}
			}
		}
	}
	//ao final do loop faltará enviar a ultima parte da mensagem
	if (strlen(novaEntrada) == 1) {
		novamsgfim->prox = criacod(novaEntrada[0]);
	}
	else {
		aux = comeco;
		while (aux != NULL) {
			if (strcmp(aux->representante, novaEntrada) == 0) {
				novamsgfim->prox = criacod(aux->valor);
				break;
			}
			aux = aux->prox;
		}
	}
	proxvalor++;

	//transfiro a estrutura da codificacao para o vetor da mensagem
	codificacao* tmp = novamsg;
	Mensagem* definitivo = criamensagemstruct(proxvalor - 258);
	for (int i = 0; i < (proxvalor - 258); i++) {
		definitivo->conteudo[i] = tmp->valor;
		tmp = tmp->prox;
	}

	destroicod(novamsg);
	destroiDicionario(comeco);
	return definitivo;
}

void adicionaNoDicionario(dicionario** comeco, dicionario** fim, codificacao** fimcod, int *proxvalor, char novaEntrada[84]) {
	if (*fim == NULL) {
		*fim = adicionaDicionario(*proxvalor, novaEntrada);
	}
	else {
		(*fim)->prox = adicionaDicionario(*proxvalor, novaEntrada);
	}
	(*proxvalor)++;
	//cada vez que uma entrada é adicionada no dicionario, uma parte da mensagem é enviada
	char enviar[84] = {'\0'};
	strncpy(enviar, novaEntrada, strlen(novaEntrada) - 1);
	if (strlen(enviar) == 1) {
		if ((*fimcod) == 0) { //caso seja a primeira parte da msg
			*fimcod = criacod(enviar[0]);
		}
		else {
			(*fimcod)->prox = criacod(enviar[0]);
			(*fimcod) = (*fimcod)->prox;
		}
	}
	else { //se strlen(enviar) > 1, há uma entrada no dicionario
		dicionario *aux = *comeco;
		while (aux != NULL) {
			if (strcmp(enviar, aux->representante) == 0) {
				if ((*fimcod) == NULL) { //caso seja a primeira parte da msg
					(*fimcod) = criacod(aux->valor);
				}
				else {
					(*fimcod)->prox = criacod(aux->valor);
					(*fimcod) = (*fimcod)->prox;
				}
				break;
			}
			aux = aux->prox;
		}
	}
}

void descomprime(char *descomprimido, Mensagem *comprimido) {
	dicionario *comeco = NULL;
	dicionario *aux, *anteriorAux;
	int proxvalor = 258;

	char novaEntrada[84];
	novaEntrada[0] = '\0';
	char anterior;

	char enviado[84];
	for (int i = 0; i < comprimido->tamanho + 1; i++) {
		//incremento novaEntrada para atualizar o dicionario
		if (comprimido->conteudo[i] < 258) {
			int len = strlen(novaEntrada);
			novaEntrada[len] = comprimido->conteudo[i];
			novaEntrada[len + 1] = '\0';
			enviado[0] = comprimido->conteudo[i];
			enviado[1] = '\0';
		}
		else {
			aux = comeco;
			int conhecido = 0;
			while(aux != NULL) {
				if (aux->valor == comprimido->conteudo[i]) {
					int len = strlen(novaEntrada);
					novaEntrada[len] = aux->representante[0];
					novaEntrada[len + 1] = '\0';
					strcpy(enviado, aux->representante);
					conhecido = 1;
				}
				aux = aux->prox;
			}
			if (!conhecido) {
				int len = strlen(novaEntrada);
				novaEntrada[len] = anterior;
				novaEntrada[len + 1] = '\0';
				strcpy(enviado, novaEntrada);
			}
		}

		//atualizo a dicionario
		if (strlen(novaEntrada) > 1) {
			if (!comeco) {
				comeco = adicionaDicionario(proxvalor, novaEntrada);
				proxvalor++;
				aux = comeco;
				strcpy(novaEntrada, enviado);
			}
			else {
				int adicionar = 1;

				aux = comeco;
				while (aux != NULL) {
					if (strcmp(novaEntrada, aux->representante) == 0) {
						adicionar = 0;
						break;
					}
					anteriorAux = aux;
					aux = aux->prox;
				}

				if (adicionar) {
					anteriorAux->prox = adicionaDicionario(proxvalor, novaEntrada);
					proxvalor++;
					aux = comeco;
					strcpy(novaEntrada, enviado);
				}
			}
		}

		//descomprimo a mensagem
		strcat(descomprimido, enviado);
		anterior = enviado[0];
	}
	destroiDicionario(comeco);
}

codificacao *criacod(int valor) {
	codificacao *novo = (codificacao*) malloc(sizeof(codificacao));

	novo->valor = valor;
	novo->prox = NULL;

	return novo;
}

void destroiDicionario(dicionario *primeiro) {
	if (primeiro != NULL) {
		destroiDicionario(primeiro->prox);

		free(primeiro);
	}
}

void destroicod(codificacao *primeiro) {
	if (primeiro != NULL) {
		destroicod(primeiro->prox);

		free(primeiro);
	}
}