#ifndef COMUNIDADEACADEMICA_H_INCLUDED
#define COMUNIDADEACADEMICA_H_INCLUDED

#include "funcoesGenericas.h"

#define MAXSTRING 50
#define MAXMEMBERS 200

typedef struct
{
    int nUtenteSaude;
    tipoData dataConfinamento;
    tipoData dataFimConfinamento;
    int duracao;
    char estadoConfinamento[23];
} tipoConfinamento;

typedef struct
{
    int nUtenteSaude;   //nº utente de saude
    char nome[MAXSTRING];
    char tipoMembro[10];
    int anoNascimento;
    char estadoConfinamento[23];
    int estadoVacinacao;
    tipoData ultimaVacina;
    int duracaoConfinamento;
    tipoData dataConfinamento;
    tipoData dataFimConfinamento;
    int testesRealizados;
} membro;

void criarLogMembros(membro member, char msg[]);
void gravarMembros(membro vetor[], int nMembros);
void lerMembros(membro vetor[], int *nMembro);
tipoConfinamento *novoMembro(membro vetor[], int* nMembers, tipoConfinamento vetorConfinamento[], int *confinamentos);
void printMembro(membro target);
int lernUtentesaude();
int localizarMembro(int nUtenteSaude, membro vetorMembros[], int nMembros);
tipoConfinamento *editarMembro(int position, membro vetor[], int nMembers, tipoConfinamento *vetorConfinamento, int *confinamentos);
void lerTipomembro(char tipo[]);
int lerAnoNascimento();
tipoConfinamento *lerEstadoConfinamento(membro *membroSelecionado, tipoConfinamento *vetorConfinamento, int *confinamentos);
int lerEstadoVacinacao();
tipoData lerDataUltimaVacina(membro member);
int contaMembrosVacinados(int nMembros, membro vetorMembros[]);
int tipodeConfinamentoQuarentena (tipoConfinamento vetorConfinamentos[], int confinamentos, membro vetorMembros[], int nMembros);
int tipodeConfinamentoIso (tipoConfinamento vetorConfinamentos[], int confinamentos,membro vetorMembros[], int nMembros, int print);
void quantidadeTipoMembro (membro vetorMembros[], int nMembros);
void printConfinamento (membro membroTarget, tipoConfinamento confinamentoTarget);
#endif // COMUNIDADEACADEMICA_H_INCLUDED
