#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

char velha[3][3];
int nivelComputador;

void inicializa_velha();
int menu();
int le_inteiro();
void escolha_simb(char *jog1, char *jog2);
void mostra_velha();
void joga_partida(int opcao, char jog1, char jog2);
int jogada_usuario(int lin, int col, char jog);
void jogada_computador(char jog, int nivel);
int encontra_jogada_vencedora(char jog);
int encontra_dupla_vitoria(char jog);
int jogada_basica();
int jogada_intermediaria(char jog);
int jogada_avancada(char jog);
int verifica_ganhador(char jog);
int tabuleiro_cheio();

int main() {
    int opcao;
    char jog1, jog2;

    srand((unsigned int) time(NULL));
    opcao = menu();

    if (opcao == 3) {
        printf("Saindo do jogo...\n");
        return 0;
    }

    escolha_simb(&jog1, &jog2);
    joga_partida(opcao, jog1, jog2);
    return 0;
}

int le_inteiro() {
    int valor;
    int resultado;
    int caractere;

    resultado = scanf("%d", &valor);

    if (resultado != 1) {
        do {
            caractere = getchar();
        } while (caractere != '\n' && caractere != EOF);
        return -1;
    }
    return valor;
}

int menu() {
    int opcao = 0;

    while (opcao < 1 || opcao > 3) {
        printf("\n===== JOGO DA VELHA =====\n");
        printf("1. Jogar contra o computador\n");
        printf("2. Jogar contra outro jogador\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");

        opcao = le_inteiro();
        if (opcao < 1 || opcao > 3) printf("Opcao invalida. Tente novamente.\n");
    }

    if (opcao == 1) {
        nivelComputador = 0;

        while (nivelComputador < 1 || nivelComputador > 3) {
            printf("\nNivel do computador:\n");
            printf("1. Basico\n");
            printf("2. Intermediario\n");
            printf("3. Avancado\n");
            printf("Escolha o nivel: ");

            nivelComputador = le_inteiro();
            if (nivelComputador < 1 || nivelComputador > 3) printf("Nivel invalido. Tente novamente.\n");
        }
    }

    return opcao;
}

void escolha_simb(char *jog1, char *jog2) {
    char simbolo;

    do {
        printf("\nJogador 1, escolha seu simbolo (X ou O): ");
        scanf(" %c", &simbolo);

        simbolo = (char) toupper((unsigned char) simbolo);

        if (simbolo != 'X' && simbolo != 'O') printf("Simbolo invalido. Digite X ou O.\n");
    } while (simbolo != 'X' && simbolo != 'O');

    *jog1 = simbolo;

    if (*jog1 == 'X') {
        *jog2 = 'O';
    } else {
        *jog2 = 'X';
    }

    printf("Jogador 1: %c\n", *jog1);
    printf("Jogador 2: %c\n", *jog2);
}

void inicializa_velha() {
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            velha[i][j] = ' ';
        }
    }
}

void mostra_velha() {
    int i, j;
    int posicao;

    printf("\n");

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            posicao = i * 3 + j + 1;

            if (velha[i][j] == ' ') {
                printf(" %d ", posicao);
            } else {
                printf(" %c ", velha[i][j]);
            }

            if (j < 2) printf("|");
        }
        printf("\n");
        if (i < 2) printf("---+---+---\n");
    }
    printf("\n");
}

/* Retorna 0 para jogada valida, 1 para posicao invalida e 2 para casa ocupada. */
int jogada_usuario(int lin, int col, char jog) {
    if (lin < 0 || lin > 2 || col < 0 || col > 2) return 1;
    if (velha[lin][col] != ' ') return 2;
    velha[lin][col] = jog;
    return 0;
}

int verifica_ganhador(char jog) {
    int i;

    /* Linhas e colunas */
    for (i = 0; i < 3; i++) {
        if (velha[i][0] == jog && velha[i][1] == jog && velha[i][2] == jog) return 1;
        if (velha[0][i] == jog && velha[1][i] == jog && velha[2][i] == jog) return 1;
    }

    /* Diagonais */
    if (velha[0][0] == jog && velha[1][1] == jog && velha[2][2] == jog) return 1;
    if (velha[0][2] == jog && velha[1][1] == jog && velha[2][0] == jog) return 1;
    return 0;
}

/* Retorna 1 quando nao ha casas vazias. */
int tabuleiro_cheio() {
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (velha[i][j] == ' ') return 0;
        }
    }
    return 1;
}

/* Retorna a posicao de vitoria ou 0 se nao houver. */
int encontra_jogada_vencedora(char jog) {
    int i, j;
    int posicao;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (velha[i][j] == ' ') {
                velha[i][j] = jog;

                if (verifica_ganhador(jog)) {
                    velha[i][j] = ' ';
                    posicao = i * 3 + j + 1;
                    return posicao;
                }

                velha[i][j] = ' ';
            }
        }
    }
    return 0;
}

void jogada_computador(char jog, int nivel) {
    int posicao;
    int lin, col;

    if (nivel == 1) {
        posicao = jogada_basica();
    } else if (nivel == 2) {
        posicao = jogada_intermediaria(jog);
    } else {
        posicao = jogada_avancada(jog);
    }
    if (posicao == 0) return;

    lin = (posicao - 1) / 3;
    col = (posicao - 1) % 3;
    jogada_usuario(lin, col, jog);
    printf("O computador escolheu a posicao %d.\n", posicao);
}

/* Escolhe uma casa vazia aleatoria. */
int jogada_basica() {
    int casas[9];
    int quantidade = 0;
    int i, j;
    int escolha;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (velha[i][j] == ' ') {
                casas[quantidade] = i * 3 + j + 1;
                quantidade++;
            }
        }
    }
    if (quantidade == 0) return 0;
    escolha = rand() % quantidade;
    return casas[escolha];
}

/* Tenta vencer, bloquear, ocupar o centro ou um canto. */
int jogada_intermediaria(char jog) {
    char adversario;
    int posicao;
    int cantos[4] = {1, 3, 7, 9};
    int quantidade = 4;
    int i, escolha;
    int lin, col;

    adversario = (jog == 'X') ? 'O' : 'X';

    posicao = encontra_jogada_vencedora(jog);
    if (posicao != 0) return posicao;

    posicao = encontra_jogada_vencedora(adversario);
    if (posicao != 0) return posicao;

    if (velha[1][1] == ' ') return 5;

    while (quantidade > 0) {
        escolha = rand() % quantidade;
        posicao = cantos[escolha];
        lin = (posicao - 1) / 3;
        col = (posicao - 1) % 3;

        if (velha[lin][col] == ' ') return posicao;

        cantos[escolha] = cantos[quantidade - 1];
        quantidade--;
    }

    return jogada_basica();
}

int encontra_dupla_vitoria(char jog) {
    int i, j;
    int lin, col;
    int possibilidadesVitoria;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (velha[i][j] == ' ') {
                velha[i][j] = jog;
                possibilidadesVitoria = 0;

                for (lin = 0; lin < 3; lin++) {
                    for (col = 0; col < 3; col++) {
                        if (velha[lin][col] == ' ') {
                            velha[lin][col] = jog;

                            if (verifica_ganhador(jog)) possibilidadesVitoria++;

                            velha[lin][col] = ' ';
                        }
                    }
                }
                velha[i][j] = ' ';
                if (possibilidadesVitoria >= 2) return i * 3 + j + 1;
            }
        }
    }

    return 0;
}

int jogada_avancada(char jog) {
    int posicao;
    int i;
    int laterais[4] = {2, 4, 6, 8};
    int linha, coluna;
    char adversario;

    adversario = (jog == 'X') ? 'O' : 'X';

    posicao = encontra_jogada_vencedora(jog);
    if (posicao != 0) return posicao;

    posicao = encontra_jogada_vencedora(adversario);
    if (posicao != 0) return posicao;

    if (velha[1][1] == ' ') return 5;

    if (velha[1][1] == jog &&
        ((velha[0][0] == adversario && velha[2][2] == adversario) ||
         (velha[0][2] == adversario && velha[2][0] == adversario))) {
        for (i = 0; i < 4; i++) {
            posicao = laterais[i];
            linha = (posicao - 1) / 3;
            coluna = (posicao - 1) % 3;

            if (velha[linha][coluna] == ' ') return posicao;
        }
    }

    posicao = encontra_dupla_vitoria(jog);
    if (posicao != 0) return posicao;

    posicao = encontra_dupla_vitoria(adversario);
    if (posicao != 0) return posicao;

    for (i = 0; i < 4; i++) {
        posicao = laterais[i] - 1;
        if (velha[posicao / 3][posicao % 3] == ' ') return laterais[i];
    }

    return jogada_basica();
}

void joga_partida(int opcao, char jog1, char jog2) {
    char turno = 'X';
    int posicao;
    int resultado;
    int lin, col;

    inicializa_velha();

    while (1) {
        mostra_velha();

        if (opcao == 1 && turno == jog2) {
            printf("Vez do computador (%c).\n", turno);
            jogada_computador(turno, nivelComputador);
        } else {
            if (turno == jog1) {
                printf("Vez do jogador 1 (%c).\n", turno);
            } else {
                printf("Vez do jogador 2 (%c).\n", turno);
            }

            do {
                printf("Escolha uma posicao (1-9): ");
                posicao = le_inteiro();

                if (posicao < 1 || posicao > 9) {
                    printf("Posicao invalida. Tente novamente.\n");
                    resultado = 1;
                } else {
                    lin = (posicao - 1) / 3;
                    col = (posicao - 1) % 3;
                    resultado = jogada_usuario(lin, col, turno);
                    if (resultado == 2) printf("Essa casa ja esta preenchida.\n");
                }
            } while (resultado != 0);
        }

        mostra_velha();

        if (verifica_ganhador(turno)) {
            if (opcao == 1 && turno == jog2) {
                printf("O computador venceu!\n");
            } else if (turno == jog1) {
                printf("Jogador 1 venceu!\n");
            } else {
                printf("Jogador 2 venceu!\n");
            }
            break;
        }

        if (tabuleiro_cheio()) {
            printf("Empate! Nao ha mais casas vazias.\n");
            break;
        }

        turno = (turno == 'X') ? 'O' : 'X';
    }
}