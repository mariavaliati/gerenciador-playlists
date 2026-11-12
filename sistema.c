#include "sistema.h"

/* Propósito: Adiciona uma música existente no início de uma playlist
 * Pré-condição: Arquivos e cabeçalhos são válidos e abertos; playlist e música existem
 * Pós-condição: Música é inserida como primeira faixa; campos cabeca e fim são atualizados; retorna 1 se sucesso ou 0 em erro */
int adicionarMusicaInicioPlaylist(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa, int codigoPlaylist, int codigoMusica) {
    Playlist *playlist;
    int posicaoPlaylist;
    int novaCabeca;
    int novoFim;
    if (buscarMusica(arquivoMusicas, cabMusica, codigoMusica) == NULL) {
        printf("Musica %d nao existe no acervo.\n", codigoMusica);
        return 0;
    }
    playlist = buscarPlaylist(arquivoPlaylists, cabPlaylist, codigoPlaylist);
    if (playlist == NULL) {
        printf("Playlist %d nao existe.\n", codigoPlaylist);
        return 0;
    }
    novaCabeca = playlist->cabeca;
    novoFim = playlist->fim;
    if (!inserirFaixaInicio(arquivoFaixas, cabFaixa, &novaCabeca, &novoFim, codigoMusica)) {
        return 0;
    }
    posicaoPlaylist = buscarPosicaoPlaylist(arquivoPlaylists, cabPlaylist, codigoPlaylist);
    if (!atualizarListaFaixasPlaylist(arquivoPlaylists, posicaoPlaylist, novaCabeca, novoFim)) {
        return 0;
    }
    return 1;
}

/* Propósito: Adiciona uma música existente no final de uma playlist
 * Pré-condição: Arquivos e cabeçalhos são válidos e abertos; playlist e música existem
 * Pós-condição: Música é inserida como última faixa; campos cabeca e fim são atualizados; retorna 1 se sucesso ou 0 em erro */
int adicionarMusicaFimPlaylist(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa, int codigoPlaylist, int codigoMusica) {
    Playlist *playlist;
    int posicaoPlaylist;
    int novaCabeca;
    int novoFim;
    if (buscarMusica(arquivoMusicas, cabMusica, codigoMusica) == NULL) {
        printf("Musica %d nao existe no acervo.\n", codigoMusica);
        return 0;
    }
    playlist = buscarPlaylist(arquivoPlaylists, cabPlaylist, codigoPlaylist);
    if (playlist == NULL) {
        printf("Playlist %d nao existe.\n", codigoPlaylist);
        return 0;
    }
    novaCabeca = playlist->cabeca;
    novoFim = playlist->fim;
    if (!inserirFaixaFim(arquivoFaixas, cabFaixa, &novaCabeca, &novoFim, codigoMusica)) {
        return 0;
    }
    posicaoPlaylist = buscarPosicaoPlaylist(arquivoPlaylists, cabPlaylist, codigoPlaylist);
    if (!atualizarListaFaixasPlaylist(arquivoPlaylists, posicaoPlaylist, novaCabeca, novoFim)) {
        return 0;
    }
    return 1;
}

/* Propósito: Remove a primeira ocorrência de uma música de uma playlist
 * Pré-condição: Arquivos de playlists e faixas são válidos e abertos; playlist existe
 * Pós-condição: Primeira ocorrência da música é removida; nó vai para lista de livres; cabeca e fim são atualizados; retorna 1 se sucesso ou 0 em erro */
int removerMusicaDaPlaylist(FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa, int codigoPlaylist, int codigoMusica) {
    Playlist *playlist;
    int posicaoPlaylist;
    int novaCabeca;
    int novoFim;
    playlist = buscarPlaylist(arquivoPlaylists, cabPlaylist, codigoPlaylist);
    if (playlist == NULL) {
        printf("Playlist %d nao existe.\n", codigoPlaylist);
        return 0;
    }
    novaCabeca = playlist->cabeca;
    novoFim = playlist->fim;
    if (removerFaixa(arquivoFaixas, cabFaixa, &novaCabeca, &novoFim, codigoMusica) == -1) {
        printf("Musica %d nao esta na playlist %d.\n", codigoMusica, codigoPlaylist);
        return 0;
    }
    posicaoPlaylist = buscarPosicaoPlaylist(arquivoPlaylists, cabPlaylist, codigoPlaylist);
    if (!atualizarListaFaixasPlaylist(arquivoPlaylists, posicaoPlaylist, novaCabeca, novoFim)) {
        return 0;
    }
    return 1;
}

/* Propósito: Remove todas as ocorrências de uma música de uma playlist
 * Pré-condição: Arquivos de playlists e faixas são válidos e abertos; playlist existe
 * Pós-condição: Todas as ocorrências são removidas; nós vão para lista de livres; retorna quantidade de ocorrências removidas */
static int removerTodasDaPlaylist(FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa, int codigoPlaylist, int codigoMusica) {
    Playlist *playlist;
    int cabeca;
    int fim;
    int quantidade = 0;
    playlist = buscarPlaylist(arquivoPlaylists, cabPlaylist, codigoPlaylist);
    if (playlist == NULL) {
        return 0;
    }
    cabeca = playlist->cabeca;
    fim = playlist->fim;
    while (removerFaixa(arquivoFaixas, cabFaixa, &cabeca, &fim, codigoMusica) != -1) {
        quantidade++;
    }
    if (quantidade > 0) {
        int posicao = buscarPosicaoPlaylist(arquivoPlaylists, cabPlaylist, codigoPlaylist);
        atualizarListaFaixasPlaylist(arquivoPlaylists, posicao, cabeca, fim);
    }
    return quantidade;
}

/* Propósito: Percorre recursivamente a árvore de playlists removendo uma música de todas as playlists
 * Pré-condição: Arquivos e cabeçalhos são válidos e abertos
 * Pós-condição: Música é removida de todas as playlists onde aparece */
static void removerMusicaDasPlaylistsRec(FILE *arquivoPlaylists, int posicao, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa, int codigoMusica) {
    Playlist playlist;
    if (posicao == -1) {
        return;
    }
    fseek(arquivoPlaylists, posicao, SEEK_SET);
    if (fread(&playlist, sizeof(Playlist), 1, arquivoPlaylists) != 1) {
        return;
    }
    removerMusicaDasPlaylistsRec(arquivoPlaylists, playlist.esquerda, cabPlaylist, arquivoFaixas, cabFaixa, codigoMusica);
    removerTodasDaPlaylist(arquivoPlaylists, cabPlaylist, arquivoFaixas, cabFaixa, playlist.codigo, codigoMusica);
    removerMusicaDasPlaylistsRec(arquivoPlaylists, playlist.direita, cabPlaylist, arquivoFaixas, cabFaixa, codigoMusica);
}

/* Propósito: Remove uma música do acervo e de todas as playlists que a contêm
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos; música existe
 * Pós-condição: Música é removida da árvore do acervo; todas as ocorrências são removidas das playlists; nós removidos vão para listas de livres; retorna 1 se sucesso ou 0 em erro */
int removerMusicaDoSistema(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa, int codigoMusica) {
    if (buscarMusica(arquivoMusicas, cabMusica, codigoMusica) == NULL) {
        printf("Musica %d nao existe no acervo.\n", codigoMusica);
        return 0;
    }
    removerMusicaDasPlaylistsRec(arquivoPlaylists, cabPlaylist->raiz, cabPlaylist, arquivoFaixas, cabFaixa, codigoMusica);
    if (!removerMusicaArquivo(arquivoMusicas, cabMusica, codigoMusica)) {
        return 0;
    }
    return 1;
}

/* Propósito: Imprime uma playlist completa com título e todas as músicas na ordem da lista
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos; playlist existe
 * Pós-condição: Exibe título da playlist e dados de cada música; nenhum arquivo é modificado */
void imprimirPlaylistCompleta(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, int codigoPlaylist) {
    Playlist *playlist;
    int atual;
    playlist = buscarPlaylist(arquivoPlaylists, cabPlaylist, codigoPlaylist);
    if (playlist == NULL) {
        printf("Playlist %d nao existe.\n", codigoPlaylist);
        return;
    }
    printf("\n PLAYLIST \n");
    printf("Codigo: %d\n", playlist->codigo);
    printf("Titulo: %s\n\n", playlist->titulo);
    if (playlist->cabeca == -1) {
        printf("Playlist vazia.\n");
        return;
    }
    atual = playlist->cabeca;
    while (atual != -1) {
        FaixaMusical faixa;
        Musica *musica;
        fseek(arquivoFaixas, atual, SEEK_SET);
        if (fread(&faixa, sizeof(FaixaMusical), 1, arquivoFaixas) != 1) {
            printf("Erro ao ler faixa.\n");
            return;
        }
        musica = buscarMusica(arquivoMusicas, cabMusica, faixa.codigoMusica);
        if (musica != NULL) {
            printf("%s - %s\n", musica->titulo, musica->artista);
        } else {
            printf("[Musica %d nao encontrada no acervo]\n", faixa.codigoMusica);
        }
        atual = faixa.proximo;
    }
}

/* Propósito: Interação de menu para adicionar uma música do acervo no início de uma playlist
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos
 * Pós-condição: Lê os códigos do usuário e insere a música no início da playlist, exibindo mensagem de resultado */
void adicionarMusicaInicio(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa) {
    int codigoPlaylist;
    int codigoMusica;
    printf("\n=== ADICIONAR MUSICA NO INICIO ===\n");
    printf("Codigo da playlist: ");
    if (!lerInteiro(&codigoPlaylist)) {
        printf("Codigo invalido.\n");
        return;
    }
    printf("Codigo da musica: ");
    if (!lerInteiro(&codigoMusica)) {
        printf("Codigo invalido.\n");
        return;
    }
    if (adicionarMusicaInicioPlaylist(arquivoMusicas, cabMusica, arquivoPlaylists, cabPlaylist, arquivoFaixas, cabFaixa, codigoPlaylist, codigoMusica)) {
        printf("Musica adicionada ao inicio da playlist!\n");
    } else {
        printf("Nao foi possivel adicionar a musica.\n");
    }
}

/* Propósito: Interação de menu para adicionar uma música do acervo no final de uma playlist
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos
 * Pós-condição: Lê os códigos do usuário e insere a música no final da playlist, exibindo mensagem de resultado */
void adicionarMusicaFinal(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa) {
    int codigoPlaylist;
    int codigoMusica;
    printf("\n=== ADICIONAR MUSICA NO FINAL ===\n");
    printf("Codigo da playlist: ");
    if (!lerInteiro(&codigoPlaylist)) {
        printf("Codigo invalido.\n");
        return;
    }
    printf("Codigo da musica: ");
    if (!lerInteiro(&codigoMusica)) {
        printf("Codigo invalido.\n");
        return;
    }
    if (adicionarMusicaFimPlaylist(arquivoMusicas, cabMusica, arquivoPlaylists, cabPlaylist, arquivoFaixas, cabFaixa, codigoPlaylist, codigoMusica)) {
        printf("Musica adicionada ao final da playlist!\n");
    } else {
        printf("Nao foi possivel adicionar a musica.\n");
    }
}

/* Propósito: Interação de menu para remover uma música de uma playlist
 * Pré-condição: arquivoPlaylists, cabPlaylist, arquivoFaixas e cabFaixa são válidos
 * Pós-condição: Lê os códigos do usuário e remove a música da playlist, exibindo mensagem de resultado */
void removerMusicaPlaylist(FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa) {
    int codigoPlaylist;
    int codigoMusica;
    printf("\n=== REMOVER MUSICA DA PLAYLIST ===\n");
    printf("Codigo da playlist: ");
    if (!lerInteiro(&codigoPlaylist)) {
        printf("Codigo invalido.\n");
        return;
    }
    printf("Codigo da musica: ");
    if (!lerInteiro(&codigoMusica)) {
        printf("Codigo invalido.\n");
        return;
    }
    if (removerMusicaDaPlaylist(arquivoPlaylists, cabPlaylist, arquivoFaixas, cabFaixa, codigoPlaylist, codigoMusica)) {
        printf("Musica removida da playlist!\n");
    } else {
        printf("Nao foi possivel remover a musica.\n");
    }
}

/* Propósito: Interação de menu para exibir o conteúdo completo de uma playlist
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos
 * Pós-condição: Lê o código da playlist do usuário e exibe seus dados e músicas */
void imprimirPlaylist(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas) {
    int codigoPlaylist;
    printf("\n=== IMPRIMIR PLAYLIST ===\n");
    printf("Codigo da playlist: ");
    if (!lerInteiro(&codigoPlaylist)) {
        printf("Codigo invalido.\n");
        return;
    }
    imprimirPlaylistCompleta(arquivoMusicas, cabMusica, arquivoPlaylists, cabPlaylist, arquivoFaixas, codigoPlaylist);
}

/* Propósito: Interação de menu para remover uma música do sistema
 * Pré-condição: Todos os arquivos e cabeçalhos são válidos e abertos
 * Pós-condição: Lê o código do usuário e remove a música do acervo e de todas as playlists, exibindo mensagem de resultado */
void removerMusica(FILE *arquivoMusicas, CabecalhoMusica *cabMusica, FILE *arquivoPlaylists, CabecalhoPlaylist *cabPlaylist, FILE *arquivoFaixas, CabecalhoFaixa *cabFaixa) {
    int codigo;
    printf("\n=== REMOVER MUSICA ===\n");
    printf("Digite o codigo da musica: ");
    if (!lerInteiro(&codigo)) {
        printf("Codigo invalido.\n");
        return;
    }
    if (removerMusicaDoSistema(arquivoMusicas, cabMusica, arquivoPlaylists, cabPlaylist, arquivoFaixas, cabFaixa, codigo)) {
        printf("Musica removida com sucesso!\n");
    } else {
        printf("Nao foi possivel remover a musica.\n");
    }
}
