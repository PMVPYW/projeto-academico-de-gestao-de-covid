#include "funcoesGenericas.h"
#include <stdio.h>
#include <string.h>

void limpaBuffer()
{
    char chr;
    do
    {
        chr = getchar();
    }
    while( chr != '\n' && chr != EOF);
}

void escreverData (tipoData data)
{
    printf("%02d-%02d-%d", data.dia, data.mes, data.ano);
}

void lerString(char texto[], int tamanho, char msg[])
{
    int tamStr;

    do
    {
        printf("%s", msg);
        fgets(texto, tamanho, stdin);

        tamStr = strlen(texto);
        if (tamStr == 1) // apenas inseriu o \n - enter
        {
            printf("\nERRO: tem que inserir um valor\n");
        }
    }
    while (tamStr == 1);

    if (texto[tamStr-1] == '\n')
    {
        texto[tamStr-1] = '\0';
    }
    else
    {
        limpaBuffer();
    }
}


int lerInteiro (int limMin, int limMax, char msg[])
{
    int num, controlo;

    do
    {
        printf("%s[%d, %d]: ", msg, limMin, limMax);
        controlo = scanf("%d", &num);
        limpaBuffer();
        if (controlo != 1)
        {
            printf("\nERRO: O valor inserido tem de ser numerico!\n");
        }
        else if (num<limMin || num>limMax)
        {
            printf("\nERRO: valor invalido\n");
        }
    }
    while (num<limMin || num>limMax || controlo != 1);

    return num;
}

int diasDoMes(tipoData data, int warnings)
{
    int diaMax;
    switch(data.mes)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        diaMax = 31;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        diaMax = 30;
        break;
    case 2:
        if (data.ano%400==0 || (data.ano%4==0 && data.ano%100!=0))
        {
            diaMax = 29;
        }
        else
        {
            diaMax = 28;
        }
        break;
    default:
        if (warnings != 0)
        {
            printf("\nERRO: mes invalido\n");
        }
        diaMax = -1;
    }
    return diaMax;
}


tipoData somarData(tipoData data, int dias)
{
    int result;
    int diaMax;
    result = data.dia + dias;
    diaMax = diasDoMes(data, 0);
    while (result > diaMax)
    {
        result-=diaMax;
        data.mes++;
        if (data.mes > 12)
        {
            data.ano++;
            data.mes = 1;
        }
        diaMax = diasDoMes(data, 0);
    }
    data.dia = result;
    return data;
}

tipoData lerData(int anoMin, int anoMax, char msg[])
{
    tipoData data;
    int controlo, diaMax, falha;

    do
    {
        falha = 0;
        do
        {
            printf("%s[dd-mm-aaaa]: ", msg);
            controlo = scanf("%d-%d-%d", &data.dia, &data.mes, &data.ano);
            limpaBuffer();
            if (controlo != 3)
            {
                printf("\nERRO: formato de data incorreto\n");
            }
        }
        while (controlo != 3);

        if (data.ano < anoMin || data.ano > anoMax)
        {
            printf("\nERRO: ano invalido\n");
            falha = 1;
        }
        else
        {
            diaMax = diasDoMes(data, 1);
            if (data.dia < 1 || data.dia > diaMax) //se diaMax == -1 data.dia será sempre maior
            {
                printf("\nERRO: dia invalido\n");
                falha = 1;
            }
        }
    }
    while (falha==1);

    return data;
}

void escreverHora(tipoHora hora)
{
    printf("%02d:%02d", hora.hora, hora.minuto);
}

tipoHora lerHora(int minHora, int maxHora, char msg[])
{
    tipoHora horas;
    int controlo;
    do
    {
        printf("%s[H:M]: ", msg);
        controlo = scanf("%d:%d", &horas.hora, &horas.minuto);
        limpaBuffer();
        if ((horas.hora < minHora || horas.hora > maxHora) || (horas.minuto < 0 || horas.minuto > 59) || controlo != 2 )
        {
            printf("\nEntrada Invalida: tem de ser entre as %d:00 e as %d:59\n", minHora, maxHora);
        }
    }
    while( (horas.hora < minHora || horas.hora > maxHora) || (horas.minuto < 0 || horas.minuto > 59) || controlo != 2 );
    return horas;
}

int menu(int nMembros, int testesAgendados, int testesRealizados, int membrosVacinados)
{
    int opcao;
    int controlo;
    printf("\n\n****************************MENU**********************************\n\n");
    printf("\n\nMembros da comunidade: %02d\t\tTestes agendados: %02d\nTestes realizados: %02d\t\t\tMembros vacinados: %02d\n\n", nMembros, testesAgendados, testesRealizados, membrosVacinados);
    printf("[0] Adicionar 1 Membro\n[1] Adicionar VARIOS membros \n[2] Editar Membro\n[3] Testes\n[4] Dados Estatisticos\n");
    printf("[5] Lista de Confinamento\n[6] Sair\n\n");
    printf("\nOpcao: ");
    do
    {
        controlo = scanf("%d", &opcao);
        limpaBuffer();
        if(controlo != 1)
        {
            printf("Entrada incorreta!\n");
        }
    }
    while(controlo != 1);
    return opcao;
}

int subMenu (void)
{
    int opcao;
    int controlo;
    printf("\n\n******************************SUBMENU TESTES******************************\n\n");
    printf("[0] Agendar a realizacao de um teste\n[1] Reagendar teste \n[2] Introduzir resultados dos testes\n[3] Mostrar informacao de um teste\n");
    printf("[4] Voltar ao menu principal\n");
    printf("\nOpcao: ");
    do
    {
        controlo = scanf("%d", &opcao);
        limpaBuffer();
        if(controlo != 1)
        {
            printf("Entrada incorreta!\n");
        }
    }
    while(controlo != 1);
    return opcao;
}

