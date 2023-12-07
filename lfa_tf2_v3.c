#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINES 4
#define MAX_PROD 10

typedef struct {
    char nome_modelo;
    char caract_1;
    char caract_2;
    char success;
} Vehicle;

typedef struct {
    char state;
    char read_symbol;
    char push_symbol;
    char pop_symbol;
    char next_state;
} Transition;

void generateFitaSiglas(int num_caract, int chance_falha, char* fita_siglas);
char generateSigla(int num_caract, int chance_falha);
void separateSiglas(char* fita_siglas);
void runAutomatonWithStack(char* fitaO, Transition* transitions, int numTransitions, Vehicle* vehicles, int* numVehicles, int num_caract);
char executeTransition(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol, int num_caract);
void transitionsForNumCaract0(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol);
void transitionsForNumCaract1(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol);
void transitionsForNumCaract2(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol);
char* transformResult(char* result);
void printReport(Vehicle* vehicles, int numVehicles, int maxProd, int num_caract);

int main() {
    int numLines, maxProd, numCaract, chanceFalha;

    printf("Quantas linhas devem funcionar no dia? (Max 4): ");
    scanf("%d", &numLines);

    if (numLines <= 0 || numLines > 4) {
        printf("Número de linhas inválido. Encerrando o programa.\n");
        return 1;
    }

    printf("Qual o máximo de veículos que pode ser produzido no dia? (Max 10): ");
    scanf("%d", &maxProd);

    printf("Qual o nome do Modelo? ");
    char nome_modelo;
    scanf(" %c", &nome_modelo);

    printf("Quantas características o modelo apresenta? (0 || 1 || 2): ");
    scanf("%d", &numCaract);

    if (numCaract < 0 || numCaract > 2) {
        printf("Valor inválido para número de características. Encerrando o programa.\n");
        return 1;
    }

    int caract_1, caract_2;

    if (numCaract == 1) {
        printf("Qual o nome da primeira característica? ");
        scanf(" %d", &caract_1);
    } else if (numCaract == 2) {
        printf("Qual o nome da segunda característica? ");
        scanf(" %d", &caract_2);
    }

    printf("Qual a chance de erro na montagem do veículo? (0 até 100): ");
    scanf("%d", &chanceFalha);

    if (chanceFalha < 0 || chanceFalha > 100) {
        printf("Valor inválido para chance de Falha. Encerrando o programa.\n");
        return 1;
    }

    char fitaSiglas[100];
    generateFitaSiglas(numCaract, chanceFalha, fitaSiglas);

    Transition transitions[100];

    Vehicle vehicles[MAX_PROD];
    int numVehicles = 0;

    runAutomatonWithStack(fitaSiglas, transitions, 10, vehicles, &numVehicles, numCaract);

    char result[100];
    char* result_o;

    result_o = transformResult(result);

    // resultado original
    // printf("Resultado Original: %s\n", result);

    // resultado organizado
    // printf("Resultado Organizado: %s\n", result_o);

    printReport(vehicles, numVehicles, maxProd);

    free(result_o);

    return 0;
}

void generateFitaSiglas(int num_caract, int chance_falha, char* fita_siglas) {
    srand(time(NULL));

    int num_siglas = rand() % 6 + 5;  // Número de 5 a 10 siglas

    int index = 0;  // Índice para controlar a posição na string

    for (int i = 0; i < num_siglas; ++i) {
        char sigla = generateSigla(num_caract, chance_falha);
        fita_siglas[index++] = sigla;

        if (i < num_siglas - 1) {
            fita_siglas[index++] = '|';  // Adiciona "|" entre as siglas, exceto para a última
        }
    }

    fita_siglas[index++] = '.';  // Adiciona "." ao final da fita
    fita_siglas[index] = '\0';    // Adiciona o caractere nulo ao final da string para indicar o término
}

char generateSigla(int num_caract, int chance_falha) {
    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios

    char sigla;

    // Caso num_caract = 0, gera 'av' ou 'af' com probabilidade 50%
    if (num_caract == 0) {
        sigla = (rand() % 2 == 0) ? 'v' : 'f';
        sigla = (rand() % 2 == 0) ? 'a' + sigla : sigla;
    }

    // Caso num_caract = 1
    else if (num_caract == 1) {
        // Adiciona 'x' ou 'n' com probabilidade 50%
        sigla = (rand() % 2 == 0) ? 'x' : 'n';

        // Sempre termina com 'v' ou 'f' (a chance de ser 'f' é determinada por chance_falha)
        if (rand() % 100 < chance_falha) {
            sigla = 'f';  // Adiciona 'f' com probabilidade igual à chance_falha
        } else {
            sigla = 'v';  // Adiciona 'v' com probabilidade 100 - chance_falha
        }
    }

    // Caso num_caract = 2
    else if (num_caract == 2) {
        // Adiciona 'x' ou 'n' com probabilidade 50%
        sigla = (rand() % 2 == 0) ? 'x' : 'n';

        // Adiciona 'y' ou 'n' com probabilidade 50%
        sigla = (rand() % 2 == 0) ? 'y' : 'n';

        // Sempre termina com 'v' ou 'f' (a chance de ser 'f' é determinada por chance_falha)
        if (rand() % 100 < chance_falha) {
            sigla = 'f';  // Adiciona 'f' com probabilidade igual à chance_falha
        } else {
            sigla = 'v';  // Adiciona 'v' com probabilidade 100 - chance_falha
        }
    }

    return sigla;
}

void separateSiglas(char* fita_siglas) {
    int num_sigla = 1;
    char* token = strtok(fita_siglas, "|.");  // Usa strtok para separar as siglas

    while (token != NULL) {
        // Imprime o número da sigla e a própria sigla
        printf("%d%s ", num_sigla++, token);

        // Obtém a próxima sigla
        token = strtok(NULL, "|.");
    }
}

void runAutomatonWithStack(char* fitaO, Transition* transitions, int numTransitions, Vehicle* vehicles, int* numVehicles, int num_caract) {
    char stack[100];
    int top = -1;

    char currentState = 'q0';
    char readSymbol, pushSymbol, popSymbol, nextSymbol;

    for (int i = 0; i < strlen(fitaO); ++i) {
        readSymbol = fitaO[i];

        for (int j = 0; j < numTransitions; ++j) {
            if (transitions[j].state == currentState && transitions[j].read_symbol == readSymbol) {
                currentState = executeTransition(currentState, readSymbol, transitions[j].push_symbol, transitions[j].pop_symbol, &nextSymbol, num_caract);

                if (nextSymbol == '|') {
                    stack[++top] = 'T';
                } else if (nextSymbol == '.') {
                    if (top >= 0) {
                        char success = stack[top--];
                        vehicles[*numVehicles].success = success;
                        (*numVehicles)++;
                    }
                }
            }
        }
    }

    char* result_o = transformResult(fitaO);

    // Imprime as informações para cada veículo gerado
    for (int i = 0; i < *numVehicles; ++i) {
        printf("Veículo %d - isigla - Modelo: %c Características: %s Montagem: %s\n",
               i + 1, vehicles[i].nome_modelo, (num_caract == 0) ? "nenhuma" : "alguma", (vehicles[i].success == 'T') ? "com sucesso" : "com falhas");
    }

    free(result_o);
}

char executeTransition(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol, int num_caract) {
    if (num_caract == 0) {
        transitionsForNumCaract0(currentState, readSymbol, pushSymbol, popSymbol, nextSymbol);
    } else if (num_caract == 1) {
        transitionsForNumCaract1(currentState, readSymbol, pushSymbol, popSymbol, nextSymbol);
    } else if (num_caract == 2) {
        transitionsForNumCaract2(currentState, readSymbol, pushSymbol, popSymbol, nextSymbol);
    }
}

void transitionsForNumCaract0(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol) {
    // transições para num_caract = 0.
    if (currentState == 'q0' && readSymbol == 'a') {
        *nextSymbol = '&';  
    } else if (currentState == 'q0' && readSymbol == '&') {
        *nextSymbol = 'a';  
    } else if (currentState == 'q0' && readSymbol == 'v') {
        *nextSymbol = '&';  
    } else if (currentState == 'q0' && readSymbol == 'f') {
        *nextSymbol = '&';  
    } else if (currentState == 'qv' && readSymbol == '|') {
        *nextSymbol = 'T';  
    } else if (currentState == 'qv' && readSymbol == '.') {
        *nextSymbol = 'T';  
    } else if (currentState == 'qf' && readSymbol == '|') {
        *nextSymbol = 'F';  
    } else if (currentState == 'qf' && readSymbol == '.') {
        *nextSymbol = 'F';  
    }
}

void transitionsForNumCaract1(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol) {
    // transições para num_caract = 1.
    if (currentState == 'q0' && readSymbol == 'a') {
        *nextSymbol = '&';  
    } else if (currentState == 'q0' && readSymbol == '&') {
        *nextSymbol = 'a';  
    } else if (currentState == 'q0' && readSymbol == 'x') {
        *nextSymbol = '&';  
    } else if (currentState == 'q0' && readSymbol == 'n') {
        *nextSymbol = '&';  
    } else if (currentState == 'qx' && readSymbol == 'v') {
        *nextSymbol = '&';  
    } else if (currentState == 'qx' && readSymbol == 'f') {
        *nextSymbol = '&'; 
    } else if (currentState == 'qn1' && readSymbol == 'v') {
        *nextSymbol = '&';  
    } else if (currentState == 'qn1' && readSymbol == 'f') {
        *nextSymbol = '&';  
    } else if (currentState == 'qv' && readSymbol == '|') {
        *nextSymbol = 'T';  
    } else if (currentState == 'qv' && readSymbol == '.') {
        *nextSymbol = 'T';  
    } else if (currentState == 'qf' && readSymbol == '|') {
        *nextSymbol = 'F';  
    } else if (currentState == 'qf' && readSymbol == '.') {
        *nextSymbol = 'F';  
    }
}

void transitionsForNumCaract2(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol) {
    // transições para num_caract = 2.
    if (currentState == 'q0' && readSymbol == 'a') {
        *nextSymbol = '&';
    } else if (currentState == 'q0' && readSymbol == '&') {
        *nextSymbol = 'a';
    } else if (currentState == 'q0' && readSymbol == 'x') {
        *nextSymbol = '&';
    } else if (currentState == 'q0' && readSymbol == 'y') {
        *nextSymbol = '&';
    } else if (currentState == 'q0' && readSymbol == 'n') {
        *nextSymbol = '&';
    } else if (currentState == 'q0' && readSymbol == '.') {
        *nextSymbol = '.';
    } else if (currentState == 'q0' && readSymbol == '|') {
        *nextSymbol = '|';
    } else if (currentState == 'qx' && readSymbol == 'v') {
        *nextSymbol = '&';
    } else if (currentState == 'qx' && readSymbol == 'f') {
        *nextSymbol = '&';
    } else if (currentState == 'qn1' && readSymbol == 'v') {
        *nextSymbol = '&';
    } else if (currentState == 'qn1' && readSymbol == 'f') {
        *nextSymbol = '&';
    } else if (currentState == 'qy' && readSymbol == 'v') {
        *nextSymbol = '&';
    } else if (currentState == 'qy' && readSymbol == 'f') {
        *nextSymbol = '&';
    } else if (currentState == 'qn2' && readSymbol == 'v') {
        *nextSymbol = '&';
    } else if (currentState == 'qn2' && readSymbol == 'f') {
        *nextSymbol = '&';
    } else if (currentState == 'qv' && readSymbol == '|') {
        *nextSymbol = 'T';
    } else if (currentState == 'qv' && readSymbol == '.') {
        *nextSymbol = 'T';
    } else if (currentState == 'qf' && readSymbol == '|') {
        *nextSymbol = 'F';
    } else if (currentState == 'qf' && readSymbol == '.') {
        *nextSymbol = 'F';
    }
}

char* transformResult(char* result, int numVehicles) {
    int result_o_size = 0;
    int tempNumVehicles = numVehicles;
    while (tempNumVehicles > 0) {
        result_o_size += (tempNumVehicles >= 10) ? 4 : 3;
        tempNumVehicles /= 10;
    }
    result_o_size += 1;

    char* result_o = (char*)malloc(result_o_size);

    int result_o_index = 0;
    for (int i = strlen(result) - 1; i >= 0; --i) {
        if (result[i] == 'T' || result[i] == 'F') {
            result_o_index += sprintf(result_o + result_o_index, "%c%d ", result[i], numVehicles);
            numVehicles--;
        }
    }

    result_o[result_o_index] = '\0';

    return result_o;
}

void printReport(Vehicle* vehicles, int numVehicles, int maxProd, int num_caract) {
    printf("Relatório da Produção:\n");
    printf("Número total de veículos produzidos: %d\n", numVehicles);

    if (numVehicles > 0) {
        float percentSuccess = ((float)numVehicles / maxProd) * 100;
        printf("Porcentagem de veículos montados com sucesso: %.2f%%\n", percentSuccess);
    }

    // Informações para cada veículo gerado
    for (int i = 0; i < numVehicles; i++) {
        printf("\nVeículo %d - isigla - Modelo: %c Características: ", i + 1, vehicles[i].nome_modelo);

        if (num_caract == 0) {
            printf("nenhuma");
        } else if (num_caract == 1) {
            if (vehicles[i].caract_1 == 'x' || vehicles[i].caract_1 == 'n') {
                printf("nenhuma");
            } else {
                printf("%c", vehicles[i].caract_1);
            }
        } else if (num_caract == 2) {
            if (vehicles[i].caract_1 == 'x' || vehicles[i].caract_1 == 'n') {
                printf("nenhuma");
            } else {
                printf("%c", vehicles[i].caract_1);
            }

            if (vehicles[i].caract_2 == 'y') {
                printf("%c", vehicles[i].caract_2);
            }
        }

        printf(" Montagem %s\n", (vehicles[i].success == 'T' ? "com sucesso" : "com falhas"));
    }
}