#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

//Structs y variables globales
char tipo[6][14]={"","ENTERO","CTE ENTERA","STRING","CTE STRING","ID"};
typedef struct {
    char nombre[100]; // Nombre del token
    char tipo[14];  // Tipo de Dato
    int  tipoDato; // Manejo interno del tipo tipo[14]
    int flag;  // Para saber si el token fue almacenado o no en la TS
    char valor[100];
    int longitud;
} tsimbolo; // tabla de simbolos

tsimbolo simbolo[100];

// Declaraciones
void crearArchivoTS(void);
int cargarEnTS (char*, int);


// Funciones

void crearArchivoTS(void) {
	FILE *fp;
	int x, i;
	fp = fopen ( "ts.txt", "w+" );
	if (fp == NULL) {
		fputs ("File error",stderr); 
		exit (1);
	}
	    
    fprintf(fp, "NOMBRE %33s | TIPO %9s | VALOR %34s | LONGITUD %12s\n", " ", " ", " ", " ");
    for (i=0; i<120; i++)
        fprintf(fp, "-");
    fprintf(fp, "\n");   
    
	for (x = 0; x < 100; x++)
    {
        if( simbolo[x].tipoDato != 0 )
            fprintf(fp, "%-50s | %-14s | %-50s | %-10d\n", simbolo[x].nombre, simbolo[x].tipo, simbolo[x].valor, simbolo[x].longitud);
        else
            break;    		
	}
	fclose(fp);
	
    printf("\n\nSe ha cerrado el archivo y la Tabla de Simbolos fue cargada sin errores.\n");
        
 }

int cargarEnTS ( char *nombre, int val ){
    printf("\n\n-%s--|%d\n", nombre, val);
    int x;
	int l_repetido=0;
    char nombreConGuion[strlen(nombre)+1];    
    printf("\n\nAAAA\n");
    for (x=0; x<100; x++ ){
        if (simbolo[x].flag==1){//para saber si el token ya esta en la tabla
            if (strcmp (nombre,simbolo[x].nombre)==0){
                return x;              
            }else{
                strcpy(nombreConGuion, "_");   
                strcat(nombreConGuion, nombre);
                if (strcmp (nombreConGuion,simbolo[x].nombre)==0){
                    return x;
                }
            }
        }
    }
    printf("\n\nBBBB\n");
    for (x=0; x<100 ; x++){
        if(simbolo[x].flag==0){

            if(val == 2 || val == 4){
                if( val == 4 )
                {
                    simbolo[x].longitud = strlen(nombre+1);
                }
                strcpy(nombreConGuion, "_");   
                strcat(nombreConGuion, nombre);
                strcpy(simbolo[x].nombre,nombreConGuion);
                strcpy(simbolo[x].valor,nombre);
            }else{
                strcpy(simbolo[x].nombre,nombre);
            }
            
            strcpy(simbolo[x].tipo,tipo[val]);
            simbolo[x].tipoDato=val;
            simbolo[x].flag=1;//para indicar que ya se almaceno en la tabla

            return x;
        }
    }
	printf("\n\nCCCC\n");	
	return x;
 }//retorna posicion en la tabla de simbolos