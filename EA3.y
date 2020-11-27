%{
/*---------------------------INCLUDES---------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
#include "funciones.h"

/*-------------------DECLARACION DE VARIABLES-------------------*/
int cantImpares = 0;
char ultimoId [30];
int flag = 0;

t_nodo* sPtr = 0; //Variables para acciones semanticas -- Arbol sintactico.
t_nodo* progPtr = 0; 
t_nodo* sentPtr = 0;
t_nodo* writePtr = 0;
t_nodo* readPtr = 0;
t_nodo* asigPtr = 0;
t_nodo* sumaPtr = 0;
t_nodo* listaPtr = 0;
/*
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
int esImpar(int);

//Tipos {0 = "",1 = "ENTERO",2 = "CTE ENTERA",3 = "STRING",4 = "CTE STRING",5 = "ID"}​​​​​;
%}

%union {
    int int_val;
    char *str_val;
}

%type <str_val> ID CTE_STRING
%type <int_val> CTE

%token WRITE READ SUMA_IMPAR
%token ASIGNA C_A C_C PAR_A PAR_C PYC COMA
%token ID CTE_STRING CTE

%%
s:
    prog {
        printf("\n Regla 0: Start -> Prog");
        printf("\n\nFIN PROGRAMA --> Cant Impares: %d\n\n", cantImpares);
        crearArchivoTS();
        sPtr=progPtr;
         
        
		grabarArbol(sPtr);
		generarArchivoGraphViz(sPtr);
		/*generarAssembler(sPtr);
        */
    } ;

prog: 
    sent {
        printf("\n Regla 1: Prog -> Sent"); 
        progPtr = sentPtr;
    }; | 
    prog sent {
        printf("\n Regla 2: Prog -> Prog Sent");
        {
            t_info info;
            strcpy(info.valor,"sent"); 
            progPtr = crearNodo(&info,progPtr,sentPtr);
        }
    };

sent:
    read {
        printf("\n Regla 3.1: Sent -> Read ");
        sentPtr = readPtr;
    } | 
    write {
        printf("\n Regla 3.2: Sent -> Write ");
        sentPtr = writePtr;
    }; | 
    asig {
        printf("\n Regla 3.3: Sent -> Asig");
        sentPtr = asigPtr;
    };

read:
    READ ID {
        readPtr = crearHojaT("READ");
        insertarHijo(&readPtr->izq,crearHojaT("stdin"));
        insertarHijo(&readPtr->der,crearHojaT($2));
		
        printf("\n Regla 4: Read -> READ ID"); 
        cargarEnTS( $2, 5 );
    };

asig:
    ID {
        cargarEnTS( $1, 5 );
        strcpy(ultimoId,$1);
    } ASIGNA suma_impar {
        t_info info;
        strcpy(info.valor,"=");
        t_info info_id;
        strcpy(info_id.valor, ultimoId);
        asigPtr = crearNodo(&info, crearHoja(&info_id), sumaPtr );
        printf("\n Regla 5: Asig -> ID ASIGNA suma_impar");
    };

suma_impar:
    SUMA_IMPAR PAR_A ID PYC C_A lista C_C PAR_C {
        printf("\n Regla 6: suma_impar -> SUMA_IMPAR PAR_A ID PYC C_A lista C_C PAR_C");
        sumaPtr = listaPtr;
    } |
    SUMA_IMPAR PAR_A ID PYC C_A C_C PAR_C {
        printf("\n Regla 7: suma_impar ->  SUMA_IMPAR PAR_A ID PYC C_A C_C PAR_C");
        //Cargar error lista vacia.
    } ;

lista: 
    CTE {
        printf("\n Regla 8: lista -> CTE %d", $1); 
        char valorString[100];
        sprintf(valorString, "%d", $1);
        cargarEnTS(valorString, 2);
                
        if( esImpar($1) == 1 )
        {
            cantImpares++;
            t_info info;
            strcpy(info.valor, valorString);
            listaPtr = crearHoja(&info);
            flag = 1; //bandera para saber si la primer CTE fue impar.
        }
    } |
    lista COMA CTE {
        printf("\n Regla 9: lista -> lista COMA CTE");
        char valorString[100];
        sprintf(valorString, "%d", $3);
        cargarEnTS(valorString, 2);
    
        if( esImpar($3) == 1 )
        {
            cantImpares++;
            
            t_info infoCTE;
            strcpy(infoCTE.valor, valorString);
            
            if(flag != 1) {
                listaPtr = crearHoja(&infoCTE);
                flag = 1; //bandera para saber si la primer CTE fue impar.
            } else {
                t_info infoSuma;
                strcpy(infoSuma.valor, "+");
                listaPtr = crearNodo(&infoSuma, listaPtr, crearHoja(&infoCTE) );
            }
        }
    };

write:
    WRITE CTE_STRING {
        	
        if( strlen($2)>50 ){
            printf("\nERROR en WRITE: cadena mayor a tamaño maximo permitido(50)."); 
            exit(1);
        }
        writePtr = crearHojaT("WRITE");
        insertarHijo( &writePtr->izq, crearHojaT("stdout") );
        insertarHijo( &writePtr->der, crearHojaT($2) );
        printf("\n Regla 10: write -> WRITE CTE_STRING"); 
        cargarEnTS( $2, 4 );
    } |
    WRITE ID {
        if(existeId($2)==-1){
            printf("\nERROR en WRITE: variable no usada anteriormente.");
            exit(1); 
        }
        writePtr = crearHojaT("WRITE");
        insertarHijo(&writePtr->izq,crearHojaT("stdout"));
        insertarHijo(&writePtr->der,crearHojaT($2));
        printf("\n Regla 11: write -> WRITE ID");
        cargarEnTS( $2, 5 );
    };

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

int esImpar( int valor ){
    return (valor%2);
}