#include "playlist.h"
#include "musica.h"

/* Propósito: Abre o arquivo de playlists, criando-o se não existir
 * Pré-condição: nome é uma string válida e cabecalho aponta para estrutura válida
 * Pós-condição: Retorna ponteiro para arquivo aberto ou NULL em erro; cabeçalho é inicializado com raiz=-1 e topo=sizeof(CabecalhoPlaylist) */
FILE* abrirArquivoPlaylists(const char* nome, CabecalhoPlaylist* cabecalho) {
    FILE* arquivo = fopen(nome, "rb+");
    if (arquivo == NULL) {
        arquivo = fopen(nome, "wb+");
        if (arquivo == NULL) {
            return NULL;
        }
        cabecalho->raiz = -1;
        cabecalho->topo = sizeof(CabecalhoPlaylist);
        fseek(arquivo, 0, SEEK_SET);
        fwrite(cabecalho, sizeof(CabecalhoPlaylist), 1, arquivo);
        fflush(arquivo);
    } else {
        fseek(arquivo, 0, SEEK_SET);
        fread(cabecalho, sizeof(CabecalhoPlaylist), 1, arquivo);
    }
    return arquivo;
}

/* Propósito: Fecha o arquivo de playlists e grava o cabeçalho atualizado
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Cabeçalho é atualizado no arquivo e o arquivo é fechado */
void fecharArquivoPlaylists(FILE* arquivo, CabecalhoPlaylist* cabecalho) {
    if (arquivo != NULL) {
        fseek(arquivo, 0, SEEK_SET);
        fwrite(cabecalho, sizeof(CabecalhoPlaylist), 1, arquivo);
        fclose(arquivo);
    }
}

/* Propósito: Insere uma nova playlist na árvore binária de playlists
 * Pré-condição: arquivo e cabecalho são válidos; playlist contém código e título válidos
 * Pós-condição: Retorna 1 se inseriu ou 0 se código já existe; playlist é inserida mantendo ordenação; campos cabeca e fim são definidos como -1 */
int inserirPlaylist(FILE* arquivo, CabecalhoPlaylist* cabecalho, Playlist playlist) {
    if (buscarPlaylist(arquivo, cabecalho, playlist.codigo) != NULL) {
        return 0;
    }
    playlist.cabeca = -1;
    playlist.fim = -1;
    playlist.esquerda = -1;
    playlist.direita = -1;
    int posicao = cabecalho->topo;
    cabecalho->topo += sizeof(Playlist);
    fseek(arquivo, posicao, SEEK_SET);
    fwrite(&playlist, sizeof(Playlist), 1, arquivo);
    fflush(arquivo);
    if (cabecalho->raiz == -1) {
        cabecalho->raiz = posicao;
    } else {
        int atual = cabecalho->raiz;
        int anterior = -1;
        int filho = 0;
        while (atual != -1) {
            Playlist atualPlaylist;
            fseek(arquivo, atual, SEEK_SET);
            fread(&atualPlaylist, sizeof(Playlist), 1, arquivo);
            anterior = atual;
            if (playlist.codigo < atualPlaylist.codigo) {
                atual = atualPlaylist.esquerda;
                filho = 0;
            } else {
                atual = atualPlaylist.direita;
                filho = 1;
            }
        }
        Playlist pai;
        fseek(arquivo, anterior, SEEK_SET);
        fread(&pai, sizeof(Playlist), 1, arquivo);
        if (filho == 0) {
            pai.esquerda = posicao;
        } else {
            pai.direita = posicao;
        }
        fseek(arquivo, anterior, SEEK_SET);
        fwrite(&pai, sizeof(Playlist), 1, arquivo);
        fflush(arquivo);
    }
    return 1;
}

/* Propósito: Busca uma playlist pelo código na árvore binária
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Retorna ponteiro estático para playlist encontrada ou NULL se não existir; não modifica a árvore */
Playlist* buscarPlaylist(FILE* arquivo, CabecalhoPlaylist* cabecalho, int codigo) {
    static Playlist playlist;
    int atual = cabecalho->raiz;
    while (atual != -1) {
        fseek(arquivo, atual, SEEK_SET);
        fread(&playlist, sizeof(Playlist), 1, arquivo);
        if (codigo == playlist.codigo) {
            return &playlist;
        } else if (codigo < playlist.codigo) {
            atual = playlist.esquerda;
        } else {
            atual = playlist.direita;
        }
    }
    return NULL;
}

/* Propósito: Busca a posição no arquivo de uma playlist pelo código
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Retorna posição do registro ou -1 se não encontrada; não modifica a árvore */
int buscarPosicaoPlaylist(FILE* arquivo, CabecalhoPlaylist* cabecalho, int codigo) {
    int atual = cabecalho->raiz;
    Playlist playlist;
    while (atual != -1) {
        fseek(arquivo, atual, SEEK_SET);
        fread(&playlist, sizeof(Playlist), 1, arquivo);
        if (codigo == playlist.codigo) {
            return atual;
        } else if (codigo < playlist.codigo) {
            atual = playlist.esquerda;
        } else {
            atual = playlist.direita;
        }
    }
    return -1;
}

/* Propósito: Atualiza a cabeça e o fim da lista de faixas de uma playlist existente
 * Pré-condição: arquivo é válido; posicao é uma posição válida de playlist
 * Pós-condição: Retorna 1 se atualizou ou 0 se posicao inválida; campos cabeca e fim são sobrescritos; demais campos inalterados */
int atualizarListaFaixasPlaylist(FILE* arquivo, int posicao, int novaCabeca, int novoFim) {
    if (posicao == -1) {
        return 0;
    }
    Playlist playlist;
    fseek(arquivo, posicao, SEEK_SET);
    fread(&playlist, sizeof(Playlist), 1, arquivo);
    playlist.cabeca = novaCabeca;
    playlist.fim = novoFim;
    fseek(arquivo, posicao, SEEK_SET);
    fwrite(&playlist, sizeof(Playlist), 1, arquivo);
    fflush(arquivo);
    return 1;
}

/* Propósito: Lista todas as playlists cadastradas em ordem crescente de código
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Exibe código e título de todas as playlists em percurso in-order; não modifica a árvore */
void listarPlaylists(FILE* arquivo, CabecalhoPlaylist* cabecalho) {
    if (cabecalho->raiz == -1) {
        printf("Nenhuma playlist cadastrada.\n");
        return;
    }
    #define MAX_PILHA_PLAYLIST 1000
    int pilha[MAX_PILHA_PLAYLIST];
    int topo = -1;
    int atual = cabecalho->raiz;
    printf("\n=== LISTA DE PLAYLISTS ===\n");
    printf("%-8s | %-30s\n", "Codigo", "Titulo");
    printf("------------------------------------\n");
    while (atual != -1 || topo != -1) {
        while (atual != -1) {
            pilha[++topo] = atual;
            Playlist p;
            fseek(arquivo, atual, SEEK_SET);
            fread(&p, sizeof(Playlist), 1, arquivo);
            atual = p.esquerda;
        }
        if (topo != -1) {
            atual = pilha[topo--];
            Playlist p;
            fseek(arquivo, atual, SEEK_SET);
            fread(&p, sizeof(Playlist), 1, arquivo);
            printf("%-8d | %-30s\n", p.codigo, p.titulo);
            atual = p.direita;
        }
    }
    #undef MAX_PILHA_PLAYLIST
}

/* Propósito: Interação de menu para cadastrar uma nova playlist via entrada padrão
 * Pré-condição: arquivoPlaylists e cabPlaylist são válidos e abertos para escrita
 * Pós-condição: Lê os dados do usuário e insere a playlist no arquivo, exibindo mensagem de resultado */
void cadastrarPlaylist(FILE* arquivoPlaylists, CabecalhoPlaylist* cabPlaylist) {
    Playlist playlist;
    printf("\n=== CADASTRAR PLAYLIST ===\n");
    printf("Codigo: ");
    if (!lerInteiro(&playlist.codigo)) {
        printf("Codigo invalido.\n");
        return;
    }
    printf("Titulo: ");
    fgets(playlist.titulo, MAX_TITULO_PLAYLIST, stdin);
    playlist.titulo[strcspn(playlist.titulo, "\n")] = '\0';
    playlist.cabeca = -1;
    playlist.fim = -1;
    playlist.esquerda = -1;
    playlist.direita = -1;
    if (inserirPlaylist(arquivoPlaylists, cabPlaylist, playlist)) {
        printf("Playlist cadastrada com sucesso!\n");
    } else {
        printf("Nao foi possivel cadastrar a playlist.\n");
    }
}

/* Propósito: Interação de menu para listar todas as playlists cadastradas
 * Pré-condição: arquivoPlaylists e cabPlaylist são válidos
 * Pós-condição: Exibe todas as playlists armazenadas */
void listarTodasPlaylists(FILE* arquivoPlaylists, CabecalhoPlaylist* cabPlaylist) {
    listarPlaylists(arquivoPlaylists, cabPlaylist);
}
