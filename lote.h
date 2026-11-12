#ifndef LOTE_H
#define LOTE_H
#include <stdio.h>
#include "musica.h"
#include "playlist.h"
#include "faixa.h"
/* Propósito: Remove espaços em branco das extremidades de uma string
 * Pré-condição: str é uma string válida
 * Pós-condição: String é modificada removendo espaços no início e fim */
void trimCampo(char *str);
/* Propósito: Processa uma linha de comando de um arquivo de lote
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos; linha contém comando válido
 * Pós-condição: Comando é executado; retorna 1 se processou ou 0 em erro */
int processarLinhaLote(char *linha, FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa);
/* Propósito: Carrega e executa todos os comandos de um arquivo de lote
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos; nomeArquivo é string válida
 * Pós-condição: Todos os comandos do arquivo são executados sequencialmente; retorna 1 se sucesso ou 0 em erro */
int carregarArquivoLote(const char *nomeArquivo, FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa);
/* Propósito: Interação de menu para executar um arquivo de lote de operações
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos
 * Pós-condição: Lê o nome do arquivo do usuário e processa todas as operações em lote, exibindo mensagem de resultado */
void executarLote(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa);
#endif
