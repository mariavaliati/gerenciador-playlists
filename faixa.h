#ifndef FAIXA_H
#define FAIXA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    int codigoMusica;
    int proximo;
} FaixaMusical;
typedef struct {
    int topo;
    int livre;
} CabecalhoFaixa;
/* Propósito: Abre o arquivo de faixas, criando-o se não existir
 * Pré-condição: nome é uma string válida e cabecalho aponta para estrutura válida
 * Pós-condição: Retorna ponteiro para arquivo aberto ou NULL em caso de erro; cabeçalho é inicializado */
FILE* abrirArquivoFaixas(const char* nome, CabecalhoFaixa* cabecalho);
/* Propósito: Fecha o arquivo de faixas e grava o cabeçalho atualizado
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Cabeçalho é atualizado no arquivo e o arquivo é fechado */
void fecharArquivoFaixas(FILE* arquivo, CabecalhoFaixa* cabecalho);
/* Propósito: Cria uma nova faixa musical no arquivo
 * Pré-condição: arquivo e cabecalho são válidos; codigoMusica é válido
 * Pós-condição: Retorna posição da nova faixa ou -1 em erro; faixa é criada com proximo=-1; reutiliza nó livre se disponível */
int criarFaixa(FILE* arquivo, CabecalhoFaixa* cabecalho, int codigoMusica);
/* Propósito: Busca uma faixa musical pelo código da música na lista
 * Pré-condição: arquivo é válido; cabeca é posição inicial da lista
 * Pós-condição: Retorna ponteiro para faixa encontrada (estática) ou NULL; não modifica a lista */
FaixaMusical* buscarFaixa(FILE* arquivo, int cabeca, int codigoMusica);
/* Propósito: Remove uma faixa musical da lista pelo código da música
 * Pré-condição: arquivo e cabecalho são válidos; cabeca e fim são ponteiros válidos
 * Pós-condição: Retorna posição da faixa removida ou -1 se não encontrada; adiciona à lista de livres; atualiza ponteiros da lista */
int removerFaixa(FILE* arquivo, CabecalhoFaixa* cabecalho, int* cabeca, int* fim, int codigoMusica);
/* Propósito: Insere uma faixa no início da lista
 * Pré-condição: arquivo e cabecalho são válidos; cabeca e fim são ponteiros válidos
 * Pós-condição: Retorna 1 se inseriu ou 0 em erro; nova faixa torna-se cabeça; atualiza ponteiros cabeca e fim */
int inserirFaixaInicio(FILE* arquivo, CabecalhoFaixa* cabecalho, int* cabeca, int* fim, int codigoMusica);
/* Propósito: Insere uma faixa no final da lista
 * Pré-condição: arquivo e cabecalho são válidos; cabeca e fim são ponteiros válidos
 * Pós-condição: Retorna 1 se inseriu ou 0 em erro; nova faixa torna-se fim; atualiza ponteiros cabeca e fim */
int inserirFaixaFim(FILE* arquivo, CabecalhoFaixa* cabecalho, int* cabeca, int* fim, int codigoMusica);
/* Propósito: Imprime todas as faixas de uma lista
 * Pré-condição: arquivo é válido
 * Pós-condição: Exibe os códigos das músicas na ordem da lista */
void imprimirListaFaixas(FILE* arquivo, int cabeca);
/* Propósito: Imprime os nós livres disponíveis no arquivo de faixas
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Exibe todas as posições dos nós livres disponíveis */
void imprimirNosLivresFaixas(FILE* arquivo, CabecalhoFaixa* cabecalho);
/* Propósito: Verifica se uma música existe em uma playlist
 * Pré-condição: arquivo é válido
 * Pós-condição: Retorna 1 se a música está na playlist ou 0 caso contrário */
int faixaExisteNaPlaylist(FILE* arquivo, int cabeca, int codigoMusica);
/* Propósito: Conta o número de faixas em uma playlist
 * Pré-condição: arquivo é válido
 * Pós-condição: Retorna o número de faixas na playlist */
int contarFaixas(FILE* arquivo, int cabeca);
#endif
