#include "faixa.h"

/* Propósito: Abre o arquivo de faixas, criando-o se não existir
 * Pré-condição: nome é uma string válida e cabecalho aponta para estrutura válida
 * Pós-condição: Retorna ponteiro para arquivo aberto ou NULL em caso de erro; cabeçalho é inicializado */
FILE* abrirArquivoFaixas(const char* nome, CabecalhoFaixa* cabecalho) {
    FILE* arquivo = fopen(nome, "rb+");
    if (arquivo == NULL) {
        arquivo = fopen(nome, "wb+");
        if (arquivo == NULL) {
            return NULL;
        }
        cabecalho->topo = sizeof(CabecalhoFaixa);
        cabecalho->livre = -1;
        fseek(arquivo, 0, SEEK_SET);
        fwrite(cabecalho, sizeof(CabecalhoFaixa), 1, arquivo);
        fflush(arquivo);
    } else {
        fseek(arquivo, 0, SEEK_SET);
        fread(cabecalho, sizeof(CabecalhoFaixa), 1, arquivo);
    }
    return arquivo;
}

/* Propósito: Fecha o arquivo de faixas e grava o cabeçalho atualizado
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Cabeçalho é atualizado no arquivo e o arquivo é fechado */
void fecharArquivoFaixas(FILE* arquivo, CabecalhoFaixa* cabecalho) {
    if (arquivo != NULL) {
        fseek(arquivo, 0, SEEK_SET);
        fwrite(cabecalho, sizeof(CabecalhoFaixa), 1, arquivo);
        fclose(arquivo);
    }
}

/* Propósito: Cria uma nova faixa musical no arquivo
 * Pré-condição: arquivo e cabecalho são válidos; codigoMusica é válido
 * Pós-condição: Retorna posição da nova faixa ou -1 em erro; faixa é criada com proximo=-1; reutiliza nó livre se disponível */
int criarFaixa(FILE* arquivo, CabecalhoFaixa* cabecalho, int codigoMusica) {
    int posicao;
    if (cabecalho->livre != -1) {
        posicao = cabecalho->livre;
        FaixaMusical livre;
        fseek(arquivo, posicao, SEEK_SET);
        fread(&livre, sizeof(FaixaMusical), 1, arquivo);
        cabecalho->livre = livre.proximo;
    } else {
        posicao = cabecalho->topo;
        cabecalho->topo += sizeof(FaixaMusical);
    }
    FaixaMusical novaFaixa;
    novaFaixa.codigoMusica = codigoMusica;
    novaFaixa.proximo = -1;
    fseek(arquivo, posicao, SEEK_SET);
    fwrite(&novaFaixa, sizeof(FaixaMusical), 1, arquivo);
    fflush(arquivo);
    return posicao;
}

/* Propósito: Busca uma faixa musical pelo código da música na lista
 * Pré-condição: arquivo é válido; cabeca é posição inicial da lista
 * Pós-condição: Retorna ponteiro para faixa encontrada (estática) ou NULL; não modifica a lista */
FaixaMusical* buscarFaixa(FILE* arquivo, int cabeca, int codigoMusica) {
    static FaixaMusical faixa;
    int atual = cabeca;
    while (atual != -1) {
        fseek(arquivo, atual, SEEK_SET);
        fread(&faixa, sizeof(FaixaMusical), 1, arquivo);
        if (faixa.codigoMusica == codigoMusica) {
            return &faixa;
        }
        atual = faixa.proximo;
    }
    return NULL;
}

/* Propósito: Remove uma faixa musical da lista pelo código da música
 * Pré-condição: arquivo e cabecalho são válidos; cabeca e fim são ponteiros válidos
 * Pós-condição: Retorna posição da faixa removida ou -1 se não encontrada; adiciona à lista de livres; atualiza ponteiros da lista */
int removerFaixa(FILE* arquivo, CabecalhoFaixa* cabecalho, int* cabeca, int* fim, int codigoMusica) {
    if (*cabeca == -1) {
        return -1;
    }
    int atual = *cabeca;
    int anterior = -1;
    FaixaMusical faixaAtual;
    while (atual != -1) {
        fseek(arquivo, atual, SEEK_SET);
        fread(&faixaAtual, sizeof(FaixaMusical), 1, arquivo);
        if (faixaAtual.codigoMusica == codigoMusica) {
            break;
        }
        anterior = atual;
        atual = faixaAtual.proximo;
    }
    if (atual == -1) {
        return -1;
    }
    if (anterior == -1) {
        *cabeca = faixaAtual.proximo;
        if (*cabeca == -1) {
            *fim = -1;
        }
    } else {
        FaixaMusical faixaAnterior;
        fseek(arquivo, anterior, SEEK_SET);
        fread(&faixaAnterior, sizeof(FaixaMusical), 1, arquivo);
        faixaAnterior.proximo = faixaAtual.proximo;
        fseek(arquivo, anterior, SEEK_SET);
        fwrite(&faixaAnterior, sizeof(FaixaMusical), 1, arquivo);
        fflush(arquivo);
        if (faixaAtual.proximo == -1) {
            *fim = anterior;
        }
    }
    faixaAtual.proximo = cabecalho->livre;
    cabecalho->livre = atual;
    fseek(arquivo, atual, SEEK_SET);
    fwrite(&faixaAtual, sizeof(FaixaMusical), 1, arquivo);
    fflush(arquivo);
    return atual;
}

/* Propósito: Insere uma faixa no início da lista
 * Pré-condição: arquivo e cabecalho são válidos; cabeca e fim são ponteiros válidos
 * Pós-condição: Retorna 1 se inseriu ou 0 em erro; nova faixa torna-se cabeça; atualiza ponteiros cabeca e fim */
int inserirFaixaInicio(FILE* arquivo, CabecalhoFaixa* cabecalho, int* cabeca, int* fim, int codigoMusica) {
    int posicao = criarFaixa(arquivo, cabecalho, codigoMusica);
    if (posicao == -1) {
        return 0;
    }
    FaixaMusical novaFaixa;
    fseek(arquivo, posicao, SEEK_SET);
    fread(&novaFaixa, sizeof(FaixaMusical), 1, arquivo);
    novaFaixa.proximo = *cabeca;
    fseek(arquivo, posicao, SEEK_SET);
    fwrite(&novaFaixa, sizeof(FaixaMusical), 1, arquivo);
    fflush(arquivo);
    if (*cabeca == -1) {
        *fim = posicao;
    }
    *cabeca = posicao;
    return 1;
}

/* Propósito: Insere uma faixa no final da lista
 * Pré-condição: arquivo e cabecalho são válidos; cabeca e fim são ponteiros válidos
 * Pós-condição: Retorna 1 se inseriu ou 0 em erro; nova faixa torna-se fim; atualiza ponteiros cabeca e fim */
int inserirFaixaFim(FILE* arquivo, CabecalhoFaixa* cabecalho, int* cabeca, int* fim, int codigoMusica) {
    int posicao = criarFaixa(arquivo, cabecalho, codigoMusica);
    if (posicao == -1) {
        return 0;
    }
    if (*cabeca == -1) {
        *cabeca = posicao;
        *fim = posicao;
    } else {
        FaixaMusical ultima;
        fseek(arquivo, *fim, SEEK_SET);
        fread(&ultima, sizeof(FaixaMusical), 1, arquivo);
        ultima.proximo = posicao;
        fseek(arquivo, *fim, SEEK_SET);
        fwrite(&ultima, sizeof(FaixaMusical), 1, arquivo);
        fflush(arquivo);
        *fim = posicao;
    }
    return 1;
}

/* Propósito: Imprime todas as faixas de uma lista
 * Pré-condição: arquivo é válido
 * Pós-condição: Exibe os códigos das músicas na ordem da lista */
void imprimirListaFaixas(FILE* arquivo, int cabeca) {
    if (cabeca == -1) {
        printf("Lista vazia.\n");
        return;
    }
    int atual = cabeca;
    int posicao = 1;
    printf("Faixas da playlist:\n");
    while (atual != -1) {
        FaixaMusical faixa;
        fseek(arquivo, atual, SEEK_SET);
        fread(&faixa, sizeof(FaixaMusical), 1, arquivo);
        printf("%d. Codigo da musica: %d (posicao: %d)\n", posicao++, faixa.codigoMusica, atual);
        atual = faixa.proximo;
    }
}

/* Propósito: Imprime os nós livres disponíveis no arquivo de faixas
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Exibe todas as posições dos nós livres disponíveis */
void imprimirNosLivresFaixas(FILE* arquivo, CabecalhoFaixa* cabecalho) {
    if (cabecalho->livre == -1) {
        printf("Nenhum no livre disponivel.\n");
        return;
    }
    printf("\n=== NOS LIVRES DE FAIXAS ===\n");
    int atual = cabecalho->livre;
    int count = 0;
    while (atual != -1) {
        printf("Posicao: %d\n", atual);
        count++;
        FaixaMusical f;
        fseek(arquivo, atual, SEEK_SET);
        fread(&f, sizeof(FaixaMusical), 1, arquivo);
        atual = f.proximo;
    }
    printf("Total: %d nos livres disponiveis\n", count);
}

/* Propósito: Verifica se uma música existe em uma playlist
 * Pré-condição: arquivo é válido
 * Pós-condição: Retorna 1 se a música está na playlist ou 0 caso contrário */
int faixaExisteNaPlaylist(FILE* arquivo, int cabeca, int codigoMusica) {
    return buscarFaixa(arquivo, cabeca, codigoMusica) != NULL;
}

/* Propósito: Conta o número de faixas em uma playlist
 * Pré-condição: arquivo é válido
 * Pós-condição: Retorna o número de faixas na playlist */
int contarFaixas(FILE* arquivo, int cabeca) {
    int count = 0;
    int atual = cabeca;
    while (atual != -1) {
        count++;
        FaixaMusical faixa;
        fseek(arquivo, atual, SEEK_SET);
        fread(&faixa, sizeof(FaixaMusical), 1, arquivo);
        atual = faixa.proximo;
    }
    return count;
}
