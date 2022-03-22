#ifndef TESTESDIAGNOSTICO_H_INCLUDED
#define TESTESDIAGNOSTICO_H_INCLUDED

#include "funcoesGenericas.h"
#include "comunidadeAcademica.h"

#define NEGATIVO 0
#define POSITIVO 1
#define INCONCLUSIVO 2

#define MAXDAILYPCR 15




typedef struct
{
    int minutos, segundos;
} tipoTempoColheita;

typedef struct
{
    char designacaoTeste[8];
    char tipoTeste[10];
    int nUtenteSaude;
    tipoData dataRealizacao;
    int resultado;  //0 negativo - 1 positivo
    tipoHora horaColheita;
    tipoTempoColheita tempoColheita;  //em minutos
} testeCovid;

tipoConfinamento *adicionarConfinamento(tipoConfinamento *vetor, int *nConfinamentos, membro novoConfinado);
void gravarConfinamentos(tipoConfinamento *vetor, int *numConfinamentos);
tipoConfinamento *lerConfinamentos(int *numConfinamentos);
int contaTestesAgendados(testeCovid vetorTestes[], int nTestes);
int contaTestesRealizados(testeCovid vetorTestes[], int nTestes);
void mostraTempoColheita(testeCovid teste);
testeCovid *agendarTeste(int *nTestes, testeCovid *vetorTestes, int nMembros, membro vetorMembros[]);
tipoTempoColheita lerTempoColheita();
void lerTipoTeste(char string[], int nPCR);
int localizarTeste(testeCovid vetorTestes[], char nIdentificacao[],int nTestes);
void reagendarTeste(testeCovid vetorTestes[], int nTestes);
void printTeste(testeCovid vetorTestes[], int nTestes, membro vetorMembros[], int nMembros);
tipoConfinamento *introduzirResultadoTeste(testeCovid vetorTestes[], int *nTestes, membro vetorMembros[], int nMembros, tipoConfinamento *vetorConfinamentos, int *confinamentos, int testesAgendados, char codigoTeste[]);
void gravarTestes(testeCovid vetorTestes[], int nTestes);
testeCovid *lerTestes(int *nTestes);
void criarLog(testeCovid teste, char msg[]);
int quantTestesPositivos (int nUtente, testeCovid testevetor[], int nTestesRealizados);
int quantTestes (testeCovid testeVetor[], int nTestesRealizados, int resultado);
void dadosEstatisticos (testeCovid testeVetor[], membro vetorMembro[], int testesRealizados, int nMembros);
void mediaTempoColheita (testeCovid vetorTeste[], int testesRealizados);
void dataTesteMaisRecente (testeCovid vetorTeste[], int testesRealizados);
void membroComunidadeAcademicaMenosTestes (membro vetorMembro[], int testesRealizados, int nMembros);
testeCovid *agendarTestePCR(int *nTestes, testeCovid *vetorTestes, int numUtente);
int contaTestesPCRDia(testeCovid *vetorTestes, int nTestes, tipoData data);
#endif // TESTESDIAGNOSTICO_H_INCLUDED
