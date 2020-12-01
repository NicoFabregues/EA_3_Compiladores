%{
/*---------------------------INCLUDES---------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
#include "funciones.h"

/*-------------------DECLARACION DE VARIABLES-------------------*/
int cantImpares[100];
int cantElementos[100];
int lista_vacia = 0;
char ultimoId [30];
char idPivot [30];
int listaConst[100];
int flag = 0;
int tope = 0;

t_nodo* sPtr; //Variables para acciones semanticas -- Arbol sintactico.
t_nodo* progPtr; 
t_nodo* sentPtr;
t_nodo* writePtr;
t_nodo* readPtr;
t_nodo* asigPtr;
t_nodo* sumaPtr;
t_nodo* listaPtr;

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
        printf("\n\nFIN PROGRAMA\n\n");
        for(int j=0; j<cantLista; j++)
        {
            printf("\nLista: %d | Cant Impares: %d | Cant Elementos: %d", j, cantImpares[j],cantElementos[j]);
        }
        crearArchivoTS();
        sPtr=progPtr;
        
		grabarArbol(sPtr);
		generarArchivoGraphViz(sPtr);
		generarAssembler(sPtr);

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

        // Validacion pivot >= 1
        t_info info_if;
        strcpy(info_if.valor,"IF_val");
        t_info write;
        strcpy(write.valor,"WRITE");
        t_nodo* bloque_if = crearNodo(&write, crearHojaT("stderr"), crearHojaT("@erPivotInvalido"));
        t_info info_lt;
        strcpy(info_lt.valor,"Menor");
        t_info hijo_izq;
        strcpy(hijo_izq.valor,"@pivot");
        t_info hijo_der;
        strcpy(hijo_der.valor,"@valor_uno");
        t_nodo* condicion = crearNodo(&info_lt,crearHoja(&hijo_izq),crearHoja(&hijo_der));
        t_nodo* aux = crearNodo(&info_if, condicion, bloque_if);
        t_info sentencia;
        strcpy(sentencia.valor,"validacion");
        sentPtr = crearNodo(&sentencia, sentPtr, aux );
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
        cantImpares[cantLista] = 0;
    } ASIGNA suma_impar {
        asigPtr = sumaPtr; 
    };

suma_impar:
    SUMA_IMPAR PAR_A ID PYC C_A lista C_C PAR_C {
        printf("\n Regla 6: suma_impar -> SUMA_IMPAR PAR_A ID PYC C_A lista C_C PAR_C");

        if(flag == 0) // No hay elementos impares en la lista
        {
            sumaPtr = crearMensajeValidacion("@erSinImpares");
        }
        else
        {
            sumaPtr = listaPtr;
            strcpy(idPivot, $3);
            
            t_info infoTope1;
            infoTope1.nro = cantImpares[cantLista];
            t_nodo * nodoPila1= crearHoja(&infoTope1);
            apilar(&pilaImpares, nodoPila1);
            
            t_info infoTope2;
            infoTope2.nro = cantElementos[cantLista];
            t_nodo * nodoPila2= crearHoja(&infoTope2);
            apilar(&pilaElementos, nodoPila2);

            cantLista++;

            t_nodo* limpiarId = poner_id_en_cero(ultimoId);
            t_info infoSent;
            strcpy(infoSent.valor, "sentencia");
            t_nodo* nodoLimpiarId = crearNodo(&infoSent, limpiarId, sumaPtr);

            t_nodo * pivot = asignarPivot(idPivot);
            t_info bloque;
            strcpy(bloque.valor,"else");
            t_nodo * falso = crearNodo(&bloque,pivot,nodoLimpiarId);
            t_info cuerpo;
            strcpy(cuerpo.valor,"cuerpo");
            t_nodo * verdadero = crearMensajeValidacion("@erImparesInsuficientes" );
            t_nodo * cuerpoIf = crearNodo(&cuerpo,verdadero,falso);
            t_info nodoIf;
            strcpy(nodoIf.valor,"IF");
            t_nodo * condicion = crearCondicionValidacion(cantLista, idPivot, "@cantImpares");
            sumaPtr = crearNodo(&nodoIf,condicion,cuerpoIf);

            t_info cuerpo1;
            strcpy(cuerpo1.valor,"cuerpo1");
            verdadero = crearMensajeValidacion("@erElementosInsuficientes" );
            cuerpoIf = crearNodo(&cuerpo1,verdadero,sumaPtr);
            t_info nodoIf1;
            strcpy(nodoIf1.valor,"IF1");
            t_nodo * validacionElementos = crearCondicionValidacion(cantLista, idPivot, "@cantElementos");
            sumaPtr = crearNodo(&nodoIf1,validacionElementos,cuerpoIf);
            
            flag = 0;
        }
      
    } |
    SUMA_IMPAR PAR_A ID PYC C_A C_C PAR_C {
        printf("\n Regla 7: suma_impar ->  SUMA_IMPAR PAR_A ID PYC C_A C_C PAR_C");

        //Cargar error lista vacia.
        lista_vacia = 1;
        t_info info;
        strcpy(info.valor,"WRITE");
        sumaPtr = crearNodo(&info, crearHojaT("stderr"), crearHojaT("@erListaVacia"));
    } ;

lista: 
    CTE {
        printf("\n Regla 8: lista -> CTE %d", $1); 
        char valorString[100];
        sprintf(valorString, "%d", $1);
        cargarEnTS(valorString, 2);

        cantElementos[cantLista]++;        
        if( esImpar($1) == 1 )
        {
            listaConst[tope] = $1;
            
            t_info info_if;
            strcpy(info_if.valor,"IF");
            t_nodo * bloque_if = crearNodoBloqueIf(listaConst[tope], ultimoId);
            t_nodo * condicion = crearNodoCondicion();
            listaPtr = crearNodo(&info_if,condicion,bloque_if);
            
            flag = 1; //bandera para saber si la primer CTE fue impar.
            tope++;
            cantImpares[cantLista]++;
        }
    } |
    lista COMA CTE {
        printf("\n Regla 9: lista -> lista COMA CTE");
        char valorString[100];
        sprintf(valorString, "%d", $3);
        cargarEnTS(valorString, 2);
    
        cantElementos[cantLista]++; 
        if( esImpar($3) == 1 )
        {            
            if(flag != 1) {                    
                listaConst[tope] = $3;
                
                t_info info_if;
                strcpy(info_if.valor,"IF");
                t_nodo * bloque_if = crearNodoBloqueIf(listaConst[tope], ultimoId);
                t_nodo * condicion = crearNodoCondicion();
                listaPtr = crearNodo(&info_if,condicion,bloque_if);
                
                flag = 1; //bandera para saber si la primer CTE fue impar.
                tope++;   
                cantImpares[cantLista]++;                 
            } else {
                listaConst[tope] = $3;
                cantImpares[cantLista]++;
                
                t_info infoCTE;
                strcpy(infoCTE.valor, valorString);

                t_info info_if;
                strcpy(info_if.valor,"IF");
                t_nodo * bloque_if = crearNodoBloqueIf(listaConst[tope], ultimoId);
                t_nodo * condicion = crearNodoCondicion();
                t_nodo * aux = crearNodo(&info_if,condicion,bloque_if);
                t_info sentencia;
                strcpy(sentencia.valor,"Sentencia");
                listaPtr = crearNodo(&sentencia,listaPtr,aux);
                tope++;
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
        crear_pila(&pilaIf);
        crear_pila(&pilaImpares);
        crear_pila(&pilaElementos);
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