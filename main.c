#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mysql.h>
#include "lista.h"

typedef struct {
    int id;
    int RA;
    char nome[255];
    char endereco[255];
} Aluno;

Lista* lista;
MYSQL *connection;

/* INSTRUÇÕES DO MYSQL ------------------------------------*/
void connect_db() {
    connection = mysql_init(NULL);
    MYSQL *test_connect = mysql_real_connect(connection, 
                                            "localhost", 
                                            "root", 
                                            "root",
                                            "escola_db",
                                            3306,
                                            NULL,
                                            0);

    if (test_connect == NULL) {
        printf("Erro de Conexão:\n %s\n", mysql_error(connection));
        system("pause");
        exit(1);
    }
}

void desconect_db() {
    int ping = mysql_ping(connection);

    if (ping == 0) {
        mysql_close(connection);
    }
}

void sql_insert(Aluno *aluno) {
    unsigned int id;
    char insertStatement[2000] = "\0";
    char insertSQL[2000] = "\0";

    strcat(insertStatement, "INSERT INTO alunos(ra, ");
    strcat(insertStatement, "nome, endereco) VALUES ");
    strcat(insertStatement, "(%d, '%s', '%s')       ");

    sprintf(insertSQL, insertStatement, aluno->RA, 
                                        aluno->nome, 
                                        aluno->endereco);

    mysql_query(connection, insertSQL);
    id = mysql_insert_id(connection);

    aluno->id = id;
}

void sql_delete(int id) {
    char insertStatement[2000] = "\0";
    char insertSQL[2000] = "\0";

    strcat(insertStatement, "DELETE FROM alunos ");
    strcat(insertStatement, "WHERE id = %d      ");
    sprintf(insertSQL, insertStatement, id);
    
    mysql_query(connection, insertSQL);
}

void sql_select() {
    MYSQL_RES *query_result;
    MYSQL_ROW query_row;
    Aluno *aluno;
    char selectSQL[1000] = "\0";

    strcat(selectSQL, "SELECT id, ra, nome, endereco ");
    strcat(selectSQL, "  FROM alunos                 ");
    strcat(selectSQL, " ORDER BY id                  ");
    
    mysql_query(connection, selectSQL);
    query_result = mysql_use_result(connection);
    lista = NULL;

    while ((query_row = mysql_fetch_row(query_result)) != NULL) {
        aluno = (Aluno*) malloc(sizeof(Aluno));
        aluno->id = atoi(query_row[0]);
        aluno->RA = atoi(query_row[1]);
        strcpy(aluno->nome, query_row[2]);
        strcpy(aluno->endereco, query_row[3]);
        lista = add(lista, aluno);
    }
}

/* INSTRUÇÕES DO PROGRAMA ---------------------------------*/
int retornarIndex(int id) {
    int index = 0;
    Aluno *aluno;
    Lista *aux = lista;

    while (aux != NULL) {
        aluno = (Aluno*) aux->item;

        if (aluno->id == id) {
            return index;
        }

        aux = aux->proximo;
        index++;
    };

    return -1;
}

void incluirAluno() {
    Aluno *aluno = malloc(sizeof(Aluno));
    system("cls");
    printf("Informe os dados do aluno\n");
    printf("RA:");
    scanf("%d", &aluno->RA);
    printf("Nome: ");
    
    fflush(stdin);
    scanf("%[^\n]", aluno->nome);
    
    fflush(stdin);
    printf("Endereco:");
    scanf("%[^\n]", aluno->endereco);

    sql_insert(aluno);
}

void listarAlunos() {
    Lista *aux;
    Aluno *aluno;
    int tamanho;

    sql_select();
    tamanho = count(lista);

    system("cls");
    printf("Alunos\n");

    if (tamanho > 0) {
        printf("ID | RA | Nome | Endereco\n");
        
        for (aux = lista; aux != NULL;aux = aux->proximo){
            aluno = (Aluno*) aux->item;
            printf("%d | %d | %s |%s\n", aluno->id,
                                         aluno->RA, 
                                         aluno->nome, 
                                         aluno->endereco);
        }
    }    

    printf("Total de alunos: %d\n", tamanho);
}

void excluirAluno() {
    int id, index;
    int tamanho = count(lista);

    system("cls");
    listarAlunos();
    printf("Exclusao de Aluno\n");
    printf("Informe um ID: ");
    scanf("%d", &id);

    index = retornarIndex(id);

    if (index == -1) {
        printf("ID Invalido!");
        return;
    }

    sql_delete(id);
    lista = delete(lista, index);
    printf("Item excluido!\n");
}

/* PROGRAMA PRINCIPAL -------------------------------------*/
int main(int argc, char const *argv[]){
    int opcao = 0;
    connect_db();

    do {
        system("cls");
        printf("!!!Sistema de Cadastro de Alunos!!!\n");
        printf("1 - Incluir Aluno\n");
        printf("2 - Listar Aluno\n");
        printf("3 - Excluir Aluno\n");
        printf("4 - Sair\n");
        printf("Digite a opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            incluirAluno();
            break;
        case 2:
            listarAlunos();
            break;
        case 3:
            excluirAluno();
            break;
        case 4:
            printf("Muito Obrigado!");
            break;
        default:
            printf("Opcao Invalida!");
            break;
        }
        system("pause");
    } while(opcao != 4);

    desconect_db();
    return 0;
}