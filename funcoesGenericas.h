#ifndef FUNCOESGENERICAS_H_INCLUDED
#define FUNCOESGENERICAS_H_INCLUDED



typedef struct
{
    int dia,mes,ano;
} tipoData;

typedef struct
{
    int hora, minuto;
} tipoHora;

int lerInteiro (int limMin, int limMax, char msg[]);
void lerString(char texto[], int tamanho, char msg[]);
tipoHora lerHora(int minHora, int maxHora, char msg[]);
tipoData lerData(int anoMin, int anoMax, char msg[]);
void limpaBuffer();
int menu(int nMembros, int testesAgendados, int testesRealizados, int membrosVacinados);
void escreverData (tipoData data);
void escreverHora(tipoHora hora);
int diasDoMes(tipoData data, int warnings);
tipoData somarData(tipoData data, int dias);
int subMenu (void);

#endif // FUNCOESGENERICAS_H_INCLUDED
