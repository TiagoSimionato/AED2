#include "projeto.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	tipoHash logins = criar(encadeamento, 50);
	int escolha;
	conversa* conversas = NULL;

	printf("\nNão há nenhuma conta logada. Digite 1 para logar. 2 para criar uma conta. Para sair, 0.\n");
	scanf(" %d", &escolha);
	//laço que permite o usuário criar uma conta nova ou entrar em uma existente.
	while (escolha != 0) {
		if (escolha == 1) {
			char usuario[32];
			if (logar(&logins, usuario)) {
				printf("\nOlá, %s. Escolha uma ação:\nMandar Mensagem: 1\nVisualizar Mensagens: 2\nDeslogar: 3\n", usuario);
				scanf(" %d", &escolha);
				//laço que permite um usuário logado mandar uma mensagem para outro usuário, ou visualizar suas mensagens com outro usuário.
				while (escolha != 3) {
					char destino[32];
					if (escolha == 1) {
						char mensagem[50];
								
						printf("\nPara quem é a mensagem?\n");
						scanf(" %s", destino);
						if (buscar(logins, destino) == NULL) {
							printf("\nNão foi possivel encontrar esse usuário\n");
						}
						else {
							criarMsg(&conversas, usuario, destino);
						}
					}
					else if (escolha == 2) {
						printf("\nDeseja ver seu histórico de mensagens com qual usuário?\n");
						scanf(" %s", destino);
						if (buscar(logins, destino) == NULL) {
							printf("\nNão foi possivel encontrar esse usuário\n");
						}
						else {
							lerMsg(conversas, usuario, destino);
						}
					}
					else {
						printf("\nOpção escolhida não é válida. Tente novamente.\n");
					}

					if (escolha != 3) {
						printf("\nEscolha uma ação:\nMandar Mensagem: 1\nVisualizar Mensagens: 2\nDeslogar: 3\n");
						scanf(" %d", &escolha);
					}
					
				}
			}
		}
		else if (escolha == 2) {
			criarlogin(&logins);
		}
		else {
			printf("\nOpção escolhida não é válida. Tente novamente.\n");
		}
		printf("\nVoce não está cadastrado. 1 para logar. 2 para criar no login. Para sair, 0.\n");
		scanf(" %d", &escolha);
	}

	destruir(logins);
	destruirConversas(conversas);
}