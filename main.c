#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "musica.h"
#include "playlist.h"
#include "faixa.h"
#include "lote.h"
#include "sistema.h"

/* Propósito: Função principal que gerencia o menu interativo do sistema
 * Pré-condição: Arquivos do sistema são abertos com sucesso
 * Pós-condição: Sistema executa operações conforme escolha do usuário e fecha arquivos */
int main(void) {
    FILE *arquivoMusicas;
    FILE *arquivoPlaylists;
    FILE *arquivoFaixas;

    CabecalhoMusica cabMusica;
    CabecalhoPlaylist cabPlaylist;
    CabecalhoFaixa cabFaixa;

    int opcao;

    arquivoMusicas = abrirArquivoMusicas("musica.bin", &cabMusica);
    arquivoPlaylists = abrirArquivoPlaylists("playlist.bin", &cabPlaylist);
    arquivoFaixas = abrirArquivoFaixas("faixa.bin", &cabFaixa);

    if (arquivoMusicas == NULL || arquivoPlaylists == NULL || arquivoFaixas == NULL) {
        printf("Erro ao abrir os arquivos do sistema.\n");
        if (arquivoMusicas != NULL)
            fecharArquivoMusicas(arquivoMusicas, &cabMusica);

        if (arquivoPlaylists != NULL)
            fecharArquivoPlaylists(arquivoPlaylists, &cabPlaylist);

        if (arquivoFaixas != NULL)
            fecharArquivoFaixas(arquivoFaixas, &cabFaixa);

        return 1;
    }
    do {
        printf("\n");
        printf("       SISTEMA DE PLAYLISTS\n");
        printf("----------------------------------------\n");

        printf("1  - Cadastrar musica\n");
        printf("2  - Imprimir dados da musica\n");
        printf("3  - Listar acervo\n");
        printf("4  - Remover musica\n");
        printf("5  - Cadastrar playlist\n");
        printf("6  - Listar playlists\n");
        printf("7  - Imprimir playlist completa\n");
        printf("8  - Adicionar musica no inicio\n");
        printf("9  - Adicionar musica no final\n");
        printf("10 - Remover musica da playlist\n");
        printf("11 - Executar arquivo de lote\n");
        printf("12 - Imprimir arvore do acervo por niveis\n");
        printf("13 - Imprimir nos livres do acervo\n");
        printf("14 - Imprimir nos livres de playlists\n");
        printf("0  - Sair\n");
        printf("----------------------------------------\n");
        printf("Opcao: ");
        if (!lerInteiro(&opcao)) {
            printf("\nOpcao invalida.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                cadastrarMusica(arquivoMusicas, &cabMusica);
                break;

            case 2:
                imprimirDadosMusica(arquivoMusicas, &cabMusica);
                break;

            case 3:
                listarAcervo(arquivoMusicas, &cabMusica);
                break;

            case 4:
                removerMusica(arquivoMusicas, &cabMusica, arquivoPlaylists, &cabPlaylist, arquivoFaixas, &cabFaixa);
                break;

            case 5:
                cadastrarPlaylist(arquivoPlaylists, &cabPlaylist);
                break;

            case 6:
                listarTodasPlaylists(arquivoPlaylists, &cabPlaylist);
                break;

            case 7:
                imprimirPlaylist(arquivoMusicas, &cabMusica, arquivoPlaylists, &cabPlaylist, arquivoFaixas);
                break;

            case 8:
                adicionarMusicaInicio(arquivoMusicas, &cabMusica, arquivoPlaylists, &cabPlaylist, arquivoFaixas, &cabFaixa);
                break;

            case 9:
                adicionarMusicaFinal(arquivoMusicas, &cabMusica, arquivoPlaylists, &cabPlaylist, arquivoFaixas, &cabFaixa);
                break;

            case 10:
                removerMusicaPlaylist(arquivoPlaylists, &cabPlaylist, arquivoFaixas, &cabFaixa);
                break;

            case 11:
                executarLote(arquivoMusicas, &cabMusica, arquivoPlaylists, &cabPlaylist, arquivoFaixas, &cabFaixa);
                break;

            case 12:
                imprimirArvorePorNiveis(arquivoMusicas, &cabMusica);
                break;

            case 13:
                imprimirNosLivresMusicas(arquivoMusicas, &cabMusica);
                break;

            case 14:
                imprimirNosLivresFaixas(arquivoFaixas, &cabFaixa);
                break;

            case 0:
                printf("\nEncerrando sistema...\n");
                break;
            default:
                printf("\nOpcao invalida.\n");
        }
    } while (opcao != 0);

    fecharArquivoMusicas(arquivoMusicas, &cabMusica);
    fecharArquivoPlaylists(arquivoPlaylists, &cabPlaylist);
    fecharArquivoFaixas(arquivoFaixas, &cabFaixa);
    return 0;
}
