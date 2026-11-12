#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TITULO_PLAYLIST 51
typedef struct {
    int codigo;
    char titulo[MAX_TITULO_PLAYLIST];
    int cabeca;
    int fim;
    int esquerda;
    int direita;
} Playlist;
typedef struct {
    int raiz;
    int topo;
} CabecalhoPlaylist;
/* Propósito: Abre o arquivo de playlists, criando-o se não existir
 * Pré-condição: nome é uma string válida e cabecalho aponta para estrutura válida
 * Pós-condição: Retorna ponteiro para arquivo aberto ou NULL em erro; cabeçalho é inicializado com raiz=-1 e topo=sizeof(CabecalhoPlaylist) */
FILE* abrirArquivoPlaylists(const char* nome, CabecalhoPlaylist* cabecalho);
/* Propósito: Fecha o arquivo de playlists e grava o cabeçalho atualizado
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Cabeçalho é atualizado no arquivo e o arquivo é fechado */
void fecharArquivoPlaylists(FILE* arquivo, CabecalhoPlaylist* cabecalho);
/* Propósito: Insere uma nova playlist na árvore binária de playlists
 * Pré-condição: arquivo e cabecalho são válidos; playlist contém código e título válidos
 * Pós-condição: Retorna 1 se inseriu ou 0 se código já existe; playlist é inserida mantendo ordenação; campos cabeca e fim são definidos como -1 */
int inserirPlaylist(FILE* arquivo, CabecalhoPlaylist* cabecalho, Playlist playlist);
/* Propósito: Busca uma playlist pelo código na árvore binária
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Retorna ponteiro estático para playlist encontrada ou NULL se não existir; não modifica a árvore */
Playlist* buscarPlaylist(FILE* arquivo, CabecalhoPlaylist* cabecalho, int codigo);
/* Propósito: Busca a posição no arquivo de uma playlist pelo código
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Retorna posição do registro ou -1 se não encontrada; não modifica a árvore */
int buscarPosicaoPlaylist(FILE* arquivo, CabecalhoPlaylist* cabecalho, int codigo);
/* Propósito: Atualiza a cabeça e o fim da lista de faixas de uma playlist existente
 * Pré-condição: arquivo é válido; posicao é uma posição válida de playlist
 * Pós-condição: Retorna 1 se atualizou ou 0 se posicao inválida; campos cabeca e fim são sobrescritos; demais campos inalterados */
int atualizarListaFaixasPlaylist(FILE* arquivo, int posicao, int novaCabeca, int novoFim);
/* Propósito: Lista todas as playlists cadastradas em ordem crescente de código
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Exibe código e título de todas as playlists em percurso in-order; não modifica a árvore */
void listarPlaylists(FILE* arquivo, CabecalhoPlaylist* cabecalho);
/* Propósito: Interação de menu para cadastrar uma nova playlist via entrada padrão
 * Pré-condição: arquivoPlaylists e cabPlaylist são válidos e abertos para escrita
 * Pós-condição: Lê os dados do usuário e insere a playlist no arquivo, exibindo mensagem de resultado */
void cadastrarPlaylist(FILE* arquivoPlaylists, CabecalhoPlaylist* cabPlaylist);
/* Propósito: Interação de menu para listar todas as playlists cadastradas
 * Pré-condição: arquivoPlaylists e cabPlaylist são válidos
 * Pós-condição: Exibe todas as playlists armazenadas */
void listarTodasPlaylists(FILE* arquivoPlaylists, CabecalhoPlaylist* cabPlaylist);
#endif
