%{
/*---------------------------INCLUDES---------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
#include "funciones.h"
//#include "PILAdinamica.h"

/*-------------------DECLARACION DE VARIABLES-------------------*/
/*
int progPtr = 0; //Variables para acciones semanticas.
int sentPtr = 0;
int writePtr = 0;
int readPtr = 0;
int asigPtr = 0;
int sPtr = 0;
int sumaPtr = 0;
int listaPtr = 0;

int tipoAux = -1; //Variables para manejar tipos de datos.
int tipoFactor = -1;
int tipoTermino = -1;
int tipoExpresion = -1;
int tipoConstante = -1;
int tipoID=-1;

int cantVariables = 0; //Variables para controlar declaraciones.
int cantTipos = 0;

char tipoComparador[4]; //Variable auxiliar para comparadores.
*/

/*-------------------VARIABLES DE BISON-------------------*/
int yystopparser=0;
FILE  *yyin;
int yyerror();
int yylex();

/*-------------------DECLARACION DE FUNCIONES-------------------*/

int crear_nodo(char*, int, int);
int crear_hoja(char*);
/*
char* ConvertirAString(int);
void procesarIfAnd(int);
void procesarIfOr(int,int);
char* obtenerNombreVariable();

//Tipos {0 = "",1 = "ENTERO",2 = "CTE ENTERA",3 = "REAL",4 = "CTE REAL",5 = "STRING",6 = "CTE STRING",7 = "ID",8 = "CONST",8 = "CONST ENTERA", 9 = "CONST REAL", 10 = "CONST STRING"}​​​​​;

t_pila pilaIF;
t_pila pilaWHILE;
t_pila pilaMAXIMO;
t_pila pilaTIPO;
*/
%}

%union {
    int int_val;
    char *str_val;
}

%type <str_val> ID CTE_STRING
%type <int_val> CTE


%token WRITE
%token READ
%token SUMA_IMPAR

%token ASIGNA              
%token C_A
%token C_C
%token PAR_A
%token PAR_C
%token PYC
%token COMA

%token ID                 
%token CTE_STRING
%token CTE

%%
s:
    prog {
        printf("\n Regla 0: Start -> Prog");
        printf("\n\nFIN PROGRAMA\n\n");
        crearArchivoTS();
        /* 
        crearArbol();
        generarAsm();
        */
    } ;

prog: 
    sent {printf("\n Regla 1: Prog -> Sent"); }; | 
    prog sent {printf("\n Regla 2: Prog -> Prog Sent");};

sent:
    read {printf("\n Regla 3.1: Sent -> Read ");} | 
    write {printf("\n Regla 3.2: Sent -> Write ");}; | 
    asig {printf("\n Regla 3.3: Sent -> Asig");};

read:
    READ ID {printf("\n Regla 4: Read -> READ ID"); cargarEnTS( $2, 5 );};

asig:
    ID {cargarEnTS( $1, 5 );} ASIGNA suma_impar {printf("\n Regla 5: Asig -> ID ASIGNA suma_impar");};

suma_impar:
    SUMA_IMPAR PAR_A ID PYC C_A lista C_C PAR_C {printf("\n Regla 6: suma_impar -> SUMA_IMPAR PAR_A ID PYC C_A lista C_C PAR_C");} |
    SUMA_IMPAR PAR_A ID PYC C_A C_C PAR_C {printf("\n Regla 7: suma_impar ->  SUMA_IMPAR PAR_A ID PYC C_A C_C PAR_C");} ;

lista: 
    CTE {
            printf("\n Regla 8: lista -> CTE"); 
            char valorString[100];
            sprintf(valorString, "%d", $1);
            cargarEnTS(valorString, 2);
        } |
    lista COMA CTE {
            printf("\n Regla 9: lista -> lista COMA CTE"); 
            //cargarEnTS( $3, 2 );
            char valorString[100];
            sprintf(valorString, "%d", $3);
            cargarEnTS(valorString, 2);
        };

write:
    WRITE CTE_STRING {printf("\n Regla %s: write -> WRITE CTE_STRING", $2); cargarEnTS( $2, 4 );} |
    WRITE ID {printf("\n Regla 11: write -> WRITE ID"); cargarEnTS( $2, 5 );};

%%


int main(int argc, char *argv[])
{
    if((yyin = fopen(argv[1], "rt"))==NULL)
    {
        printf("\nNo se puede abrir el archivo de prueba: %s\n", argv[1]);
    }
    else
    { 
        /*
        crear_pila(&pilaIF);
        crear_pila(&pilaWHILE);
        crear_pila(&pilaTIPO);
        crear_pila(&pilaMAXIMO);
        */
        yyparse();
    }
	fclose(yyin);

    return 0;
}

int yyerror(void)
{
    printf("\nError Sintactico\n");
	exit(1);
}

int crear_nodo(char* op, int nodo1, int nodo2)
{
    return 1;
}

int crear_hoja(char* valor)
{
    return 1;
}