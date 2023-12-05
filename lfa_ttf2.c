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
    char* symbol;
    int index;
} SymbolIndex;

typedef struct {
    char state;
    char read_symbol;
    char push_symbol;
    char pop_symbol;
    char next_state;
} Transition;

void generateFitaSiglas(char* fitaSiglas, int numCaract, int chanceFalha);
void separateSiglas(char* fitaSiglas, SymbolIndex* symbolIndex, int* numSymbols);
void runAutomatonWithStack(char* fitaO, Transition* transitions, int numTransitions, Vehicle* vehicles, int* numVehicles);
void executeTransition(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol, int num_caract);
void transitionsForNumCaract0(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol);
void transitionsForNumCaract2(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol);
void transitionsForNumCaract1(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol);

int main() {
    int numLines, maxProd, numCaract, chanceFalha;

    printf("Quantas linhas devem funcionar no dia? (Max 4): ");
    scanf("%d", &numLines);

    if (num_linhas <= 0 || num_linhas > 4) {
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

    if(numCaract < 0 || numCaract > 2) {
        printf("Valor invalido para número de caracteristicas. Encerrando o programa.");
        return 1;
    }

    if (numCaract == 1) {
        printf("Qual o nome da primeira característica?");
        scanf("%d", &caract_1);
    } else if (numCaract == 2) {
        printf("Qual o nome da segunda característica?");
        scanf("%d", &caract_2);
    }

    printf("Qual a chance de erro na montagem do veículo? (0 até 100): ");
    scanf("%d", &chanceFalha);

    if(chanceFalha < 0 && chanceFalha > 100) {
        printf("Valor invalido para chance de Falha. Encerrando o programa.");
        return 1;
    }

    char fitaSiglas[100];  
    generateFitaSiglas(fitaSiglas, numCaract, chanceFalha);

    SymbolIndex symbolIndex[10];  
    int numSymbols;
    separateSiglas(fitaSiglas, symbolIndex, &numSymbols);

    Transition transitions[100];  

    Vehicle vehicles[MAX_PROD];  
    int numVehicles = 0;

    runAutomatonWithStack(symbolIndex[0].symbol, transitions, 10, vehicles, &numVehicles);

    printReport(vehicles, numVehicles, maxProd);
        printf("Produção da Linha %d:\n", num_linhas);
    for (int i = 0; i < numVehicles && i < max_prod; ++i) {
        printf("Veículo %d - %s - Modelo %s Características %s Montagem %s\n", i + 1, vehicles[i].sigla, nome_modelo,
               vehicles[i].caracts, vehicles[i].success == 'T' ? "com sucesso" : "com falhas");
    }

    return 0;
}

void generateFitaSiglas(int num_caract, int chance_falha, char* fita_siglas) {

    srand(time(NULL));  

    int num_siglas = rand() % 10 + 1;  //número de 1 a 10 siglas

    for (int i = 0; i < num_siglas; ++i) {
        fita_siglas[strlen(fita_siglas)] = generateSigla(num_caract, chance_falha);  
        if (i < num_siglas - 1) {
            fita_siglas[strlen(fita_siglas)] = '|';  // adiciona "|" entre as siglas, exceto para a última
        }
    }

    fita_siglas[strlen(fita_siglas)] = '.';  // adiciona "." ao final da fita
}

char generateSigla(int num_caract, int chance_falha) {
    char sigla = 'a';

    if (num_caract == 1 || num_caract == 2) {
        if (rand() % 100 < chance_falha) {
            sigla = 'f';  // adiciona 'f' com probabilidade igual à chance_falha
        }
    }

    if (num_caract == 2) {
        if (rand() % 2 == 0) {
            sigla = 'x';  // adiciona 'x' com probabilidade 50%
        } else {
            sigla = 'y';  // adiciona 'y' com probabilidade 50%
        }
    }

    return sigla;
}

void runAutomatonWithStack(char* fitaO, Transition* transitions, int numTransitions, Vehicle* vehicles, int* numVehicles) {
    char stack[100];  
    int top = -1;      

    char currentState = 'q0';  
    char readSymbol, pushSymbol, popSymbol, nextSymbol;

    for (int i = 0; i < strlen(fitaO); ++i) {
        readSymbol = fitaO[i];

        for (int j = 0; j < numTransitions; ++j) {
            if (transitions[j].state == currentState && transitions[j].read_symbol == readSymbol) {
                currentState = executeTransition(currentState, readSymbol, transitions[j].push_symbol, transitions[j].pop_symbol, &nextSymbol);

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
}

void executeTransition(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol, int num_caract) {
    if (num_caract == 0) {
        transitionsForNumCaract0(currentState, readSymbol, pushSymbol, popSymbol, nextSymbol);
    } else if (num_caract == 1) {
        transitionsForNumCaract1(currentState, readSymbol, pushSymbol, popSymbol, nextSymbol);
    } else if (num_caract == 2) {
        transitionsForNumCaract2(currentState, readSymbol, pushSymbol, popSymbol, nextSymbol);
    }
}

// transições do autômato com pilha para num_caract = 0
void transitionsForNumCaract0(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol) {
    if (currentState == 'q0' && readSymbol == 'a') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'q0' && readSymbol == '&') {
        *nextSymbol = 'a';  // Desempilha vazio.
    } else if (currentState == 'q0' && readSymbol == 'v') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'q0' && readSymbol == 'f') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qv' && readSymbol == '|') {
        *nextSymbol = 'T';  // Adiciona 'T' à pilha.
    } else if (currentState == 'qv' && readSymbol == '.') {
        *nextSymbol = 'T';  // Adiciona 'T' à pilha.
    } else if (currentState == 'qf' && readSymbol == '|') {
        *nextSymbol = 'F';  // Adiciona 'F' à pilha.
    } else if (currentState == 'qf' && readSymbol == '.') {
        *nextSymbol = 'F';  // Adiciona 'F' à pilha.
    }
}

// transições do autômato com pilha para num_caract = 1
void transitionsForNumCaract1(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol) {
    // Implemente as transições específicas para num_caract = 1.
    if (currentState == 'q0' && readSymbol == 'a') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'q0' && readSymbol == '&') {
        *nextSymbol = 'a';  // Desempilha vazio.
    } else if (currentState == 'q0' && readSymbol == 'x') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'q0' && readSymbol == 'n') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qx' && readSymbol == 'v') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qx' && readSymbol == 'f') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qn1' && readSymbol == 'v') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qn1' && readSymbol == 'f') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qv' && readSymbol == '|') {
        *nextSymbol = 'T';  // Adiciona 'T' à pilha.
    } else if (currentState == 'qv' && readSymbol == '.') {
        *nextSymbol = 'T';  // Adiciona 'T' à pilha.
    } else if (currentState == 'qf' && readSymbol == '|') {
        *nextSymbol = 'F';  // Adiciona 'F' à pilha.
    } else if (currentState == 'qf' && readSymbol == '.') {
        *nextSymbol = 'F';  // Adiciona 'F' à pilha.
    }
}

// transições do autômato com pilha para num_caract = 2
void transitionsForNumCaract2(char currentState, char readSymbol, char pushSymbol, char popSymbol, char* nextSymbol) {
    if (currentState == 'q0' && readSymbol == 'a') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'q0' && readSymbol == '&') {
        *nextSymbol = 'a';  // Desempilha vazio.
    } else if (currentState == 'q0' && readSymbol == 'x') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'q0' && readSymbol == 'y') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'q0' && readSymbol == 'n') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'q0' && readSymbol == '.') {
        *nextSymbol = '.';  // Mantém o vazio.
    } else if (currentState == 'q0' && readSymbol == '|') {
        *nextSymbol = '|';  // Mantém o vazio.
    } else if (currentState == 'qx' && readSymbol == 'v') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qx' && readSymbol == 'f') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qn1' && readSymbol == 'v') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qn1' && readSymbol == 'f') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qy' && readSymbol == 'v') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qy' && readSymbol == 'f') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qn2' && readSymbol == 'v') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qn2' && readSymbol == 'f') {
        *nextSymbol = '&';  // Empilha vazio.
    } else if (currentState == 'qv' && readSymbol == '|') {
        *nextSymbol = 'T';  // Adiciona 'T' à pilha.
    } else if (currentState == 'qv' && readSymbol == '.') {
        *nextSymbol = 'T';  // Adiciona 'T' à pilha.
    } else if (currentState == 'qf' && readSymbol == '|') {
        *nextSymbol = 'F';  // Adiciona 'F' à pilha.
    } else if (currentState == 'qf' && readSymbol == '.') {
        *nextSymbol = 'F';  // Adiciona 'F' à pilha.
    }
}
