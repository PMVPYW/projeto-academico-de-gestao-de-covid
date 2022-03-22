#include <stdio.h>
#include <string.h>
#include "comunidadeAcademica.h"
#include "testesDiagnostico.h"

/*
int contaEstudante(membro vetorMembros, int nMembros)
{
    int i;
    for (i = 0; i < nMembros; i++)
}
*/

void printConfinamento (membro membroTarget, tipoConfinamento confinamentoTarget)
{
    int confinado, quarentena;
    printf("\n\n\n");
    printf("N. Utente de Saude: %d\nNome: %s\nTipo de Membro: %s\nAno de nascimento: %d\nEstado de confinamento: %s\nEstado de Vacinacao: %d doses\n", confinamentoTarget.nUtenteSaude, membroTarget.nome, membroTarget.tipoMembro, membroTarget.anoNascimento, confinamentoTarget.estadoConfinamento, membroTarget.estadoVacinacao);
    confinado = strcmp(confinamentoTarget.estadoConfinamento, "Isolamento Profilatico");
    quarentena = strcmp(confinamentoTarget.estadoConfinamento, "Quarentena");
    if (confinado == 0 || quarentena == 0)
    {
        printf("Duracao o confinamento: %d dias\n", confinamentoTarget.duracao);
        printf("Data de inicio de confinamento: ");
        escreverData(confinamentoTarget.dataConfinamento);
        printf("\nData de fim de confinamento: ");
        escreverData(confinamentoTarget.dataFimConfinamento);
        printf("\n");
    }

    if (membroTarget.estadoVacinacao != 0)
    {
        printf("Data da Ultima Vacina: ");
        escreverData(membroTarget.ultimaVacina);
        printf("\n");
    }
    else printf("Data da Ultima Vacina: Nao Aplicavel.\n");
    printf("Quantidade de testes realizados: %02d\n", membroTarget.testesRealizados);
    printf("\n\n");

}




void quantidadeTipoMembro (membro vetorMembros[], int nMembros)
{
    int i;
    int controlo1, controlo2, controlo3;
    int estudante = 0;
    int docente = 0;
    int tecnico = 0;

    for (i=0; i<nMembros; i++)
    {
        controlo1 = strcmp ("Estudante", vetorMembros[i].tipoMembro);
        controlo2 = strcmp ("Docente", vetorMembros[i].tipoMembro);
        controlo3 = strcmp ("Tecnico", vetorMembros[i].tipoMembro);

        if(controlo1 == 0)
        {
            estudante++;
        }

        if(controlo2 == 0)
        {
            docente++;
        }

        if(controlo3 == 0)
        {
            tecnico++;
        }
    }

    printf("\nTipo de Membros :\n\n\tEstudantes : %d\n\tDocentes : %d\n\tTecnicos : %d\n\n", estudante, docente, tecnico);
}

int tipodeConfinamentoQuarentena (tipoConfinamento *vetorConfinamentos, int confinamentos, membro vetorMembros[], int nMembros)
{
    int controlo,i;
    int quarentena = 0;
    int printed = 0;
    int pos;
    for (i = 0; i <= confinamentos; i++)
    {
        controlo = strcmp("Quarentena", vetorConfinamentos[i].estadoConfinamento);
        if (controlo == 0)
        {
            if (printed == 0)
            {
                printf("\n**********************QUARENTENA**********************\n");
                printed++;
            }

            quarentena++;
            pos = localizarMembro(vetorConfinamentos[i].nUtenteSaude, vetorMembros, nMembros);
            printConfinamento(vetorMembros[pos], vetorConfinamentos[i]);
        }
    }
    return quarentena;
}

int tipodeConfinamentoIso (tipoConfinamento vetorConfinamentos[], int confinamentos,membro vetorMembros[], int nMembros, int print)
{
    int controlo,i;
    int isolamento = 0;
    int printed = 0;
    int pos;

    for (i = 0; i <= confinamentos; i++)
    {
        controlo = strcmp ("Isolamento Profilatico", vetorConfinamentos[i].estadoConfinamento);
        if (controlo == 0)
        {
            if (printed == 0)
            {
                printf("****************ISOLAMENTO PROFILATICO****************\n");
                printed++;
            }
            isolamento++;
            if (print != 0)
            {
                pos = localizarMembro(vetorConfinamentos[i].nUtenteSaude, vetorMembros, nMembros);
                printConfinamento(vetorMembros[pos], vetorConfinamentos[i]);
            }

        }
    }
    return isolamento;
}

void criarLogMembros(membro member, char msg[])
{
    FILE *ficheiro;
    int isolamento, quarentena;
    ficheiro = fopen("log.txt", "a");
    if (ficheiro == NULL)
    {
        printf("\nERRO ao gerar log!\n");
    }
    else
    {
        fprintf(ficheiro, "============MEMBRO %s============\n", msg);
        fprintf(ficheiro, "Numero de utente de saude: %d\nNome: %s\nTipo de membro: %s\nAno de nascimento: %d\n", member.nUtenteSaude,member.nome,member.tipoMembro, member.anoNascimento);
        fprintf(ficheiro, "Doses da vacina: %d\n", member.estadoVacinacao);
        switch(member.estadoVacinacao)
        {
        case 0:
            fprintf(ficheiro, "Data da ultima vacina: Nao aplicavel\n");
            break;
        case 1:
        case 2:
        case 3:
            fprintf(ficheiro, "Data da ultima vacina: %02d-%02d-%d\n", member.ultimaVacina.dia, member.ultimaVacina.mes, member.ultimaVacina.ano);
            break;
        }
        fprintf(ficheiro, "Estado de confinamento: %s", member.estadoConfinamento);
        isolamento = strcmp(member.estadoConfinamento, "Isolamento Profilatico");
        quarentena = strcmp(member.estadoConfinamento, "Quarentena");
        if (isolamento == 0 || quarentena == 0)
        {
            fprintf(ficheiro, "Data de inicio do confinamento: %02d-%02d-%d\nDuracao do confinamento: %02d dias\n", member.dataConfinamento.dia, member.dataConfinamento.mes, member.dataConfinamento.ano, member.duracaoConfinamento);
            fprintf(ficheiro, "Data de fim do confinamento: %02d-%02d-%d\nDuracao\n\n\n", member.dataFimConfinamento.dia, member.dataFimConfinamento.mes, member.dataFimConfinamento.ano);
        }
        fclose(ficheiro);
    }
}

void gravarMembros(membro vetor[], int nMembros)
{
    int controlo;
    FILE *ficheiro;
    ficheiro = fopen("comunidadeAcademica.bin", "wb");

    if (ficheiro == NULL)
    {
        printf("\nERRO: ficheiro nao criado!\n");
    }
    else
    {
        controlo = fwrite(&nMembros, sizeof(int), 1, ficheiro);
        if (controlo != 1)
        {
            printf("\nErro: erro de escrita 1\n");
        }
        else
        {
            controlo = fwrite(vetor, sizeof(membro), nMembros, ficheiro);
            if (controlo != nMembros)
            {
                printf("\nErro: erro de escrita 2\n");
            }
        }
        fclose(ficheiro);
    }
}

void lerMembros(membro vetor[], int *nMembro)
{
    FILE *ficheiro;
    int controlo;
    ficheiro = fopen("comunidadeAcademica.bin", "rb");

    if (ficheiro == NULL)
    {
        printf("\nERRO: nao foi possivel ler o ficheiro!\n");
    }
    else
    {
        controlo = fread(nMembro, sizeof(int), 1, ficheiro);
        if (controlo != 1)
        {
            printf("\nERRO: erro de leitura 1\n");
        }
        else
        {
            controlo = fread(vetor, sizeof(membro), *nMembro, ficheiro);
            if (controlo != *nMembro)
            {
                printf("\nERRO: erro de leitura 2\n");
            }
        }
        fclose(ficheiro);
    }


}

int contaMembrosVacinados(int nMembros, membro vetorMembros[])
{
    int i;
    int contador;
    contador = 0;
    for (i = 0; i < nMembros; i++)
    {
        if (vetorMembros[i].estadoVacinacao != 0)
        {
            contador++;
        }
    }
    return contador;
}


tipoConfinamento *novoMembro(membro vetor[], int *nMembers, tipoConfinamento vetorConfinamento[], int *confinamentos)
{
    membro novo;
    int numero;
    int membroUnico;
    do
    {
        numero = lernUtentesaude();
        membroUnico = localizarMembro(numero, vetor, *nMembers);
        if (membroUnico != -1)
        {
            printf("\nEsse numero ja existe!\n");
        }
    }
    while(membroUnico != -1);
    novo.nUtenteSaude = numero;


    lerString(novo.nome, MAXSTRING, "\nNome: ");
    lerTipomembro(novo.tipoMembro);
    novo.anoNascimento = lerAnoNascimento();
    novo.estadoVacinacao = lerEstadoVacinacao();
    novo.ultimaVacina = lerDataUltimaVacina(novo);
    novo.testesRealizados = 0;
    vetorConfinamento = lerEstadoConfinamento(&novo, vetorConfinamento, confinamentos);
    vetor[*nMembers] = novo;
    (*nMembers)++;
    criarLogMembros(novo, "ADICIONADO");

    return vetorConfinamento;
}


int localizarMembro(int nUtenteSaude, membro vetorMembros[], int nMembros)
{
    int i;
    int target = -1;
    for(i=0; i <= nMembros; i++)//<= pq nmembros = quantidade membros a partir do zero
    {
        if (nUtenteSaude == vetorMembros[i].nUtenteSaude)
        {
            target = i;
            i = nMembros;
        }
    }
    return target;
}
tipoConfinamento *editarMembro(int position, membro vetor[], int nMembers, tipoConfinamento *vetorConfinamento, int *confinamentos)
{
    int numero;
    int membroUnico;
    int opcao;
    int alteracao=0;
    printMembro(vetor[position]);
    do
    {
        opcao = lerInteiro(0, 7, "\n\n[0] N. Utente de saude\n[1] Nome\n[2] Tipo de membro\n[3] Ano de nascimento\n[4] Estado confinamento\n[5] Estado vacinacao\n[6] Data da ultima vacina\n[7] Voltar\n");
        switch (opcao)
        {
        case 0:
            do
            {
                numero = lernUtentesaude();
                membroUnico = localizarMembro(numero, vetor, nMembers);
                if (membroUnico != -1)
                {
                    printf("Esse numero ja existe!\n");
                }
            }
            while(membroUnico != -1);
            printf("\n********** Numero de Utente alterado com sucesso **********");
            alteracao = -1;
            vetor[position].nUtenteSaude = numero;
            printMembro(vetor[position]);
            break;
        case 1:
            lerString(vetor[position].nome, MAXSTRING, "\nNome: ");
            alteracao = -1;
            printf("\n********** Nome alterado com sucesso **********");
            printMembro(vetor[position]);
            break;
        case 2:
            lerTipomembro(vetor[position].tipoMembro);
            alteracao = -1;
            printf("\n********** Tipo de Membro alterado com sucesso **********");
            printMembro(vetor[position]);
            break;
        case 3:
            vetor[position].anoNascimento = lerAnoNascimento();
            alteracao = -1;
            printf("\n********** Ano de Nascimento alterado com sucesso **********");
            printMembro(vetor[position]);
            break;
        case 4:
            vetorConfinamento = lerEstadoConfinamento(&vetor[position], vetorConfinamento, confinamentos);
            alteracao = -1;
            printf("\n********** Estado de Confinamento alterado com sucesso **********");
            printMembro(vetor[position]);
            break;
        case 5:
            vetor[position].estadoVacinacao = lerEstadoVacinacao();
            vetor[position].ultimaVacina = lerDataUltimaVacina(vetor[position]);
            alteracao = -1;
            printf("\n********** Data de Vacinacao alterada com sucesso **********");
            printMembro(vetor[position]);
            break;
        case 6:
            vetor[position].ultimaVacina = lerDataUltimaVacina(vetor[position]);
            alteracao = -1;
            printf("\n********** Data da ultima vacina alterada com sucesso **********");
            printMembro(vetor[position]);
            break;
        case 7:
            break;
        default:
            printf("\nERRO: Opcao %d nao reconhecido\n", opcao);
        }
    }
    while(opcao != 7);
    criarLogMembros(vetor[position], "EDITADO");
    if(alteracao==-1)
    {
        printf("\n********** PARAMETROS ALTERADOS COM SUCESSO **********\n");
        printMembro(vetor[position]);
    }
    else
    {
        printf("\n********** SEM ALTERACOES **********");
        printMembro(vetor[position]);
    }
    return vetorConfinamento;
}


int lernUtentesaude()
{
    int numero;
    numero = lerInteiro(111111111, 999999999, "\nInisira o n. de utente de saude ");
    return numero;
}

void printMembro(membro target)
{
    int confinado, quarentena;
    printf("\n\n\n");
    printf("N. Utente de Saude: %d\nNome: %s\nTipo de Membro: %s\nAno de nascimento: %d\nEstado de confinamento: %s\nEstado de Vacinacao: %d doses\n", target.nUtenteSaude, target.nome, target.tipoMembro, target.anoNascimento, target.estadoConfinamento, target.estadoVacinacao);
    confinado = strcmp(target.estadoConfinamento, "Isolamento Profilatico");
    quarentena = strcmp(target.estadoConfinamento, "Quarentena");
    if (confinado == 0 || quarentena == 0)
    {
        printf("Duracao o confinamento: %d dias\n", target.duracaoConfinamento);
        printf("Data de inicio de confinamento: ");
        escreverData(target.dataConfinamento);
        printf("\nData de fim de confinamento: ");
        escreverData(target.dataFimConfinamento);
        printf("\n");
    }

    if (target.estadoVacinacao != 0)
    {
        printf("Data da Ultima Vacina: ");
        escreverData(target.ultimaVacina);
        printf("\n");
    }
    else printf("Data da Ultima Vacina: Nao Aplicavel.\n");
    printf("Quantidade de testes realizados: %02d\n", target.testesRealizados);
    printf("\n\n\n");
}

void lerTipomembro(char tipo[])
{
    int opcao;
    opcao = lerInteiro(1,3, "\nTipo de membro:\n[1] Estudante\n[2] Docente\n[3] Tecnico\n");
    switch (opcao)
    {
    case 1:
        strcpy(tipo, "Estudante");
        break;
    case 2:
        strcpy(tipo, "Docente");
        break;
    case 3:
        strcpy(tipo, "Tecnico");
    }
}

int lerAnoNascimento()
{
    return lerInteiro(1900, 2020, "\nAno de Nascimento ");
}

tipoConfinamento *lerEstadoConfinamento(membro *membroSelecionado, tipoConfinamento *vetorConfinamento, int *confinamentos)
{
    int opcao;
    opcao = lerInteiro(1,3, "\nEstado de confinamento:\n[1] Nao Confinado\n[2] Quarentena\n[3] Isolamento Profilatico\n");
    switch (opcao)
    {
    case 1:
        strcpy(membroSelecionado->estadoConfinamento, "Nao Confinado");
        break;
    case 2:
        strcpy(membroSelecionado->estadoConfinamento, "Quarentena");
        membroSelecionado->dataConfinamento = lerData(2020, 2030, "\nInsira o data do inicio do confinamento");
        membroSelecionado->duracaoConfinamento = lerInteiro(0, 30, "\nDuracao do confinamento");
        membroSelecionado->dataFimConfinamento = somarData(membroSelecionado->dataConfinamento, membroSelecionado->duracaoConfinamento);
        vetorConfinamento=adicionarConfinamento(vetorConfinamento, confinamentos, *membroSelecionado);
        break;
    case 3:
        strcpy(membroSelecionado->estadoConfinamento, "Isolamento Profilatico");
        membroSelecionado->dataConfinamento = lerData(2020, 2030, "\nInsira o data do inicio do confinamento");
        membroSelecionado->duracaoConfinamento = lerInteiro(0, 30, "\nDuracao do confinamento");
        membroSelecionado->dataFimConfinamento = somarData(membroSelecionado->dataConfinamento, membroSelecionado->duracaoConfinamento);
        vetorConfinamento=adicionarConfinamento(vetorConfinamento, confinamentos, *membroSelecionado);
    }
    return vetorConfinamento;
}

int lerEstadoVacinacao()
{
    return lerInteiro(0,3, "\nEstado de vacinacao:\n[0] Sem Vacina\n[1] Dose 1\n[2] Dose 2\n[3] Dose 3\n");
}

tipoData lerDataUltimaVacina(membro member)
{
    tipoData data;
    if (member.estadoVacinacao != 0)
    {
        data =  lerData(2020, 2022, "\nData da ultima vacina ");
        return data;
    }
    data.ano = 0;
    data.dia = 0;
    data.mes = 0;
    printf("\nData da Ultima Vacina: Nao Aplicavel\n\nPrecisa de ser vacinado primeiro!\n");
    return data;
}
