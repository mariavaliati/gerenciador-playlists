#include "lote.h"
#include "sistema.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Propósito: Remove espaços em branco das extremidades de uma string
 * Pré-condição: str é uma string válida
 * Pós-condição: String é modificada removendo espaços no início e fim */
void trimCampo(char *str) {
    if (str == NULL) {
        return;
    }
    int inicio = 0;
    int fim;
    int i;
    while (str[inicio] != '\0' && isspace((unsigned char)str[inicio])) {
        inicio++;
    }
    fim = strlen(str) - 1;
    while (fim >= inicio && isspace((unsigned char)str[fim])) {
        fim--;
    }
    for (i = 0; inicio + i <= fim; i++) {
        str[i] = str[inicio + i];
    }
    str[i] = '\0';
}

/* Propósito: Processa uma linha de comando de um arquivo de lote
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos; linha contém comando válido
 * Pós-condição: Comando é executado; retorna 1 se processou ou 0 em erro */
int processarLinhaLote(char *linha, FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa) {
    char *operacao;
    linha[strcspn(linha, "\n")] = '\0';
    trimCampo(linha);
    if (strlen(linha) == 0) {
        return 0;
    }
    operacao = strtok(linha, ";");
    if (operacao == NULL) {
        return 0;
    }
    trimCampo(operacao);
    if (strcmp(operacao, "M") == 0) {
        char *codigoStr = strtok(NULL, ";");
        char *titulo = strtok(NULL, ";");
        char *artista = strtok(NULL, ";");
        char *anoStr = strtok(NULL, ";");
        if (codigoStr == NULL || titulo == NULL || artista == NULL || anoStr == NULL) {
            printf("Linha invalida para operacao M.\n");
            return 0;
        }
        trimCampo(codigoStr);
        trimCampo(titulo);
        trimCampo(artista);
        trimCampo(anoStr);
        Musica musica;
        musica.codigo = atoi(codigoStr);
        strncpy(musica.titulo, titulo, MAX_TITULO - 1);
        musica.titulo[MAX_TITULO - 1] = '\0';
        strncpy(musica.artista, artista, MAX_ARTISTA - 1);
        musica.artista[MAX_ARTISTA - 1] = '\0';
        musica.ano = atoi(anoStr);
        musica.esquerda = -1;
        musica.direita = -1;
        if (inserirMusica(arquivoMusicas, cabMusica, musica)) {
            printf("Musica %d cadastrada.\n", musica.codigo);
        } else {
            printf("Nao foi possivel cadastrar a musica %d.\n", musica.codigo);
        }
        return 1;
    }
    if (strcmp(operacao, "P") == 0) {
        char *codigoStr = strtok(NULL, ";");
        char *titulo = strtok(NULL, ";");
        if (codigoStr == NULL || titulo == NULL) {
            printf("Linha invalida para operacao P.\n");
            return 0;
        }
        trimCampo(codigoStr);
        trimCampo(titulo);
        Playlist playlist;
        playlist.codigo = atoi(codigoStr);
        strncpy(playlist.titulo, titulo, MAX_TITULO_PLAYLIST - 1);
        playlist.titulo[MAX_TITULO_PLAYLIST - 1] = '\0';
        playlist.cabeca = -1;
        playlist.fim = -1;
        playlist.esquerda = -1;
        playlist.direita = -1;
        if (inserirPlaylist(arquivoPlaylists, cabPlaylist, playlist)) {
            printf("Playlist %d cadastrada.\n", playlist.codigo);
        } else {
            printf("Nao foi possivel cadastrar a playlist %d.\n", playlist.codigo);
        }
        return 1;
    }
    if (strcmp(operacao, "I") == 0) {
        char *posicao = strtok(NULL, ";");
        char *codigoPlaylistStr = strtok(NULL, ";");
        char *codigoMusicaStr = strtok(NULL, ";");
        if (posicao == NULL || codigoPlaylistStr == NULL || codigoMusicaStr == NULL) {
            printf("Linha invalida para operacao I.\n");
            return 0;
        }
        trimCampo(posicao);
        trimCampo(codigoPlaylistStr);
        trimCampo(codigoMusicaStr);
        int codigoPlaylist = atoi(codigoPlaylistStr);
        int codigoMusica = atoi(codigoMusicaStr);
        int sucesso = 0;
        if (strcmp(posicao, "I") == 0) {
            sucesso = adicionarMusicaInicioPlaylist(arquivoMusicas, cabMusica, arquivoPlaylists, cabPlaylist, arquivoFaixas, cabFaixa, codigoPlaylist, codigoMusica);
        } else if (strcmp(posicao, "F") == 0) {
            sucesso = adicionarMusicaFimPlaylist(arquivoMusicas, cabMusica, arquivoPlaylists, cabPlaylist, arquivoFaixas, cabFaixa, codigoPlaylist, codigoMusica);
        } else {
            printf("Posicao invalida na operacao I.\n");
            return 0;
        }
        if (sucesso) {
            printf("Musica %d adicionada na playlist %d.\n", codigoMusica, codigoPlaylist);
        } else {
            printf("Nao foi possivel adicionar musica %d na playlist %d.\n", codigoMusica, codigoPlaylist);
        }
        return 1;
    }
    if (strcmp(operacao, "R") == 0) {
        char *codigoPlaylistStr = strtok(NULL, ";");
        char *codigoMusicaStr = strtok(NULL, ";");
        if (codigoPlaylistStr == NULL || codigoMusicaStr == NULL) {
            printf("Linha invalida para operacao R.\n");
            return 0;
        }
        trimCampo(codigoPlaylistStr);
        trimCampo(codigoMusicaStr);
        int codigoPlaylist = atoi(codigoPlaylistStr);
        int codigoMusica = atoi(codigoMusicaStr);
        if (removerMusicaDaPlaylist(arquivoPlaylists, cabPlaylist, arquivoFaixas, cabFaixa, codigoPlaylist, codigoMusica)) {
            printf("Musica %d removida da playlist %d.\n", codigoMusica, codigoPlaylist);
        } else {
            printf("Nao foi possivel remover musica %d da playlist %d.\n", codigoMusica, codigoPlaylist);
        }
        return 1;
    }
    if (strcmp(operacao, "D") == 0) {
        char *codigoMusicaStr = strtok(NULL, ";");
        if (codigoMusicaStr == NULL) {
            printf("Linha invalida para operacao D.\n");
            return 0;
        }
        trimCampo(codigoMusicaStr);
        int codigoMusica = atoi(codigoMusicaStr);
        if (removerMusicaDoSistema(arquivoMusicas, cabMusica, arquivoPlaylists, cabPlaylist, arquivoFaixas, cabFaixa, codigoMusica)) {
            printf("Musica %d removida do sistema.\n", codigoMusica);
        } else {
            printf("Nao foi possivel remover musica %d.\n", codigoMusica);
        }
        return 1;
    }
    printf("Operacao desconhecida: %s\n", operacao);
    return 0;
}

/* Propósito: Carrega e executa todos os comandos de um arquivo de lote
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos; nomeArquivo é string válida
 * Pós-condição: Todos os comandos do arquivo são executados sequencialmente; retorna 1 se sucesso ou 0 em erro */
int carregarArquivoLote(const char *nomeArquivo, FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa) {
    FILE *arquivo;
    char linha[MAX_LINHA];
    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de lote.\n");
        return 0;
    }
    while (fgets(linha, MAX_LINHA, arquivo) != NULL) {
        processarLinhaLote(linha, arquivoMusicas, cabMusica, arquivoPlaylists, cabPlaylist, arquivoFaixas, cabFaixa);
    }
    fclose(arquivo);
    return 1;
}

/* Propósito: Interação de menu para executar um arquivo de lote de operações
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos
 * Pós-condição: Lê o nome do arquivo do usuário e processa todas as operações em lote, exibindo mensagem de resultado */
void executarLote(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa) {
    char nomeArquivo[100];
    printf("\n=== EXECUTAR ARQUIVO DE LOTE ===\n");
    printf("Nome do arquivo: ");
    scanf("%99s", nomeArquivo);
    if (carregarArquivoLote(nomeArquivo, arquivoMusicas, cabMusica, arquivoPlaylists, cabPlaylist, arquivoFaixas, cabFaixa)) {
        printf("Arquivo de lote processado com sucesso!\n");
    } else {
        printf("Nao foi possivel processar o arquivo de lote.\n");
    }
}
