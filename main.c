#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comunidadeAcademica.h"
#include "testesDiagnostico.h"

int main()
{
    membro vetorMembros[MAXMEMBERS];
    testeCovid *vetorTestes=NULL;
    tipoConfinamento *vetorConfinamentos=NULL;
    int confinamentos;
    int nMembros;
    int testes;
    int testesAgendados;
    int testesRealizados;
    int membrosVacinados;
    int opcao,opcao2;
    int membroSelecionado;
    int i;
    int variosMembros;
    int numQuarentena, numIsolamento;
    int pos, controlo;
    char codigoTeste[8];
    nMembros = 0;
    membrosVacinados = 0;
    testes = 0;
    testesAgendados = 0;
    testesRealizados = 0;

    lerMembros(vetorMembros, &nMembros);
    vetorTestes = lerTestes(&testes);
    vetorConfinamentos = lerConfinamentos(&confinamentos);

    do
    {
        testesAgendados = contaTestesAgendados(vetorTestes, testes);
        testesRealizados = contaTestesRealizados(vetorTestes, testes);
        membrosVacinados = contaMembrosVacinados(nMembros, vetorMembros);
        opcao = menu(nMembros, testesAgendados, testesRealizados, membrosVacinados);
        switch(opcao)
        {
        case 0:
            if (nMembros < MAXMEMBERS)
            {
                vetorConfinamentos = novoMembro(vetorMembros, &nMembros, vetorConfinamentos, &confinamentos);
            }
            else printf("Limite de utilizadores atingido.\n");
            break;
        case 1:
            variosMembros = lerInteiro(1, MAXMEMBERS-nMembros, "\nQuantidade de membros a adicionar");
            if (nMembros+variosMembros < MAXMEMBERS)
            {
                for (i = 0; i < variosMembros; i++)
                {
                    printf("\n********************** Membro %d **********************\n", i+1);
                    vetorConfinamentos = novoMembro(vetorMembros, &nMembros, vetorConfinamentos, &confinamentos);
                }
            }

            break;
        case 2:

            if (nMembros > 0)
            {
                do
                {
                    membroSelecionado = lerInteiro(111111111,999999999, "\nNumero de utente de saude a selecionar ");
                    membroSelecionado = localizarMembro(membroSelecionado, vetorMembros, nMembros);
                    if(membroSelecionado == -1)
                    {
                        printf("ERRO: Esse numero nao existe!\n");
                    }
                }
                while (membroSelecionado == -1);

                vetorConfinamentos = editarMembro(membroSelecionado, vetorMembros, nMembros, vetorConfinamentos, &confinamentos);

            }
            else printf("\n\nERRO: Nao existem membros disponiveis!\n");
            break;
        case 3: // TESTES

            do
            {
                opcao2=subMenu();
                testesAgendados = contaTestesAgendados(vetorTestes, testes);
                testesRealizados = contaTestesRealizados(vetorTestes, testes);
                switch(opcao2)
                {
                case 0:
                    if(nMembros!=0)
                    {
                        vetorTestes=agendarTeste(&testes, vetorTestes, nMembros, vetorMembros);
                    }else printf("\n\nNecessita de inserir membros - 0 Membros disponiveis!\n\n");
                    break;

                case 1:
                    if (testesAgendados > 0)
                    {
                        reagendarTeste(vetorTestes, testes);
                    }
                    else printf("\n\nAgende um teste primeiro por favor!\n\n");

                    break;

                case 2:
                    if(testes!=0) // novo
                    {
                        vetorConfinamentos = introduzirResultadoTeste(vetorTestes, &testes, vetorMembros, nMembros, vetorConfinamentos, &confinamentos, testesAgendados, codigoTeste);

                        pos = localizarTeste(vetorTestes, codigoTeste, testes);
                        controlo = strcmp(vetorTestes[pos].tipoTeste, "PCR");
                        if(controlo != 0 && vetorTestes[pos].resultado == POSITIVO)
                        {
                            printf("\n\nComo o teste realizado e do tipo Antigenio e deu positivo, e necessario agendar um teste PCR\n");
                            printf("\n\n######## AGENDAMENTO TESTE PCR ########\n\n");
                            vetorTestes = agendarTestePCR (&testes, vetorTestes, vetorTestes[pos].nUtenteSaude);
                        }
                        else if(vetorTestes[pos].resultado == INCONCLUSIVO)
                        {
                            printf("\n\nComo o teste realizado deu Inconclusivo, e necessario agendar um teste PCR\n");
                            printf("\n\n######## AGENDAMENTO TESTE PCR ########\n\n");
                            vetorTestes = agendarTestePCR (&testes, vetorTestes, vetorTestes[pos].nUtenteSaude);
                        }

                    }
                    else printf("\n\nATENCAO - Nenhum teste agendado\n\n\n");
                    break;

                case 3:
                    if (testes > 0)
                    {
                        printTeste(vetorTestes, testes, vetorMembros, nMembros);
                    }
                    else printf("\n\nNenhum Teste agendado ou realizado\n");

                    break;

                case 4:
                    break;

                default:
                    printf("\n\nERRO: opcao desconhecida\n");
                }
            }
            while(opcao2!=4);

            break;
        case 4:
            dadosEstatisticos (vetorTestes, vetorMembros, testesRealizados, nMembros);
            if(nMembros != 0)
            {
                quantidadeTipoMembro(vetorMembros, nMembros);

            }
            else printf("\n\nImpossivel mostrar a quantidade dos tipos de Membros - 0 Membros");
            break;
        case 5:
            //Lista de confinamento
            if (nMembros==0)
            {
                printf("\nERRO - Nao ha membros disponiveis\n");
            }
            else
            {
                numQuarentena = tipodeConfinamentoQuarentena (vetorConfinamentos, confinamentos, vetorMembros, nMembros);
                numIsolamento = tipodeConfinamentoIso (vetorConfinamentos, confinamentos, vetorMembros,nMembros, 1);
                if (numIsolamento==0 && numQuarentena==0)
                {
                    printf("\n\nNao ha membros em confinamento\n\n\n");
                }
            }

            break;
        case 6:
            break;
        default:
            printf("Erro, opcao nao existe!");
        }
    }
    while (opcao != 6);

    opcao = lerInteiro(0, 1, "Deseja guardar?(0 - nao, 1- sim)\n");
    if(opcao==1)
    {
        gravarMembros(vetorMembros, nMembros);
        gravarTestes(vetorTestes, testes);
        gravarConfinamentos(vetorConfinamentos, &confinamentos);
        printf("\n\n********************************* Informacao guardada com sucesso *********************************\n\n");
    }
    free(vetorTestes);
    free(vetorConfinamentos);
    printf("\nAdeus ate a proxima!\n");


    return 0;
}
