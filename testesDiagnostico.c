#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoesGenericas.h"
#include "testesDiagnostico.h"

int contaTestesPCRDia(testeCovid *vetorTestes, int nTestes, tipoData data)
{
    int i, n;
    int control;
    n = 0;
    for (i = 0; i< nTestes; i++)
    {
        control = strcmp(vetorTestes[i].tipoTeste, "PCR");
        if(vetorTestes[i].dataRealizacao.dia == data.dia && vetorTestes[i].dataRealizacao.mes == data.mes && vetorTestes[i].dataRealizacao.ano == data.ano &&  control == 0)
        {
            n++;
        }
    }
    return n;
}

testeCovid *agendarTestePCR(int *nTestes, testeCovid *vetorTestes, int numUtente)
{
    int position;
    char numeroTeste[8];
    testeCovid *backup;
    backup = vetorTestes;

    vetorTestes = realloc(vetorTestes, ((*nTestes)+1)*sizeof(testeCovid));
    if (vetorTestes == NULL)
    {
        printf("Erro, nao e possivel agendar mais testes\n");
        return backup;
    }
    else
    {
        do
        {
            lerString(numeroTeste,8,"\n\nIntroduza o numero do teste [1-100000]: ");
            strcpy(vetorTestes[*nTestes].designacaoTeste,numeroTeste);
            position = localizarTeste(vetorTestes, vetorTestes[*nTestes].designacaoTeste, *nTestes);
            if (position != -1)
            {
                printf("\nEsse teste ja existe!\n");
            }
        }
        while(position != -1);
        strcpy(vetorTestes[*nTestes].tipoTeste, "PCR");
        vetorTestes[*nTestes].nUtenteSaude = numUtente;
        vetorTestes[*nTestes].dataRealizacao = lerData(2020, 2030, "\nData do teste");
        vetorTestes[*nTestes].horaColheita = lerHora(0, 23, "\nHora do teste");
        vetorTestes[*nTestes].resultado = -1;
        criarLog(vetorTestes[*nTestes], "TESTE AGENDADO");
        printf("\nTESTE PCR AGENDADO COM SUCESSO\n");
        (*nTestes)++;
    }
    return vetorTestes;
}



tipoConfinamento *adicionarConfinamento(tipoConfinamento *vetor, int *nConfinamentos, membro novoConfinado)
{
    tipoConfinamento *backup;
    backup = vetor;

    vetor = realloc(vetor, sizeof(tipoConfinamento)*(*nConfinamentos+1));

    if (vetor == NULL)
    {
        printf("ERRO: Impossivel adicionar novo confinamento!\n");
        vetor = backup;
    }
    else
    {
        vetor[*nConfinamentos].nUtenteSaude = novoConfinado.nUtenteSaude;
        vetor[*nConfinamentos].dataConfinamento = novoConfinado.dataConfinamento;
        vetor[*nConfinamentos].dataFimConfinamento = novoConfinado.dataFimConfinamento;
        vetor[*nConfinamentos].duracao = novoConfinado.duracaoConfinamento;
        strcpy(vetor[*nConfinamentos].estadoConfinamento, novoConfinado.estadoConfinamento);
        (*nConfinamentos)++;
    }
    return vetor;
}

void gravarConfinamentos(tipoConfinamento *vetor, int *numConfinamentos)
{
    int controlo;
    FILE *ficheiro;

    ficheiro = fopen("confinamentos.bin", "wb");
    if (ficheiro == NULL)
    {
        printf("ERRO ao escrever confinamentos\n");
    }
    else
    {
        controlo = fwrite(numConfinamentos, sizeof(int), 1, ficheiro);
        if (controlo != 1)
        {
            printf("Erro de escrita da quantidade de confinamentos!\n");
        }
        else
        {

            controlo = fwrite(vetor, sizeof(tipoConfinamento), *numConfinamentos, ficheiro);
            if (controlo != *numConfinamentos)
            {
                printf("Erro ao escrever confinamentos!\n");
            }

        }
        fclose(ficheiro);
    }
}


tipoConfinamento *lerConfinamentos(int *numConfinamentos)
{
    int controlo;
    FILE *ficheiro;
    tipoConfinamento *vetor;

    vetor = NULL;
    ficheiro = fopen("confinamentos.bin", "rb");
    if (ficheiro == NULL)
    {
        printf("\nERRO: Nao foi possivel ler confinamentos!\n");
    }
    else
    {
        controlo = fread(numConfinamentos, sizeof(int), 1, ficheiro);
        if (controlo != 1)
        {
            printf("\nErro de leitura dos confinamentos!\n");
        }
        else
        {
            vetor = realloc(vetor, (*numConfinamentos)*sizeof(tipoConfinamento));

            if (vetor == NULL && *numConfinamentos>0)
            {
                printf("\nErro ao carregar quantidade de confinamentos!\n");
            }
            else
            {
                controlo = fread(vetor, sizeof(tipoConfinamento), *numConfinamentos, ficheiro);
                if (controlo != *numConfinamentos)
                {
                    printf("\nErro ao carregar confinamentos!\n");
                }
            }
        }
        fclose(ficheiro);
    }
    return vetor;
}

void membroComunidadeAcademicaMenosTestes (membro vetorMembro[], int testesRealizados, int nMembros)
{
    int i, quantTestesAnt=999999999;

    if (testesRealizados!=0)
    {
        for(i=0; i< nMembros; i++)
        {
            if (vetorMembro[i].testesRealizados < quantTestesAnt && vetorMembro[i].testesRealizados != 0)
            {
                quantTestesAnt = vetorMembro[i].testesRealizados;
            }
        }
        printf("O menor numero de testes realizados foi : %d", quantTestesAnt);

        for(i=0; i<nMembros; i++)
        {
            if(vetorMembro[i].testesRealizados == quantTestesAnt)
            {
                printMembro(vetorMembro[i]);
            }
        }
    }
    else
    {
        printf("\nImpossivel calcular o membro com o menor numero de testes realizados - 0 Testes Realizados\n\n");
    }
}

void dataTesteMaisRecente (testeCovid vetorTeste[], int testesRealizados)
{
    // somar anos+mes+dia , problema que ha varias combinações que dao o mesmo resultado
    // data --- 01/02/2020 --- se der para meter no formato 20200201 -- permite dar sort pela mais elevada que equivale a data mais recente

    int i, ano, mes, dia, data, dataAnt=0;
    int anoMax, mesMax, diaMax;
    int testeMsmDia=0;

    if (testesRealizados!=0)
    {
        for (i=0; i<testesRealizados; i++)
        {
            ano = vetorTeste[i].dataRealizacao.ano;
            mes = vetorTeste[i].dataRealizacao.mes;
            dia = vetorTeste[i].dataRealizacao.dia;

            data = (ano*1000)+(mes*100)+dia;

            if (data > dataAnt)
            {
                anoMax = ano;
                mesMax = mes;
                diaMax = dia;
            }

            dataAnt = data;
        }

        // verificar se ha mais do que um teste no mesmo dia

        for (i=0; i<testesRealizados; i++)
        {

            if (anoMax == vetorTeste[i].dataRealizacao.ano && mesMax == vetorTeste[i].dataRealizacao.mes && diaMax == vetorTeste[i].dataRealizacao.dia)
            {
                testeMsmDia++;
            }

        }

        printf("Teste(s) mais recente(s) : ");
        if (testeMsmDia==1)
        {
            printf("\n\nO teste mais recente foi realizado na data : %02d-%02d-%d\n\n", diaMax, mesMax, anoMax);
        }
        else
        {
            printf("\n\nOs %d testes mais recentes foram realizados na data : %02d-%02d-%d\n\n", testeMsmDia, diaMax, mesMax, anoMax);
        }
    }
    else
    {
        printf("\nImpossivel calcular a data do teste mais recente - 0 Testes Realizados\n\n");
    }
}

void mediaTempoColheita (testeCovid vetorTeste[], int testesRealizados)
{
    int i;
    int somamin=0, somaseg=0, Min, Seg;
    int media;
    int minutos,segundos;
    if (testesRealizados!=0)
    {
        for(i=0; i<testesRealizados; i++)
        {
            Min = vetorTeste[i].tempoColheita.minutos;
            Min = 60*Min ;// minutos para segundos
            Seg = vetorTeste[i].tempoColheita.segundos;
            somamin = somamin + Min;
            somaseg = somaseg + Seg;
        }



        media = (somamin+somaseg)/testesRealizados; // media vai ficar em segundos

        minutos =  media / 60;
        segundos = media % 60;

        printf("Tempo Medio de Colheita : %d min %d seg\n\n", minutos, segundos);

    }
    else
    {
        printf("Impossivel calcular a media do Tempo de Colheita - 0 Testes Realizados\n\n");
    }
}


void dadosEstatisticos (testeCovid vetorTeste[], membro vetorMembro[],int testesRealizados, int nMembros)
{
    int testesInconclusivos;
    float percTestesInconclusivos = 0;

    printf("\n\n*****************************************DADOS ESTATISTICOS*****************************************");

    testesInconclusivos = quantTestes(vetorTeste, testesRealizados, INCONCLUSIVO);
    if(testesRealizados==0)
    {
        printf("\n\nMedia dos testes Inconclusivos : 0 Testes Realizados\n\n\n");
    }
    else
    {
        if (testesInconclusivos!=0)
        {
            percTestesInconclusivos = (testesInconclusivos*100.0)/testesRealizados;
            printf("\n\nMedia dos testes Inconclusivos = %2.f %%\n\n", percTestesInconclusivos);
        }
        else
        {
            printf("\n\nMedia dos testes Inconclusivos : Nenhum teste teve como resultado inconclusivo\n\n");
        }
    }

    mediaTempoColheita (vetorTeste, testesRealizados);

    dataTesteMaisRecente (vetorTeste, testesRealizados);

    membroComunidadeAcademicaMenosTestes (vetorMembro,  testesRealizados, nMembros);

}


int quantTestes (testeCovid testeVetor[], int nTestesRealizados, int resultado)
{
    int i;
    int quantTestes=0;
    for (i=0; i<nTestesRealizados; i++)
    {
        if (testeVetor[i].resultado == resultado)
        {
            quantTestes++;
        }
    }

    return quantTestes;
}

int contaTestesAgendados(testeCovid vetorTestes[], int nTestes)
{
    int i;
    int num;
    num = 0;
    for (i = 0; i < nTestes; i++)
    {
        if (vetorTestes[i].resultado == -1)
        {
            num++;
        }
    }
    return num;
}

int contaTestesRealizados(testeCovid vetorTestes[], int nTestes)
{
    int i;
    int num;
    num = 0;
    for (i = 0; i < nTestes; i++)
    {
        if (vetorTestes[i].resultado != -1)
        {
            num++;
        }
    }
    return num;
}

void criarLog(testeCovid teste, char msg[])
{
    FILE *ficheiro;
    ficheiro = fopen("log.txt", "a");
    if (ficheiro == NULL)
    {
        printf("\nERRO ao gerar log!\n");
    }
    else
    {
        fprintf(ficheiro, "============%s============\n", msg);
        fprintf(ficheiro, "Designacao do teste: %s\nTipo de teste: %s\nNumero de utente: %d\n", teste.designacaoTeste, teste.tipoTeste, teste.nUtenteSaude);
        fprintf(ficheiro, "Data de realizacao: %02d-%02d-%d\n", teste.dataRealizacao.dia,teste.dataRealizacao.mes, teste.dataRealizacao.ano);
        fprintf(ficheiro, "Hora agendada: %02d : %02d\n", teste.horaColheita.hora, teste.horaColheita.minuto);
        switch (teste.resultado)
        {
        case -1:
            fprintf(ficheiro, "Resultado: Teste nao realisado\n");
            fprintf(ficheiro, "Tempo de colheita: Nao Aplicavel\n\n\n");
            break;
        case INCONCLUSIVO:
            fprintf(ficheiro,"Resultado: Inconclusivo\n");
            fprintf(ficheiro, "Tempo de colheita: %02d minutos e %02d segundos\n\n\n", teste.tempoColheita.minutos, teste.tempoColheita.segundos);
            break;
        case NEGATIVO:
            fprintf(ficheiro, "Resultado: Negativo\n");
            fprintf(ficheiro, "Tempo de colheita: %02d minutos e %02d segundos\n\n\n", teste.tempoColheita.minutos, teste.tempoColheita.segundos);
            break;
        case POSITIVO:
            fprintf(ficheiro, "Resultado: Positivo\n");
            fprintf(ficheiro, "Tempo de colheita: %02d minutos e %02d segundos\n\n\n", teste.tempoColheita.minutos, teste.tempoColheita.segundos);
            break;
        }
    }
    fclose(ficheiro);
}

void gravarTestes(testeCovid vetorTestes[], int nTestes)
{
    int controlo;
    FILE *ficheiro;
    ficheiro = fopen("testes.bin", "wb");
    if (ficheiro == NULL)
    {
        printf("\nERRO: ficheiro nao criado!\n");
    }
    else
    {
        controlo = fwrite(&nTestes, sizeof(int), 1, ficheiro);
        if (controlo != 1)
        {
            printf("\nErro: erro de escrita 3\n");
        }
        else
        {
            controlo = fwrite(vetorTestes, sizeof(testeCovid), nTestes, ficheiro);
            if (controlo != nTestes)
            {
                printf("\nErro: erro de escrita 4\n");
            }
        }
        fclose(ficheiro);
    }
}

testeCovid *lerTestes(int *nTestes)
{
    int controlo;
    testeCovid *vetorTestes;
    vetorTestes = NULL;
    FILE *ficheiro;
    ficheiro = fopen("testes.bin", "rb");
    if (ficheiro == NULL)
    {
        printf("\nERRO: nao foi possivel ler o ficheiro!\n");
    }
    else
    {
        controlo = fread(nTestes, sizeof(int), 1, ficheiro);
        if (controlo != 1)
        {
            printf("\nErro: erro de leitura 3\n");
        }
        else
        {
            vetorTestes = realloc(vetorTestes, *nTestes*sizeof(testeCovid));
            if (vetorTestes == NULL)
            {
                printf("Erro de mamoria\n");
            }
            else
            {
                controlo = fread(vetorTestes, sizeof(testeCovid), *nTestes, ficheiro);
                if (controlo != *nTestes)
                {
                    printf("\nErro: erro de leitura 4\n");
                }
            }
        }
        fclose(ficheiro);
    }
    return vetorTestes;
}

void mostraTempoColheita(testeCovid teste)
{
    printf("%d minutos e %d segundos", teste.tempoColheita.minutos, teste.tempoColheita.segundos);
}


tipoConfinamento *introduzirResultadoTeste(testeCovid vetorTestes[], int *nTestes, membro vetorMembros[], int nMembros, tipoConfinamento *vetorConfinamentos, int *confinamentos, int testesAgendados, char codigoTeste[])
{
    int quantidade;
    int i;
    char numeroteste[8];
    int pos;
    int resultado;
    int pos_member;
    int controlo;
    int mesmoTeste;
    int respostaContactos, numeroPessoas, num, posContacto;
    int negativoMudanca = NEGATIVO;

    if (testesAgendados == 0)
    {
        printf("Nao existem testes agendados!\n");
    }
    else
    {
        quantidade = lerInteiro(1,testesAgendados,"\n\nQuantos resultados pretende introduzir\n");
        for (i=0; i<quantidade; i++)
        {
            printf("\n\n############### TESTE NUMERO %d ###############\n\n", i+1);
            do
            {
                lerString (numeroteste, 8, "\nN. de identificacao do teste [1-100000]: ");
                pos = localizarTeste(vetorTestes, numeroteste, *nTestes);
                if (pos == -1)
                {
                    printf("\n\nERRO - Teste nao existe\n\n");
                }
                else
                {


                    if (vetorTestes[pos].resultado != -1)
                    {
                        printf("\n\nERRO - O resultado do respetivo teste ja se encontra introduzido\n\n");
                        mesmoTeste = -1;

                    }
                    else
                    {
                        mesmoTeste = 0;
                    }
                }

            }
            while (pos == -1 || mesmoTeste==-1);

            strcpy(codigoTeste, numeroteste);
            pos_member = localizarMembro(vetorTestes[pos].nUtenteSaude, vetorMembros, nMembros);
            vetorMembros[pos_member].testesRealizados++;
            vetorTestes[pos].tempoColheita = lerTempoColheita();
            resultado = lerInteiro(0, 2, "\nResultado\n[0] Negativo\n[1] Positivo\n[2] Inconclusivo\nopcao: ");
            vetorTestes[pos].resultado = resultado;
            criarLog(vetorTestes[pos], "TESTE REALIZADO");

            switch(resultado)
            {
            case POSITIVO:

                //colocar em isolamento;
                controlo = strcmp ("Isolamento Profilatico",vetorMembros[i].estadoConfinamento);
                if (controlo != 0)
                {
                    strcpy(vetorMembros[pos_member].estadoConfinamento, "Isolamento Profilatico");
                    printf("\nDevido ao teste positivo,o membro da comunidade academica %s com o Numero de Utente de Saude %d foi colocado em isolamento profilatico.\n\n\a", vetorMembros[pos_member].nome, vetorMembros[pos_member].nUtenteSaude);
                    vetorMembros[pos_member].duracaoConfinamento = lerInteiro(1, 30, "Duracao do confinamento");
                    vetorMembros[pos_member].dataConfinamento = vetorTestes[pos].dataRealizacao;
                    vetorMembros[pos_member].dataFimConfinamento = somarData(vetorMembros[pos_member].dataConfinamento, vetorMembros[pos_member].duracaoConfinamento);
                    vetorConfinamentos = adicionarConfinamento(vetorConfinamentos, confinamentos, vetorMembros[pos_member]);
                    if(nMembros!=1)
                    {
                        respostaContactos = lerInteiro(0,1,"\nEsteve em contacto com alguem da comunidade Academica\n[1]SIM - [0]NAO\n");
                        if(respostaContactos == 1)
                        {
                            numeroPessoas = lerInteiro(1,nMembros-1,"\nQuantas pessoas ");
                            for(i=0; i<numeroPessoas; i++)
                            {
                                do
                                {
                                    printf("\n\n########## Contacto Numero %d ##########\n\n", i+1);

                                    num=lerInteiro(111111111,999999999,"\nInsira o numero de utente ");
                                    posContacto=localizarMembro(num, vetorMembros, nMembros);
                                    if(posContacto!=-1)
                                    {
                                        vetorMembros[posContacto].dataConfinamento = vetorTestes[pos].dataRealizacao;
                                        vetorMembros[posContacto].duracaoConfinamento = vetorMembros[pos_member].duracaoConfinamento;
                                        vetorMembros[posContacto].dataFimConfinamento = vetorMembros[pos_member].dataFimConfinamento;
                                        strcpy(vetorMembros[posContacto].estadoConfinamento, "Isolamento Profilatico");
                                        vetorConfinamentos = adicionarConfinamento(vetorConfinamentos, confinamentos, vetorMembros[posContacto]);
                                        printMembro(vetorMembros[posContacto]);

                                    }
                                    else printf("\n\nO Numero nao existe\n\n");
                                }
                                while(pos==-1);


                            }

                        }
                    }

                    printMembro(vetorMembros[pos_member]);
                }
                else printf("\n\nJa se encontra em Isolamento Profilatico\n\n");

                break;


            case INCONCLUSIVO:
                controlo = strcmp ("Quarentena",vetorMembros[i].estadoConfinamento);
                if (controlo != 0)
                {
                    controlo = strcmp ("Isolamento Profilatico",vetorMembros[i].estadoConfinamento);
                    if (controlo == 0)
                    {
                        printf("\nO membro da comunidade academica %s com o Numero de Utente de Saude %d ja se encontra em isolamento profilatico.\n\n\a", vetorMembros[pos_member].nome, vetorMembros[pos_member].nUtenteSaude);
                    }
                    else
                    {
                        strcpy(vetorMembros[pos_member].estadoConfinamento, "Quarentena");
                        printf("\nDevido ao teste inconclusivo,o membro da comunidade academica %s com o Numero de Utente de Saude %d foi colocado em quarentena.\n\n\a", vetorMembros[pos_member].nome, vetorMembros[pos_member].nUtenteSaude);
                        vetorMembros[pos_member].duracaoConfinamento = lerInteiro(1, 30, "Duracao do confinamento");
                        vetorMembros[pos_member].dataConfinamento = vetorTestes[pos].dataRealizacao;
                        vetorMembros[pos_member].dataFimConfinamento = somarData(vetorMembros[pos_member].dataConfinamento, vetorMembros[pos_member].duracaoConfinamento);
                        vetorConfinamentos = adicionarConfinamento(vetorConfinamentos, confinamentos, vetorMembros[pos_member]);
                        if(nMembros!=1)
                        {
                            respostaContactos = lerInteiro(0,1,"\nEsteve em contacto com alguem da comunidade Academica\n[1]SIM - [0]NAO\n");
                            if(respostaContactos == 1)
                            {
                                numeroPessoas = lerInteiro(1,nMembros-1,"\nQuantas pessoas ");
                                for(i=0; i<numeroPessoas; i++)
                                {
                                    do
                                    {
                                        printf("\n\n########## Contacto Numero %d ##########\n\n", i+1);

                                        num=lerInteiro(111111111,999999999,"\nInsira o numero de utente ");
                                        posContacto=localizarMembro(num, vetorMembros, nMembros);
                                        if(posContacto!=-1)
                                        {
                                            vetorMembros[posContacto].dataConfinamento = vetorTestes[pos].dataRealizacao;
                                            vetorMembros[posContacto].duracaoConfinamento = vetorMembros[pos_member].duracaoConfinamento;
                                            vetorMembros[posContacto].dataFimConfinamento = vetorMembros[pos_member].dataFimConfinamento;
                                            strcpy(vetorMembros[posContacto].estadoConfinamento,"Quarentena");
                                            vetorConfinamentos = adicionarConfinamento(vetorConfinamentos, confinamentos, vetorMembros[posContacto]);
                                            printMembro(vetorMembros[posContacto]);

                                        }
                                        else printf("\n\nO Numero nao existe\n\n");
                                    }
                                    while(pos==-1);

                                }

                            }
                        }
                        printMembro(vetorMembros[pos_member]);
                    }
                }
                else printf("\n\nJa se encontra em Quarentena\n\n");

                break;

            case NEGATIVO:
                controlo = strcmp("Quarentena", vetorMembros[pos_member].estadoConfinamento);
                if(controlo==0)
                {
                    strcpy(vetorMembros[pos_member].estadoConfinamento,"Nao Confinado");
                    negativoMudanca = INCONCLUSIVO;

                }
                else
                {
                    controlo = strcmp("Isolamento Profilatico", vetorMembros[pos_member].estadoConfinamento);
                    if(controlo == 0)
                    {
                        strcpy(vetorMembros[pos_member].estadoConfinamento,"Nao Confinado");
                        negativoMudanca = POSITIVO;
                    }
                }

                if(negativoMudanca != NEGATIVO)
                {
                    printf("\n\nEstado de Confinamento alterado para Nao Confinado");
                    printMembro(vetorMembros[pos_member]);

                }


                break;
            }
        }

    }
    return vetorConfinamentos;
}



void printTeste(testeCovid vetorTestes[], int nTestes, membro vetorMembros[], int nMembros)
{
    int pos;
    char nIdentificacao[8];
    int quantPositivos;
    testeCovid teste;

    do
    {
        lerString(nIdentificacao, 8, "\nN. de identificacao do teste [1-100000]: ");
        pos = localizarTeste(vetorTestes, nIdentificacao, nTestes);
        if (pos == -1)
        {
            printf("\nEsse teste nao existe!\n");
        }
    }
    while (pos == -1);
    teste = vetorTestes[pos];

    pos = localizarMembro(teste.nUtenteSaude, vetorMembros, nMembros);
    printf("\n\n############### Teste Covid Numero %s #################\n\n", teste.designacaoTeste);
    printf("\tTipo de Teste: %s\n\tN. de Utente de Saude: %d\n", teste.tipoTeste, teste.nUtenteSaude);
    if (pos != -1)
    {
        printf("\tTipo de membro: %s\n", vetorMembros[pos].tipoMembro);
    }
    printf("\tData de Realizacao: ");
    escreverData(teste.dataRealizacao);
    if (teste.resultado == -1)
    {
        printf("\n\tResultado: Nao Aplicavel\n\tTempo de Colheita: Nao Aplicavel\n");
    }
    else
    {
        if (teste.resultado == 1)
        {
            printf("\n\tResultado: Positivo\n");
        }
        else if(teste.resultado == 2)
        {
            printf("\n\tResultado: Inconclusivo\n");
        }
        else printf("\n\tResultado: Negativo\n");
        printf("\tTempo de Colheita: ");
        mostraTempoColheita(teste);
        printf("\n");
    }
    printf("\tHora da colheita: ");
    escreverHora(teste.horaColheita);
    quantPositivos = quantTestesPositivos(teste.nUtenteSaude, vetorTestes, nTestes);
    printf("\n\tQuantidade de testes positivos : %d\n", quantPositivos);
    printf("\n\n");

}

int quantTestesPositivos (int nUtente, testeCovid vetorTestes[], int nTestes)
{
    int i;
    int quantTestesPos=0;
    for (i=0; i<nTestes; i++)
    {
        if (nUtente == vetorTestes[i].nUtenteSaude && vetorTestes[i].resultado == POSITIVO)
        {
            quantTestesPos++;
        }
    }

    return quantTestesPos;
}

void reagendarTeste(testeCovid *vetorTestes, int nTestes)
{
    int pos;
    int nPCR;
    int control;
    char nIdentificacao[8];
    do
    {
        lerString(nIdentificacao, 8,"\nN. de identificacao do teste [1-100000]: ");
        pos = localizarTeste(vetorTestes, nIdentificacao, nTestes);
        if (pos == -1)
        {
            printf("\nEsse teste nao existe!\n");
        }
    }
    while (pos == -1);

    do
    {
        vetorTestes[pos].dataRealizacao = lerData(2020, 2030, "\nData nova do teste");
        nPCR = contaTestesPCRDia(vetorTestes, nTestes, vetorTestes[pos].dataRealizacao);
        control = strcmp(vetorTestes[pos].tipoTeste, "PCR");
        if (nPCR > MAXDAILYPCR && control == 0)
        {
            printf("ERRO: Impossivel agendar teste PCR para esse dia");
        }
    }
    while(nPCR > MAXDAILYPCR && control == 0);

    vetorTestes[pos].horaColheita = lerHora(0, 23, "\nHora nova do teste");
    criarLog(vetorTestes[pos], "TESTE REAGENDADO");
    printf("\nTESTE REAGENDADO\n");

}

int localizarTeste(testeCovid vetorTestes[], char nIdentificacao[],int nTestes)
{
    int pos = -1;
    int i;
    int controlo;
    for (i = 0; i < nTestes; i++)
    {
        controlo = strcmp(vetorTestes[i].designacaoTeste,nIdentificacao);
        if (controlo == 0)
        {
            pos = i;
            i = nTestes+1;
        }
    }
    return pos;
}

void lerTipoTeste(char string[], int nPCR)
{
    int opcao;
    opcao = lerInteiro(0, 1, "\nTipo de Teste\n[0] PCR\n[1] Antigenio\nopcao");
    switch (opcao)
    {
    case 0:
        if (nPCR > MAXDAILYPCR)
        {
            printf("\nLimite de testes PCR atingidos para esse dia\n");
            lerTipoTeste(string, nPCR);
        }
        else
        {
            strcpy(string, "PCR");
        }
        break;
    case 1:
        strcpy(string, "Antigenio");
        break;
    }
}

testeCovid *agendarTeste(int *nTestes, testeCovid *vetorTestes, int nMembros, membro vetorMembros[])
{
    int position;
    int num;
    int nPCR;
    int option;
    char numeroTeste[8];
    testeCovid *backup;
    backup = vetorTestes;

    vetorTestes = realloc(vetorTestes, ((*nTestes)+1)*sizeof(testeCovid));
    if (vetorTestes == NULL)
    {
        printf("\nErro, nao e possivel agendar mais testes, maximo de testes diarios PCR atingidos (15)\n");
        return backup;
    }
    else
    {
        do
        {
            lerString(numeroTeste,8,"\nIntroduza o numero do teste [1-100000]: ");
            strcpy(vetorTestes[*nTestes].designacaoTeste,numeroTeste);
            position = localizarTeste(vetorTestes, vetorTestes[*nTestes].designacaoTeste, *nTestes);
            if (position != -1)
            {
                printf("Esse teste ja existe!\n");
            }
        }
        while(position != -1);
        do
        {
            vetorTestes[*nTestes].dataRealizacao = lerData(2020, 2030, "\nData do teste");
            nPCR = contaTestesPCRDia(vetorTestes, *nTestes, vetorTestes[*nTestes].dataRealizacao);
            if (nPCR > MAXDAILYPCR)
            {
                option = lerInteiro(0, 1, "\nNesse dia nao sera possivel efetuar um teste PCR.\n\nDeseja prosseguir com a mesma data? [0 - NAO] [1 - SIM]\n");
            }
            else
            {
                option = 1;
            }
        }
        while (option != 1);

        lerTipoTeste(vetorTestes[*nTestes].tipoTeste, nPCR);

        do
        {
            num = lerInteiro(111111111, 999999999, "\nInisira o n. de utente de saude");
            position = localizarMembro(num, vetorMembros, nMembros);
            if (position == -1)
            {
                printf("\nEsse numero de utente nao esta registado.\n");
            }
        }
        while(position == -1);
        vetorTestes[*nTestes].nUtenteSaude = num;
        vetorTestes[*nTestes].horaColheita = lerHora(0, 23, "\nHora do teste");
        vetorTestes[*nTestes].resultado = -1;
        criarLog(vetorTestes[*nTestes], "TESTE AGENDADO");
        printf("\nTESTE AGENDADO COM SUCESSO\n");
        (*nTestes)++;
    }
    return vetorTestes;
}

tipoTempoColheita lerTempoColheita()
{
    int controlo;
    tipoTempoColheita colheita;

    do
    {
        printf("\nTempo de Colheita[minutos]:[segundos]: ");
        controlo = scanf("%d:%d", &colheita.minutos, &colheita.segundos);
        limpaBuffer();

        if(controlo != 2 || colheita.minutos < 0 || colheita.minutos > 59 || colheita.segundos < 0 || colheita.segundos > 59 )
        {
            printf("\n\nERRO - Formato introduzido incorreto\n\n"); // novo
        }

    }
    while (controlo != 2 || colheita.minutos < 0 || colheita.minutos > 59 || colheita.segundos < 0 || colheita.segundos > 59 );
    return colheita;
}
