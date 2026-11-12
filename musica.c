#include "musica.h"

/* Propósito: Remove espaços em branco das extremidades de uma string
 * Pré-condição: str é uma string válida
 * Pós-condição: String é modificada removendo espaços no início e fim */
void trimString(char *str) {
    if (str == NULL || str[0] == '\0') {
        return;
    }
    char *inicio = str;
    char *fim = str + strlen(str) - 1;
    while (isspace((unsigned char)*inicio)) {
        inicio++;
    }
    while (fim > inicio && isspace((unsigned char)*fim)) {
        fim--;
    }
    if (inicio != str) {
        memmove(str, inicio, fim - inicio + 1);
    }
    str[fim - inicio + 1] = '\0';
}

/* Propósito: Lê um número inteiro da entrada padrão de forma segura
 * Pré-condição: valor aponta para um inteiro válido
 * Pós-condição: Retorna 1 e armazena o inteiro lido em valor, ou retorna 0 se a entrada não for numérica; buffer de entrada é limpo até o fim da linha em qualquer caso */
int lerInteiro(int *valor) {
    int resultado = scanf("%d", valor);
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    return resultado == 1;
}

/* Propósito: Abre o arquivo de músicas, criando-o se não existir
 * Pré-condição: nome é uma string válida e cabecalho aponta para estrutura válida
 * Pós-condição: Retorna o arquivo aberto ou NULL em caso de erro; cabeçalho é inicializado */
FILE *abrirArquivoMusicas(const char *nome, CabecalhoMusica *cabecalho) {
    FILE *arquivo;
    arquivo = fopen(nome, "rb+");
    if (arquivo == NULL) {
        arquivo = fopen(nome, "wb+");
        if (arquivo == NULL) {
            return NULL;
        }
        cabecalho->raiz = -1;
        cabecalho->topo = sizeof(CabecalhoMusica);
        cabecalho->livre = -1;
        fseek(arquivo, 0, SEEK_SET);
        fwrite(cabecalho, sizeof(CabecalhoMusica), 1, arquivo);
        fflush(arquivo);
    } else {
        fseek(arquivo, 0, SEEK_SET);
        fread(cabecalho, sizeof(CabecalhoMusica), 1, arquivo);
    }
    return arquivo;
}

/* Propósito: Fecha o arquivo de músicas e grava o cabeçalho atualizado
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Cabeçalho é atualizado no arquivo e o arquivo é fechado */
void fecharArquivoMusicas(FILE *arquivo, CabecalhoMusica *cabecalho) {
    if (arquivo == NULL) {
        return;
    }
    fseek(arquivo, 0, SEEK_SET);
    fwrite(cabecalho, sizeof(CabecalhoMusica), 1, arquivo);
    fflush(arquivo);
    fclose(arquivo);
}

/* Propósito: Insere uma música na árvore binária ordenada por código
 * Pré-condição: arquivo e cabecalho são válidos; musica contém dados válidos
 * Pós-condição: Retorna 1 se inseriu ou 0 se código já existe; árvore mantém ordenação */
int inserirMusica(FILE *arquivo, CabecalhoMusica *cabecalho, Musica musica) {
    if (buscarMusica(arquivo, cabecalho, musica.codigo) != NULL) {
        return 0;
    }
    int posicao;
    if (cabecalho->livre != -1) {
        posicao = cabecalho->livre;
        Musica livre;
        fseek(arquivo, posicao, SEEK_SET);
        fread(&livre, sizeof(Musica), 1, arquivo);
        cabecalho->livre = livre.esquerda;
    } else {
        posicao = cabecalho->topo;
        cabecalho->topo += sizeof(Musica);
    }
    musica.esquerda = -1;
    musica.direita = -1;
    fseek(arquivo, posicao, SEEK_SET);
    fwrite(&musica, sizeof(Musica), 1, arquivo);
    fflush(arquivo);
    if (cabecalho->raiz == -1) {
        cabecalho->raiz = posicao;
        return 1;
    }
    int atual = cabecalho->raiz;
    int anterior = -1;
    int filho = 0;
    while (atual != -1) {
        Musica atualMusica;
        fseek(arquivo, atual, SEEK_SET);
        fread(&atualMusica, sizeof(Musica), 1, arquivo);
        anterior = atual;
        if (musica.codigo < atualMusica.codigo) {
            atual = atualMusica.esquerda;
            filho = 0;
        } else {
            atual = atualMusica.direita;
            filho = 1;
        }
    }
    Musica pai;
    fseek(arquivo, anterior, SEEK_SET);
    fread(&pai, sizeof(Musica), 1, arquivo);
    if (filho == 0) {
        pai.esquerda = posicao;
    } else {
        pai.direita = posicao;
    }
    fseek(arquivo, anterior, SEEK_SET);
    fwrite(&pai, sizeof(Musica), 1, arquivo);
    fflush(arquivo);
    return 1;
}

/* Propósito: Busca uma música pelo código na árvore binária
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Retorna ponteiro para música encontrada ou NULL se não encontrada */
Musica *buscarMusica(FILE *arquivo, CabecalhoMusica *cabecalho, int codigo) {
    static Musica musica;
    int atual = cabecalho->raiz;
    while (atual != -1) {
        fseek(arquivo, atual, SEEK_SET);
        fread(&musica, sizeof(Musica), 1, arquivo);
        if (codigo == musica.codigo) {
            return &musica;
        }
        if (codigo < musica.codigo) {
            atual = musica.esquerda;
        } else {
            atual = musica.direita;
        }
    }
    return NULL;
}

/* Propósito: Lista todas as músicas do acervo em ordem crescente de código
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Exibe todas as músicas em percurso in-order sem modificar a árvore */
void listarMusicas(FILE *arquivo, CabecalhoMusica *cabecalho) {
    if (cabecalho->raiz == -1) {
        printf("\nNenhuma musica cadastrada.\n");
        return;
    }
    int pilha[1000];
    int topoPilha = -1;
    int atual = cabecalho->raiz;
    printf("\n=== ACERVO DE MUSICAS ===\n");
    printf("%-8s | %-30s | %-30s | %-4s\n", "Codigo", "Titulo", "Artista", "Ano");
    printf("--------------------------------------------------------------------------------\n");
    while (atual != -1 || topoPilha != -1) {
        while (atual != -1) {
            pilha[++topoPilha] = atual;
            Musica musica;
            fseek(arquivo, atual, SEEK_SET);
            fread(&musica, sizeof(Musica), 1, arquivo);
            atual = musica.esquerda;
        }
        atual = pilha[topoPilha--];
        Musica musica;
        fseek(arquivo, atual, SEEK_SET);
        fread(&musica, sizeof(Musica), 1, arquivo);
        printf("%-8d | %-30s | %-30s | %-4d\n", musica.codigo, musica.titulo, musica.artista, musica.ano);
        atual = musica.direita;
    }
}

/* Propósito: Imprime a árvore binária por níveis
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Exibe os códigos por nível sem modificar a árvore */
void imprimirArvorePorNiveis(FILE *arquivo, CabecalhoMusica *cabecalho) {
    if (cabecalho->raiz == -1) {
        printf("\nArvore vazia.\n");
        return;
    }
    int fila[1000];
    int inicio = 0;
    int fim = 0;
    fila[fim++] = cabecalho->raiz;
    int nivel = 0;
    printf("\n=== ARVORE POR NIVEIS ===\n");
    while (inicio < fim) {
        int nivelFim = fim;
        printf("Nivel %d: ", nivel);
        while (inicio < nivelFim) {
            int posicao = fila[inicio++];
            Musica musica;
            fseek(arquivo, posicao, SEEK_SET);
            fread(&musica, sizeof(Musica), 1, arquivo);
            printf("%d ", musica.codigo);
            if (musica.esquerda != -1) {
                fila[fim++] = musica.esquerda;
            }
            if (musica.direita != -1) {
                fila[fim++] = musica.direita;
            }
        }
        printf("\n");
        nivel++;
    }
}

/* Propósito: Remove uma música da árvore binária pelo código
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Retorna 1 se removeu ou 0 se não encontrou; árvore mantém validade BST; nó removido vai para lista de livres */
int removerMusicaArquivo(FILE *arquivo, CabecalhoMusica *cabecalho, int codigo) {
    int pai = -1;
    int atual = cabecalho->raiz;
    int filho = 0;
    Musica atualMusica;
    while (atual != -1) {
        fseek(arquivo, atual, SEEK_SET);
        fread(&atualMusica, sizeof(Musica), 1, arquivo);
        if (codigo == atualMusica.codigo) {
            break;
        }
        pai = atual;
        if (codigo < atualMusica.codigo) {
            atual = atualMusica.esquerda;
            filho = 0;
        } else {
            atual = atualMusica.direita;
            filho = 1;
        }
    }
    if (atual == -1) {
        return 0;
    }
    if (atualMusica.esquerda != -1 && atualMusica.direita != -1) {
        int sucessorPai = atual;
        int sucessor = atualMusica.direita;
        Musica sucessorMusica;
        fseek(arquivo, sucessor, SEEK_SET);
        fread(&sucessorMusica, sizeof(Musica), 1, arquivo);
        while (sucessorMusica.esquerda != -1) {
            sucessorPai = sucessor;
            sucessor = sucessorMusica.esquerda;
            fseek(arquivo, sucessor, SEEK_SET);
            fread(&sucessorMusica, sizeof(Musica), 1, arquivo);
        }
        atualMusica.codigo = sucessorMusica.codigo;
        strcpy(atualMusica.titulo, sucessorMusica.titulo);
        strcpy(atualMusica.artista, sucessorMusica.artista);
        atualMusica.ano = sucessorMusica.ano;
        int filhoDoSucessor = sucessorMusica.direita;
        if (sucessorPai == atual) {
            atualMusica.direita = filhoDoSucessor;
        } else {
            Musica paiSucessor;
            fseek(arquivo, sucessorPai, SEEK_SET);
            fread(&paiSucessor, sizeof(Musica), 1, arquivo);
            paiSucessor.esquerda = filhoDoSucessor;
            fseek(arquivo, sucessorPai, SEEK_SET);
            fwrite(&paiSucessor, sizeof(Musica), 1, arquivo);
        }
        fseek(arquivo, atual, SEEK_SET);
        fwrite(&atualMusica, sizeof(Musica), 1, arquivo);
        sucessorMusica.esquerda = cabecalho->livre;
        cabecalho->livre = sucessor;
        fseek(arquivo, sucessor, SEEK_SET);
        fwrite(&sucessorMusica, sizeof(Musica), 1, arquivo);
        fflush(arquivo);
        return 1;
    }
    int filhoUnico;
    if (atualMusica.esquerda != -1) {
        filhoUnico = atualMusica.esquerda;
    } else {
        filhoUnico = atualMusica.direita;
    }
    if (pai == -1) {
        cabecalho->raiz = filhoUnico;
    } else {
        Musica paiMusica;
        fseek(arquivo, pai, SEEK_SET);
        fread(&paiMusica, sizeof(Musica), 1, arquivo);
        if (filho == 0) {
            paiMusica.esquerda = filhoUnico;
        } else {
            paiMusica.direita = filhoUnico;
        }
        fseek(arquivo, pai, SEEK_SET);
        fwrite(&paiMusica, sizeof(Musica), 1, arquivo);
    }
    atualMusica.esquerda = cabecalho->livre;
    cabecalho->livre = atual;
    fseek(arquivo, atual, SEEK_SET);
    fwrite(&atualMusica, sizeof(Musica), 1, arquivo);
    fflush(arquivo);
    return 1;
}

/* Propósito: Imprime os nós livres disponíveis no arquivo de músicas
 * Pré-condição: arquivo e cabecalho são válidos
 * Pós-condição: Exibe todas as posições dos nós livres disponíveis */
void imprimirNosLivresMusicas(FILE *arquivo, CabecalhoMusica *cabecalho) {
    if (cabecalho->livre == -1) {
        printf("\nNenhum no livre disponivel.\n");
        return;
    }
    printf("\n=== NOS LIVRES DO ACERVO ===\n");
    int atual = cabecalho->livre;
    int quantidade = 0;
    while (atual != -1) {
        printf("Posicao: %d\n", atual);
        quantidade++;
        Musica musica;
        fseek(arquivo, atual, SEEK_SET);
        fread(&musica, sizeof(Musica), 1, arquivo);
        atual = musica.esquerda;
    }
    printf("Total: %d nos livres disponiveis\n", quantidade);
}

/* Propósito: Interação de menu para cadastrar uma nova música via entrada padrão
 * Pré-condição: arquivoMusicas e cabMusica são válidos e abertos para escrita
 * Pós-condição: Lê os dados do usuário e insere a música no acervo, exibindo mensagem de resultado */
void cadastrarMusica(FILE *arquivoMusicas, CabecalhoMusica *cabMusica) {
    Musica musica;
    printf("\n=== CADASTRAR MUSICA ===\n");
    printf("Codigo: ");
    if (!lerInteiro(&musica.codigo)) {
        printf("Codigo invalido.\n");
        return;
    }
    printf("Titulo: ");
    fgets(musica.titulo, MAX_TITULO, stdin);
    musica.titulo[strcspn(musica.titulo, "\n")] = '\0';
    printf("Artista: ");
    fgets(musica.artista, MAX_ARTISTA, stdin);
    musica.artista[strcspn(musica.artista, "\n")] = '\0';
    printf("Ano: ");
    if (!lerInteiro(&musica.ano)) {
        printf("Ano invalido.\n");
        return;
    }
    musica.esquerda = -1;
    musica.direita = -1;
    if (inserirMusica(arquivoMusicas, cabMusica, musica)) {
        printf("Musica cadastrada com sucesso!\n");
    } else {
        printf("Nao foi possivel cadastrar a musica.\n");
    }
}

/* Propósito: Interação de menu para exibir os dados de uma música pelo código
 * Pré-condição: arquivoMusicas e cabMusica são válidos
 * Pós-condição: Lê o código do usuário e exibe os dados da música ou mensagem de erro */
void imprimirDadosMusica(FILE *arquivoMusicas, CabecalhoMusica *cabMusica) {
    int codigo;
    Musica *musica;
    printf("\n=== DADOS DA MUSICA ===\n");
    printf("Digite o codigo da musica: ");
    if (!lerInteiro(&codigo)) {
        printf("Codigo invalido.\n");
        return;
    }
    musica = buscarMusica(arquivoMusicas, cabMusica, codigo);
    if (musica == NULL) {
        printf("Musica nao encontrada.\n");
        return;
    }
    printf("\nCodigo: %d\n", musica->codigo);
    printf("Titulo: %s\n", musica->titulo);
    printf("Artista: %s\n", musica->artista);
    printf("Ano: %d\n", musica->ano);
}

/* Propósito: Interação de menu para listar todas as músicas do acervo
 * Pré-condição: arquivoMusicas e cabMusica são válidos
 * Pós-condição: Exibe todas as músicas armazenadas no sistema */
void listarAcervo(FILE *arquivoMusicas, CabecalhoMusica *cabMusica) {
    listarMusicas(arquivoMusicas, cabMusica);
}
