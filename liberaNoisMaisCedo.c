#include <stdio.h>
 
#define N_ESTADOS 3
#define N_DIAS 8 
 
 
void simularSemana()
{
    const char *nomesEstados[N_ESTADOS] = {"Sol", "Chuva", "Nublado"};
    const char *nomesDias[N_DIAS] = {
        "Domingo)", "Segunda", "Terca", "Quarta",
        "Quinta", "Sexta", "Sabado", "Domingo"
    };
 
    double P[N_ESTADOS][N_ESTADOS]; /* matriz de transicao */
    double dist[N_DIAS][N_ESTADOS]; /* distribuicao de probabilidade por dia */
    int estadoInicial;
    int i, j, dia;
    int opcao;
    int matrizPronta = 0; /* controla se ja existe uma matriz valida preenchida */
 
    do {
        /* ---------- ENTRADA: matriz de transicao (somente se necessario) ---------- */
        if (!matrizPronta) {
            printf("=== Configuracao da Cadeia de Markov ===\n\n");
            printf("Estados: 0=Sol  1=Chuva  2=Nublado\n\n");
            printf("Informe a matriz de transicao.\n");
            printf("Para cada estado ATUAL (linha), informe as 3 probabilidades\n");
            printf("de ir para Sol, Chuva e Nublado. A soma da linha\n");
            printf("DEVE dar 1.0 (tolerancia de 0.001), ou sera pedido novamente.\n\n");
 
            for (i = 0; i < N_ESTADOS; i++) {
                double soma;
                int linhaValida = 0;
 
                do {
                    soma = 0.0;
                    printf("--- Estado atual: %s ---\n", nomesEstados[i]);
                    for (j = 0; j < N_ESTADOS; j++) {
                        printf("  P(%s -> %s) = ", nomesEstados[i], nomesEstados[j]);
                        scanf("%lf", &P[i][j]);
                        soma += P[i][j];
                    }
                    printf("  (soma da linha = %.4f)\n", soma);
 
                    if (soma < 0.999 || soma > 1.001) {
                        printf("  [Erro] A soma precisa ser 1.0. Preencha essa linha novamente.\n\n");
                    } else {
                        linhaValida = 1;
                        printf("  [OK] Linha valida.\n\n");
                    }
                } while (!linhaValida);
            }
 
            matrizPronta = 1;
        }
 
        // Estado inicial
        printf("=== Estado inicial (Domingo) ===\n");
        printf("Escolha o estado inicial:\n");
        for (i = 0; i < N_ESTADOS; i++) {
            printf("  %d - %s\n", i, nomesEstados[i]);
        }
        printf("Opcao: ");
        scanf("%d", &estadoInicial);
 
        if (estadoInicial < 0 || estadoInicial >= N_ESTADOS) {
            printf("\nEstado invalido. Assumindo Sol (0) como padrao.\n");
            estadoInicial = 0;
        }
 
        /* ---------- INICIALIZACAO do vetor de distribuicao (dia 0) ---------- */
        for (j = 0; j < N_ESTADOS; j++) {
            dist[0][j] = (j == estadoInicial) ? 1.0 : 0.0;
        }
 
        /* ---------- CALCULO: propagacao dia a dia ---------- */
        /* dist[dia+1][j] = soma_i( dist[dia][i] * P[i][j] ) */
        for (dia = 0; dia < N_DIAS - 1; dia++) {
            for (j = 0; j < N_ESTADOS; j++) {
                double acumulado = 0.0;
                for (i = 0; i < N_ESTADOS; i++) {
                    acumulado += dist[dia][i] * P[i][j];
                }
                dist[dia + 1][j] = acumulado;
            }
        }
 
        /* ---------- SAIDA: tabela de porcentagens ---------- */
        printf("\n=== Probabilidade de cada estado por dia (Domingo -> Domingo) ===\n\n");
        printf("%-18s", "Dia");
        for (j = 0; j < N_ESTADOS; j++) {
            printf("%-12s", nomesEstados[j]);
        }
        printf("\n");
 
        for (dia = 0; dia < N_DIAS; dia++) {
            printf("%-18s", nomesDias[dia]);
            for (j = 0; j < N_ESTADOS; j++) {
                printf("%-12.2f", dist[dia][j] * 100.0);
            }
            printf("\n\n\n\n");
        }
 
        /* ---------- MENU FINAL ---------- */
        printf("=== O que deseja fazer agora? ===\n");
        printf("  1 - Rodar de novo com a MESMA matriz e um novo estado inicial\n");
        printf("  2 - Preencher uma NOVA matriz de transicao\n");
        printf("  3 - Sair do programa\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        printf("\n");
 
        if (opcao == 2) {
            matrizPronta = 0; /* forca reentrada da matriz no proximo loop */
        }
 
    } while (opcao == 1 || opcao == 2);
 
    printf("Encerrando o programa. Ate mais!\n");
}
 
int main()
{
    simularSemana();
    return 0;
}