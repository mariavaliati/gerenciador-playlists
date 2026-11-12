#ifndef MUSICA_H
#define MUSICA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_TITULO 51
#define MAX_ARTISTA 51
#define MAX_LINHA 200
typedef struct {
    int codigo;
    char titulo[MAX_TITULO];
    char artista[MAX_ARTISTA];
    int ano;
    int esquerda;
    int direita;
} Musica;
typedef struct {
    int raiz;
    int topo;
    int livre;
} CabecalhoMusica;
/* Propósito: Abre o arquivo de músicas, criando-o se não existir
 * Pré-condição: nome é uma string válida e cabecalho aponta para estrutura válida
 * Pós-condição: Retorna o arquivo aberto ou NULL em caso de erro; cabeçalho é inicializado */
FILE *abrirArquivoMusicas(const char *nome, CabecalhoMusica *cabecalho);
/* Propósito: Fecha o arquivo de músicas e grava o cabeçalho atualizado
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Cabeçalho é atualizado no arquivo e o arquivo é fechado */
void fecharArquivoMusicas(FILE *arquivo, CabecalhoMusica *cabecalho);
/* Propósito: Insere uma música na árvore binária ordenada por código
 * Pré-condição: arquivo e cabecalho são válidos; musica contém dados válidos
 * Pós-condição: Retorna 1 se inseriu ou 0 se código já existe; árvore mantém ordenação */
int inserirMusica(FILE *arquivo, CabecalhoMusica *cabecalho, Musica musica);
/* Propósito: Busca uma música pelo código na árvore binária
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Retorna ponteiro para música encontrada ou NULL se não encontrada */
Musica *buscarMusica(FILE *arquivo, CabecalhoMusica *cabecalho, int codigo);
/* Propósito: Lista todas as músicas do acervo em ordem crescente de código
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Exibe todas as músicas em percurso in-order sem modificar a árvore */
void listarMusicas(FILE *arquivo, CabecalhoMusica *cabecalho);
/* Propósito: Imprime a árvore binária por níveis
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Exibe os códigos por nível sem modificar a árvore */
void imprimirArvorePorNiveis(FILE *arquivo, CabecalhoMusica *cabecalho);
/* Propósito: Remove uma música da árvore binária pelo código
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Retorna 1 se removeu ou 0 se não encontrou; árvore mantém validade BST; nó removido vai para lista de livres */
int removerMusicaArquivo(FILE *arquivo, CabecalhoMusica *cabecalho, int codigo);
/* Propósito: Imprime os nós livres disponíveis no arquivo de músicas
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Exibe todas as posições dos nós livres disponíveis */
void imprimirNosLivresMusicas(FILE *arquivo, CabecalhoMusica *cabecalho);
/* Propósito: Remove espaços em branco das extremidades de uma string
 * Pré-condição: str é uma string válida
 * Pós-condição: String é modificada removendo espaços no início e fim */
void trimString(char *str);
/* Propósito: Lê um número inteiro da entrada padrão de forma segura
 * Pré-condição: valor aponta para um inteiro válido
 * Pós-condição: Retorna 1 e armazena o inteiro lido em valor, ou retorna 0 se a entrada não for numérica; buffer de entrada é limpo até o fim da linha em qualquer caso */
int lerInteiro(int *valor);
/* Propósito: Interação de menu para cadastrar uma nova música via entrada padrão
 * Pré-condição: arquivoMusicas e cabMusica são válidos e abertos para escrita
 * Pós-condição: Lê os dados do usuário e insere a música no acervo, exibindo mensagem de resultado */
void cadastrarMusica(FILE *arquivoMusicas, CabecalhoMusica *cabMusica);
/* Propósito: Interação de menu para exibir os dados de uma música pelo código
 * Pré-condição: arquivoMusicas e cabMusica são válidos
 * Pós-condição: Lê o código do usuário e exibe os dados da música ou mensagem de erro */
void imprimirDadosMusica(FILE *arquivoMusicas, CabecalhoMusica *cabMusica);
/* Propósito: Interação de menu para listar todas as músicas do acervo
 * Pré-condição: arquivoMusicas e cabMusica são válidos
 * Pós-condição: Exibe todas as músicas armazenadas no sistema */
void listarAcervo(FILE *arquivoMusicas, CabecalhoMusica *cabMusica);
#endif
