
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

char **palavras;
char **dicas;
int quantidadePalavras;

int menu();
void liberar_memoria();
int le_inteiro();
void cadastra_palavras();
void joga_partida();
void mostra_palavra(char palavra[], char letrasUsadas[]);
int verifica_letra(char palavra[], char letra);
int verifica_vitoria(char palavra[], char letrasUsadas[]);
void mostra_forca(int erros);

int main() {
    int opcao;

    srand((unsigned int) time(NULL));

    cadastra_palavras();

    do {
        opcao = menu();

        if (opcao == 1) {
            joga_partida();
        } else if (opcao == 2) {
            printf("Saindo do jogo...\n");
        }
    } while (opcao != 2);
    liberar_memoria();
    return 0;
}

void liberar_memoria() {
    int i;

    for (i = 0; i < quantidadePalavras; i++) {
        free(palavras[i]);
        free(dicas[i]);
    }

    free(palavras);
    free(dicas);
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

    while ((caractere = getchar()) != '\n' && caractere != EOF);

    return valor;
}

int menu() {
    int opcao = 0;

    while (opcao < 1 || opcao > 2) {
        printf("\n===== JOGO DA FORCA =====\n");
        printf("1. Jogar\n");
        printf("2. Sair\n");
        printf("Escolha uma opcao: ");

        opcao = le_inteiro();

        if (opcao < 1 || opcao > 2) {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return opcao;
}

void cadastra_palavras() {
    int i;
    char entrada[200];

    printf("Quantas palavras deseja cadastrar? ");

    do {
        quantidadePalavras = le_inteiro();

        if (quantidadePalavras < 1) {
            printf("Informe uma quantidade valida: ");
        }
    } while (quantidadePalavras < 1);

    palavras = malloc(quantidadePalavras * sizeof(*palavras));
    dicas = malloc(quantidadePalavras * sizeof(*dicas));

    if (palavras == NULL || dicas == NULL) {
        free(palavras);
        free(dicas);
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    for (i = 0; i < quantidadePalavras; i++) {
        palavras[i] = malloc(200 * sizeof(char));
        dicas[i] = malloc(200 * sizeof(char));

        if (palavras[i] == NULL || dicas[i] == NULL) {
            int j;
        
            free(palavras[i]);
            free(dicas[i]);
        
            for (j = 0; j < i; j++) {
                free(palavras[j]);
                free(dicas[j]);
            }
        
            free(palavras);
            free(dicas);
        
            printf("Erro ao alocar memoria.\n");
            exit(1);
        }

        printf("\nDigite a palavra %d: ", i + 1);
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        while (strlen(entrada) == 0) {
            printf("A palavra nao pode ficar vazia. Digite novamente: ");
            fgets(entrada, sizeof(entrada), stdin);
            entrada[strcspn(entrada, "\n")] = '\0';
        }

        strcpy(palavras[i], entrada);

        printf("Digite a dica da palavra: ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        strcpy(dicas[i], entrada);
    }

    printf("\nBiblioteca cadastrada com sucesso!\n");
}

void joga_partida() {
    int indice;
    int erros = 0;
    int i;
    int venceu = 0;
    char letra;
    char letrasUsadas[27] = "";

    indice = rand() % quantidadePalavras;

    printf("\nDica: %s\n", dicas[indice]);

    while (erros < 6 && !venceu) {
        mostra_forca(erros);
        mostra_palavra(palavras[indice], letrasUsadas);

        printf("Letras usadas: %s\n", letrasUsadas);
        printf("Digite uma letra: ");
        scanf(" %c", &letra);
        getchar();
        letra = toupper(letra);

        for (i = 0; letrasUsadas[i] != '\0'; i++) {
            if (letrasUsadas[i] == letra) {
                break;
            }
        }

        if (letrasUsadas[i] == letra) {
            printf("Voce ja tentou essa letra!\n");
            continue;
        }

        i = strlen(letrasUsadas);
        letrasUsadas[i] = letra;
        letrasUsadas[i + 1] = '\0';

        if (!verifica_letra(palavras[indice], letra)) {
            erros++;
            printf("Letra incorreta!\n");
        } else {
            printf("Letra correta!\n");
        }

        venceu = verifica_vitoria(palavras[indice], letrasUsadas);
    }

    mostra_forca(erros);

    if (venceu) {
        printf("\nParabens! Voce acertou a palavra: %s\n", palavras[indice]);
    } else {
        printf("\nVoce perdeu! A palavra era: %s\n", palavras[indice]);
    }
}

void mostra_palavra(char palavra[], char letrasUsadas[]) {
    int i;
    int encontrada;

    printf("\nPalavra: ");

    for (i = 0; palavra[i] != '\0'; i++) {
        encontrada = 0;

        for (int j = 0; letrasUsadas[j] != '\0'; j++) {
            if (toupper(palavra[i]) == letrasUsadas[j]) {
                encontrada = 1;
                break;
            }
        }

        if (encontrada) {
            printf("%c ", palavra[i]);
        } else if (palavra[i] == ' ') {
            printf("  ");
        } else {
            printf("_ ");
        }
    }

    printf("\n");
}

int verifica_letra(char palavra[], char letra) {
    int i;

    for (i = 0; palavra[i] != '\0'; i++) {
        if (toupper(palavra[i]) == letra) {
            return 1;
        }
    }

    return 0;
}

int verifica_vitoria(char palavra[], char letrasUsadas[]) {
    int i;
    int j;
    int encontrada;

    for (i = 0; palavra[i] != '\0'; i++) {
        if (palavra[i] == ' ') {
            continue;
        }

        encontrada = 0;

        for (j = 0; letrasUsadas[j] != '\0'; j++) {
            if (toupper(palavra[i]) == letrasUsadas[j]) {
                encontrada = 1;
                break;
            }
        }

        if (!encontrada) {
            return 0;
        }
    }

    return 1;
}

void mostra_forca(int erros) {
    printf("\n +---+\n");
    printf(" |   |\n");

    if (erros >= 1) {
        printf(" O   |\n");
    } else {
        printf("     |\n");
    }

    if (erros == 2) {
        printf(" |   |\n");
    } else if (erros == 3) {
        printf("/|   |\n");
    } else if (erros >= 4) {
        printf("/|\\  |\n");
    } else {
        printf("     |\n");
    }

    if (erros == 5) {
        printf("/    |\n");
    } else if (erros >= 6) {
        printf("/ \\  |\n");
    } else {
        printf("     |\n");
    }

    printf("     |\n");
    printf("=======\n");
}
