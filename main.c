
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>

/* Matriz global do jogo */
char velha[3][3];

/* Nivel escolhido para o computador */
int nivelComputador = 2;

/* Prototipos das funcoes */
int jogadaUsuario(int lin, int col, char jog);
void jogadaComputador(char jog, int nivel);
int menu(void);
void escolhaSimb(char *jog1, char *jog2);
void inicializaVelha(void);
int verificaGanhador(char jog);

void mostraVelha(void);
int tabuleiroCheio(void);
int leInteiro(void);
int jogadaBasica(char jog);
int jogadaIntermediaria(char jog);
int jogadaAvancada(char jog);
int encontraJogadaVencedora(char jog);
void jogaPartida(int opcao, char jog1, char jog2);

int main(void) {
    int opcao;
    char jog1, jog2;

    // Só pra aceitar acentos...
    setlocale(LC_ALL, "Portuguese");
    // Gerador de numeros aleatorios
    srand((unsigned int) time(NULL));

    opcao = menu(); 

    if (opcao == 3) {
        printf("Saindo do jogo...\n");
        return 0;
    }

    escolhaSimb(&jog1, &jog2);

    jogaPartida(opcao, jog1, jog2);

    return 0;
}

/* Le um numero inteiro com validacao basica da entrada */
int leInteiro(void) {
    int valor;
    int resultado;
    int caractere;

    resultado = scanf("%d", &valor);

    if (resultado != 1) {
        while ((caractere = getchar()) != '\n' &&
               caractere != EOF) {
            /* Limpa a entrada invalida */
        }
        return -1;
    }

    return valor;
}

/* Menu principal */
int menu(void) {
    int opcao = 0;

    while (opcao < 1 || opcao > 3) {
        printf("\n===== JOGO DA VELHA =====\n");
        printf("1. Jogar contra o computador\n");
        printf("2. Jogar contra outro jogador\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");

        opcao = leInteiro();

        if (opcao < 1 || opcao > 3) {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }

    if (opcao == 1) {
        nivelComputador = 0;

        while (nivelComputador < 1 ||
               nivelComputador > 3) {
            printf("\nNivel do computador:\n");
            printf("1. Basico\n");
            printf("2. Intermediario\n");
            printf("3. Avancado\n");
            printf("Escolha o nivel: ");

            nivelComputador = leInteiro();

            if (nivelComputador < 1 ||
                nivelComputador > 3) {
                printf("Nivel invalido. Tente novamente.\n");
            }
        }
    }

    return opcao;
}

/* Escolha dos simbolos dos jogadores */
void escolhaSimb(char *jog1, char *jog2) {
    char simbolo;

    do {
        printf("\nJogador 1, escolha seu simbolo (X ou O): ");
        scanf(" %c", &simbolo);

        simbolo = (char) toupper((unsigned char) simbolo);

        if (simbolo != 'X' && simbolo != 'O') {
            printf("Simbolo invalido. Digite X ou O.\n");
        }
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

/* Inicializa todas as casas como vazias */
void inicializaVelha(void) {
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            velha[i][j] = ' ';
        }
    }
}

/* Mostra o tabuleiro com os numeros das casas vazias */
void mostraVelha(void) {
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

            if (j < 2) {
                printf("|");
            }
        }

        printf("\n");

        if (i < 2) {
            printf("---+---+---\n");
        }
    }

    printf("\n");
}

/*
Retornos:
0 = jogada valida
1 = linha ou coluna invalida
2 = casa ja preenchida

Linha e coluna variam de 0 a 2.
*/
int jogadaUsuario(int lin, int col, char jog) {
    if (lin < 0 || lin > 2 ||
        col < 0 || col > 2) {
        return 1;
    }

    if (velha[lin][col] != ' ') {
        return 2;
    }

    velha[lin][col] = jog;

    return 0;
}

/* Verifica se o simbolo ganhou */
int verificaGanhador(char jog) {
    int i;

    /* Verifica linhas e colunas */
    for (i = 0; i < 3; i++) {
        if (velha[i][0] == jog &&
            velha[i][1] == jog &&
            velha[i][2] == jog) {
            return 1;
        }

        if (velha[0][i] == jog &&
            velha[1][i] == jog &&
            velha[2][i] == jog) {
            return 1;
        }
    }

    /* Verifica diagonais */
    if (velha[0][0] == jog &&
        velha[1][1] == jog &&
        velha[2][2] == jog) {
        return 1;
    }

    if (velha[0][2] == jog &&
        velha[1][1] == jog &&
        velha[2][0] == jog) {
        return 1;
    }

    return 0;
}

/* Verifica se todas as casas estao ocupadas */
int tabuleiroCheio(void) {
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (velha[i][j] == ' ') {
                return 0;
            }
        }
    }

    return 1;
}

/*
Procura uma jogada que permita ao simbolo ganhar
imediatamente. Retorna a posicao de 1 a 9,
ou zero se nao encontrar.
*/
int encontraJogadaVencedora(char jog) {
    int i, j;
    int posicao;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (velha[i][j] == ' ') {
                velha[i][j] = jog;

                if (verificaGanhador(jog)) {
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

/* Computador basico: escolhe uma casa vazia aleatoriamente */
int jogadaBasica(char jog) {
    int casas[9];
    int quantidade = 0;
    int i, j;
    int escolha;

    (void) jog;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (velha[i][j] == ' ') {
                casas[quantidade] = i * 3 + j + 1;
                quantidade++;
            }
        }
    }

    if (quantidade == 0) {
        return 0;
    }

    escolha = rand() % quantidade;

    return casas[escolha];
}

/*
Computador intermediario:
1. Tenta vencer.
2. Bloqueia uma vitoria do adversario.
3. Escolhe o centro.
4. Escolhe um canto.
5. Escolhe outra casa vazia.
*/
int jogadaIntermediaria(char jog) {
    int posicao;
    int adversario;
    int cantos[4] = {1, 3, 7, 9};
    int i, escolha;
    int lin, col;

    adversario = (jog == 'X') ? 'O' : 'X';

    /* Tenta vencer */
    posicao = encontraJogadaVencedora((char) jog);

    if (posicao != 0) {
        return posicao;
    }

    /* Tenta impedir a vitoria do adversario */
    posicao = encontraJogadaVencedora((char) adversario);

    if (posicao != 0) {
        return posicao;
    }

    /* Prioriza o centro */
    if (velha[1][1] == ' ') {
        return 5;
    }

    /* Tenta escolher um canto */
    for (i = 0; i < 4; i++) {
        escolha = rand() % 4;
        posicao = cantos[escolha];

        lin = (posicao - 1) / 3;
        col = (posicao - 1) % 3;

        if (velha[lin][col] == ' ') {
            return posicao;
        }

        /* Tenta os outros cantos se este estiver ocupado */
        cantos[escolha] = cantos[3];
        cantos[3] = posicao;
    }

    /* Se necessario, escolhe uma casa vazia */
    return jogadaBasica(jog);
}

/* Nivel avancado: estrategia semelhante, com prioridade ao centro */
int jogadaAvancada(char jog) {
    int posicao;
    int adversario;
    int i, j;

    adversario = (jog == 'X') ? 'O' : 'X';

    /* Vence se houver oportunidade */
    posicao = encontraJogadaVencedora(jog);

    if (posicao != 0) {
        return posicao;
    }

    /* Bloqueia a vitoria adversaria */
    posicao = encontraJogadaVencedora((char) adversario);

    if (posicao != 0) {
        return posicao;
    }

    /* Prioriza o centro */
    if (velha[1][1] == ' ') {
        return 5;
    }

    /* Prioriza os cantos */
    for (i = 0; i < 3; i += 2) {
        for (j = 0; j < 3; j += 2) {
            if (velha[i][j] == ' ') {
                return i * 3 + j + 1;
            }
        }
    }

    return jogadaBasica(jog);
}

/* Executa a jogada do computador conforme o nivel */
void jogadaComputador(char jog, int nivel) {
    int posicao;
    int lin, col;

    if (nivel == 1) {
        posicao = jogadaBasica(jog);
    } else if (nivel == 2) {
        posicao = jogadaIntermediaria(jog);
    } else {
        posicao = jogadaAvancada(jog);
    }

    if (posicao == 0) {
        return;
    }

    lin = (posicao - 1) / 3;
    col = (posicao - 1) % 3;

    jogadaUsuario(lin, col, jog);

    printf("O computador escolheu a posicao %d.\n", posicao);
}

// Controla a partida, fiz desse jeito pq ficou mais fácil de organiar o código, mas poderia ter feito de outra forma direto na main 
void jogaPartida(int opcao, char jog1, char jog2) {
    char turno = 'X'; // Começa sempre com o jogador 1, no caso X
    int posicao; //  armazena a posição escolhida pelo jogador
    int resultado; // armazena o resultado da jogada do jogador
    int lin, col;

    inicializaVelha();

    while (1) {
        mostraVelha();

        if (opcao == 1 && turno == jog2) {
            printf("Vez do computador (%c).\n", turno);
            jogadaComputador(turno, nivelComputador);
        } else {
            if (turno == jog1) {
                printf("Vez do jogador 1 (%c).\n", turno);
            } else {
                printf("Vez do jogador 2 (%c).\n", turno);
            }

            do {
                printf("Escolha uma posicao (1-9): ");
                posicao = leInteiro();

                if (posicao < 1 || posicao > 9) {
                    printf("Posicao invalida. Tente novamente.\n");
                    resultado = 1;
                } else {
                    lin = (posicao - 1) / 3;
                    col = (posicao - 1) % 3;

                    resultado = jogadaUsuario(lin, col, turno);

                    if (resultado == 2) {
                        printf("Essa casa ja esta preenchida.\n");
                    }
                }
            } while (resultado != 0);
        }

        mostraVelha();

        if (verificaGanhador(turno)) {
            if (opcao == 1 && turno == jog2) {
                printf("O computador venceu!\n");
            } else if (turno == jog1) {
                printf("Jogador 1 venceu!\n");
            } else {
                printf("Jogador 2 venceu!\n");
            }

            break;
        }

        if (tabuleiroCheio()) {
            printf("Empate! Nao ha mais casas vazias.\n");
            break;
        }

        turno = (turno == 'X') ? 'O' : 'X';
    }
}
