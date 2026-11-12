#ifndef SISTEMA_H
#define SISTEMA_H
#include <stdio.h>
#include "musica.h"
#include "playlist.h"
#include "faixa.h"
/* Propósito: Adiciona uma música existente no início de uma playlist
 * Pré-condição: Arquivos e cabeçalhos são válidos e abertos; playlist e música existem
 * Pós-condição: Música é inserida como primeira faixa; campos cabeca e fim são atualizados; retorna 1 se sucesso ou 0 em erro */
int adicionarMusicaInicioPlaylist(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa, int codigoPlaylist, int codigoMusica);
/* Propósito: Adiciona uma música existente no final de uma playlist
 * Pré-condição: Arquivos e cabeçalhos são válidos e abertos; playlist e música existem
 * Pós-condição: Música é inserida como última faixa; campos cabeca e fim são atualizados; retorna 1 se sucesso ou 0 em erro */
int adicionarMusicaFimPlaylist(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa, int codigoPlaylist, int codigoMusica);
/* Propósito: Remove a primeira ocorrência de uma música de uma playlist
 * Pré-condição: Arquivos de playlists e faixas são válidos e abertos; playlist existe
 * Pós-condição: Primeira ocorrência da música é removida; nó vai para lista de livres; cabeca e fim são atualizados; retorna 1 se sucesso ou 0 em erro */
int removerMusicaDaPlaylist(FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa, int codigoPlaylist, int codigoMusica);
/* Propósito: Remove uma música do acervo e de todas as playlists que a contêm
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos; música existe
 * Pós-condição: Música é removida da árvore do acervo; todas as ocorrências são removidas das playlists; nós removidos vão para listas de livres; retorna 1 se sucesso ou 0 em erro */
int removerMusicaDoSistema(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa, int codigoMusica);
/* Propósito: Imprime uma playlist completa com título e todas as músicas na ordem da lista
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos; playlist existe
 * Pós-condição: Exibe título da playlist e dados de cada música; nenhum arquivo é modificado */
void imprimirPlaylistCompleta(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, int codigoPlaylist);
/* Propósito: Interação de menu para adicionar uma música do acervo no início de uma playlist
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos
 * Pós-condição: Lê os códigos do usuário e insere a música no início da playlist, exibindo mensagem de resultado */
void adicionarMusicaInicio(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa);
/* Propósito: Interação de menu para adicionar uma música do acervo no final de uma playlist
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos
 * Pós-condição: Lê os códigos do usuário e insere a música no final da playlist, exibindo mensagem de resultado */
void adicionarMusicaFinal(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa);
/* Propósito: Interação de menu para remover uma música de uma playlist
 * Pré-condição: arquivoPlaylists, cabPlaylist, arquivoFaixas e cabFaixa são válidos
 * Pós-condição: Lê os códigos do usuário e remove a música da playlist, exibindo mensagem de resultado */
void removerMusicaPlaylist(FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa);
/* Propósito: Interação de menu para exibir o conteúdo completo de uma playlist
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos
 * Pós-condição: Lê o código da playlist do usuário e exibe seus dados e músicas */
void imprimirPlaylist(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas);
/* Propósito: Interação de menu para remover uma música do sistema
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos
 * Pós-condição: Lê o código do usuário e remove a música do acervo e de todas as playlists, exibindo mensagem de resultado */
void removerMusica(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa);
#endif
