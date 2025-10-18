#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

typedef struct {
    char nome; // Tipo da peca: 'I', 'O', 'T', 'L'
    int id;    // Identificador unico
} Peca;

typedef struct {
    Peca fila[TAMANHO_FILA];
    int frente;
    int tras;
    int tamanho;
    int contador_id;
} FilaPecas;

typedef struct {
    Peca pilha[TAMANHO_PILHA];
    int topo;
} PilhaReserva;

// Gera uma nova peca com tipo aleatorio e id unico
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Inicializa a fila com 5 pecas
void inicializarFila(FilaPecas *fp) {
    fp->frente = 0;
    fp->tras = 0;
    fp->tamanho = 0;
    fp->contador_id = 0;

    for (int i = 0; i < TAMANHO_FILA; i++) {
        fp->fila[fp->tras] = gerarPeca(fp->contador_id++);
        fp->tras = (fp->tras + 1) % TAMANHO_FILA;
        fp->tamanho++;
    }
}

// Inicializa a pilha de reserva
void inicializarPilha(PilhaReserva *pr) {
    pr->topo = -1;
}

// Adiciona nova peca ao final da fila
void adicionarNovaPeca(FilaPecas *fp) {
    if (fp->tamanho == TAMANHO_FILA) {
        fp->fila[fp->tras] = gerarPeca(fp->contador_id++);
        fp->tras = (fp->tras + 1) % TAMANHO_FILA;
        fp->frente = (fp->frente + 1) % TAMANHO_FILA;
    } else {
        fp->fila[fp->tras] = gerarPeca(fp->contador_id++);
        fp->tras = (fp->tras + 1) % TAMANHO_FILA;
        fp->tamanho++;
    }
}

// Remove a peca da frente da fila
void jogarPeca(FilaPecas *fp) {
    if (fp->tamanho == 0) {
        printf("Fila vazia. Nenhuma peca para jogar.\n");
        return;
    }
    printf("Peca jogada: [%c %d]\n", fp->fila[fp->frente].nome, fp->fila[fp->frente].id);
    fp->frente = (fp->frente + 1) % TAMANHO_FILA;
    fp->tamanho--;
    adicionarNovaPeca(fp);
}

// Move a peca da frente da fila para o topo da pilha
void reservarPeca(FilaPecas *fp, PilhaReserva *pr) {
    if (fp->tamanho == 0) {
        printf("Fila vazia. Nenhuma peca para reservar.\n");
        return;
    }
    if (pr->topo == TAMANHO_PILHA - 1) {
        printf("Pilha cheia. Nao e possivel reservar mais pecas.\n");
        return;
    }
    pr->topo++;
    pr->pilha[pr->topo] = fp->fila[fp->frente];
    printf("Peca reservada: [%c %d]\n", pr->pilha[pr->topo].nome, pr->pilha[pr->topo].id);
    fp->frente = (fp->frente + 1) % TAMANHO_FILA;
    fp->tamanho--;
    adicionarNovaPeca(fp);
}

// Remove a peca do topo da pilha
void usarPecaReservada(PilhaReserva *pr) {
    if (pr->topo == -1) {
        printf("Pilha vazia. Nenhuma peca reservada para usar.\n");
        return;
    }
    printf("Peca usada da reserva: [%c %d]\n", pr->pilha[pr->topo].nome, pr->pilha[pr->topo].id);
    pr->topo--;
}

// Troca a peca da frente da fila com o topo da pilha
void trocarAtual(FilaPecas *fp, PilhaReserva *pr) {
    if (fp->tamanho == 0 || pr->topo == -1) {
        printf("Nao e possivel realizar a troca atual.\n");
        return;
    }
    Peca temp = fp->fila[fp->frente];
    fp->fila[fp->frente] = pr->pilha[pr->topo];
    pr->pilha[pr->topo] = temp;
    printf("Troca realizada entre frente da fila e topo da pilha.\n");
}

// Troca os 3 primeiros da fila com os 3 da pilha
void trocaMultipla(FilaPecas *fp, PilhaReserva *pr) {
    if (fp->tamanho < 3 || pr->topo < 2) {
        printf("Nao ha pecas suficientes para troca multipla.\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int posFila = (fp->frente + i) % TAMANHO_FILA;
        Peca temp = fp->fila[posFila];
        fp->fila[posFila] = pr->pilha[pr->topo - i];
        pr->pilha[pr->topo - i] = temp;
    }
    printf("Troca multipla realizada.\n");
}

// Exibe o estado atual da fila e da pilha
void exibirEstado(FilaPecas *fp, PilhaReserva *pr) {
    printf("\nEstado atual:\n");
    printf("Fila de pecas\t");
    int i = fp->frente;
    for (int count = 0; count < fp->tamanho; count++) {
        printf("[%c %d] ", fp->fila[i].nome, fp->fila[i].id);
        i = (i + 1) % TAMANHO_FILA;
    }
    printf("\nPilha de reserva\t(Topo -> base): ");
    for (int j = pr->topo; j >= 0; j--) {
        printf("[%c %d] ", pr->pilha[j].nome, pr->pilha[j].id);
    }
    printf("\n");
}

int main() {
    FilaPecas fila;
    PilhaReserva pilha;
    int opcao;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    do {
        exibirEstado(&fila, &pilha);
        printf("\nCodigo\tAcao\n");
        printf("1\tJogar peca da frente da fila\n");
        printf("2\tEnviar peca da fila para a pilha de reserva\n");
        printf("3\tUsar peca da pilha de reserva\n");
        printf("4\tTrocar frente da fila com topo da pilha\n");
        printf("5\tTrocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0\tSair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                break;
            case 3:
                usarPecaReservada(&pilha);
                break;
            case 4:
                trocarAtual(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
