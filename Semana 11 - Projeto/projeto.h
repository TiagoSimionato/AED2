#ifndef PROJETO_H
#define PROJETO_H

#define STR_SIZE 32 // Tamanho maximo da chave e do valor (que eh do tipo string)

// Tres tipos de hash, o primeiro jah esta implementado
typedef enum { semColisao, encadeamento, aberto } modoHash;

// Estrutura utilizada no hash semColisao e aberto
struct hashAberto {
    char chave[STR_SIZE];
    char valor[STR_SIZE];
    bool excluido; // Utilizado para encontrar uma chave em hash com encadeamento aberto
};

// Estrutura da lista ligada
struct listaEncadeada {
    char chave[STR_SIZE];
    char valor[STR_SIZE];
    listaEncadeada *proximo;
};

// Estrutura utilizada no hash com encadeamento externo
struct hashEncadeada {
    listaEncadeada *primeiro;
};

// Estrutura principal do hash
struct tipoHash {
  union { // A estrutura union permite 'selecionar' a estrutura da tabela que sera utilizada
      hashEncadeada *encadeada;
      hashAberto    *aberto;
  } tabela;
  int tamanho;   // Tamanho da tabela hash
  modoHash modo; // Modo de operacao da tabela hash: semColisao, encadeamento, aberto
};

//lista ligada com todas as conversas entre usuarios que o programa registrou
struct conversa {
	char ids[65];
	struct Mensagem *inicio;
	struct conversa *prox;
};

//lista ligada com o historico de mensagens comprimidas de uma conversa
struct Mensagem {
	int *conteudo;
	int tamanho;
	struct Mensagem *prox;
};

//lista ligada que guarda o dicionario usado pelas compressoes e descompressoes
struct dicionario {
	int valor;
	char representante[84];
	struct dicionario *prox;
};

//estrutura de lista ligada que guarda a nova codificacao da mensagem que sera guardada
struct codificacao {
	int valor;
	struct codificacao *prox;
};

// Funcao hash, que transforma a chave em um indice da tabela
int hash(char chave[STR_SIZE], int tamanho, int tentativa = 0);

// Inicializa uma tabela hash
tipoHash criar(modoHash modo, int tamanho);

// Libera da memoria toda a tabela
void destruir(tipoHash hash);

//insere uma chave com seu respectivo valor na tabela hash
void inserir(tipoHash hash, char chave[STR_SIZE], char valor[STR_SIZE]);

//busca uma chave na tabela hash e retorna seu valor, ou NULL caso a chave não seja encontrada
char *buscar(tipoHash hash, char chave[STR_SIZE]);

//apaga uma chave da tabela hash
void apagar(tipoHash hash, char chave[STR_SIZE]);

//cria um novo item da lista ligada de conversas com o id passado, que é a junção dos nomes de usuario envolvidos, e retorna o endereço do nó
conversa *criaconversa(char id[64]);

//cria um novo item da lista ligada de mensagens, com o conteudo da mensagem passado, e retorna o endereço do nó
Mensagem *criamensagemstruct(int tamanho);

//libera a memória de toda a lista ligada de conversas
void destruirConversas(conversa* primeira);

//libera a memória de toda a lista ligada de mensagens de uma conversa
void destruirMensagens(Mensagem* primeira);

//Exibe a 'interface' para criar uma nova conta e a insere na tabela hash
void criarlogin(tipoHash *logins);

//Exibe a 'interface' de login para o usuário e retorna 1 se o usuário conseguiu logar, 0 cc.
int logar(tipoHash *logins, char *usuario);

//Cria uma nova mensagem e a armazena na conversa entre o usuário remetente e o usuário destinatário
void criarMsg(conversa** conversas, char origem[32], char destino[32]);

//Le o historico de mensagens do usuario logado com outro escolhido por este e o imprime na tela
void lerMsg(conversa* conversas, char origem[32], char destino[32]);

//cria uma nova instacia da estrutura do dicionario
dicionario* adicionaDicionario(int valor, char representante[84]);

//comprime a mensagem passada como parametro a retorna a estrutura de mensagem comprimida correspondente
Mensagem* comprime(char mensagem[84]);

//auxilia a função de comprimir a adicionar novos itens no dicionario dinamico
void adicionaNoDicionario(dicionario** comeco, dicionario** fim, codificacao** fimcod, int *proxvalor, char novaEntrada[84]);

//descomprime uma mensagem e coloca o resultado na string 'descomprimido'
void descomprime(char *descomprimido, Mensagem *comprimido);

//cria uma nova instancia da estrutura de lista ligada da nova codificacao da msg
codificacao* criacod(int valor);

//libera a memória usada para armazenar o dicionario
void destroiDicionario(dicionario *primeiro);

//libera a memória usada para armazenar a codificacao da mensagem
void destroicod(codificacao *primeiro);

#endif