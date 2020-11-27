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

typedef struct
{
    char valor[50];
    int nro;
    int nroNodo;
}t_info;

typedef struct s_nodo
{
    t_info info;
    struct s_nodo *izq;
    struct s_nodo *der;
}t_nodo;

// Variables globales
int nroNodo;


// Declaraciones
void crearArchivoTS(void);
int cargarEnTS (char*, int);
int existeId(char * id);

/*-------------------FUNCIONES ARBOL-------------------*/
t_nodo * crearHoja(const t_info *d);
t_nodo * crearNodo(const t_info *d, t_nodo * hijo_izq, t_nodo * hijo_der);
t_nodo * crearHojaT(const char* info);
void insertarHijo (t_nodo ** puntero, t_nodo * hijo);
t_nodo * copiarNodo(t_nodo* nodo);
int esHoja(t_nodo* nodo);
int contarAux(t_nodo* nodo);
void recorrer_en_orden(const t_nodo* nodo);
void recorrer_guardando(const t_nodo* nodo, FILE* pf);
void grabarArbol(t_nodo* arbol);
void recorrerGenerandoCodigo(t_nodo* nodo, FILE* pf);
void recorrerGenerandoViz(const t_nodo* nodo, FILE* pf);
void generarArchivoGraphViz(t_nodo *raiz);
void enumerarNodos(t_nodo *n);
char * reemplazarCaracter(char const * const original,  char const * const pattern,  char const * const replacement);

// Funciones

void crearArchivoTS(void) {
	FILE *fp;
	int x, i;
	fp = fopen ( "ts.txt", "w+" );
	if (fp == NULL) {
		fputs ("File error",stderr); 
		exit (1);
	}
	    
    fprintf(fp, "NOMBRE %43s | TIPO %9s | VALOR %44s | LONGITUD %12s\n", " ", " ", " ", " ");
    for (i=0; i<133; i++)
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
    int x;
	int l_repetido=0;
    char nombreConGuion[strlen(nombre)+1];    
    
    for (x=0; x<100; x++ ){
        if (simbolo[x].flag==1){//El token ya esta en la tabla
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
        else {//El token NO esta en la tabla
            if(val == 2 || val == 4){ //Es una CTE INT o STR
                if( val == 4 )//Si es CTE STR, cuanto su longitud
                {
                    simbolo[x].longitud = strlen(nombre+2);
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
	return x;
 }//retorna posicion en la tabla de simbolos

 int existeId(char * id){
	int  i;
	for(i = 0; i<100; i++){
        if (simbolo[i].flag==1){
            if(strcmp(simbolo[i].nombre,id) == 0)
                return i;
        }
	}
	return -1;
}

 ///////////////////////// FUNCIONES ARBOL //////////////////////////////////////////////

t_nodo * crearHoja(const t_info *d)
{
    t_nodo *p = (t_nodo*) malloc(sizeof(t_nodo));
    if(!p){ 
        printf("No hay memoria disponible. El programa se cerrará\n");
        exit(1);
    }
    p->info=*d;
    p->der=p->izq=NULL;
    return p;
}

t_nodo * crearNodo(const t_info *d, t_nodo * hijo_izq, t_nodo * hijo_der)
{
    t_nodo *p = (t_nodo*) malloc(sizeof(t_nodo));
    if(!p){ 
        printf("No hay memoria disponible. El programa se cerrará\n");
        exit(1);
    }
    p->info=*d;
    p->izq= hijo_izq;
    p->der= hijo_der;
    return p;
}

t_nodo * crearHojaT(const char* info)
{
    t_nodo *p = (t_nodo*) malloc(sizeof(t_nodo));
    if(!p){ 
        printf("No hay memoria disponible. El programa se cerrará\n");
        exit(1);
    }
    strcpy(p->info.valor,info);
    p->der=p->izq=NULL;
    return p;
}

void insertarHijo (t_nodo ** puntero, t_nodo * hijo){
    *puntero=hijo;
}

t_nodo * copiarNodo(t_nodo* nodo){
    if(!nodo)
        return NULL;
    t_nodo *nuevo=(t_nodo*)malloc(sizeof(t_nodo));
    nuevo->info=nodo->info;
    if(nodo->izq)
        nuevo->izq=copiarNodo(nodo->izq);
    else
        nuevo->izq=NULL;
    if(nodo->der)
        nuevo->der=copiarNodo(nodo->der);
    else
        nuevo->der=NULL;
    return nuevo;
}

int esHoja(t_nodo* nodo){
    if(nodo->izq==NULL&&nodo->der==NULL)
        return 1;
    return 0;
}
/*
int contarAux(t_nodo* nodo){ //Cuenta los operadores(?)
    if(nodo){
        if(strcmp(nodo->info.valor,"*") ==0||strcmp(nodo->info.valor,"-")==0
            ||strcmp(nodo->info.valor,"+") ==0||strcmp(nodo->info.valor,"/")==0
            ||strcmp(nodo->info.valor,"++") ==0)
            return 1+contarAux(nodo->izq)+contarAux(nodo->der);
        else 
            return contarAux(nodo->izq)+contarAux(nodo->der);
    }
    return 0;
}*/

void recorrer_en_orden(const t_nodo* nodo)
{
    if(nodo)
    {
        if(nodo->izq!=NULL&&nodo->der!=NULL)
            printf("%s\t%s\t%s\n", nodo->info.valor,nodo->izq->info.valor,nodo->der->info.valor);
        recorrer_en_orden(nodo->izq);
        recorrer_en_orden(nodo->der);
    }
}

void recorrer_guardando(const t_nodo* nodo, FILE* pf)
{
    if(nodo)
    {
        if(nodo->izq!=NULL&&nodo->der!=NULL)
            fprintf(pf,"%-32s\t%-32s\t%-32s\n", nodo->info.valor,nodo->izq->info.valor,nodo->der->info.valor);
        recorrer_guardando(nodo->izq,pf);
        recorrer_guardando(nodo->der,pf);
    }
}

void grabarArbol(t_nodo* arbol)
{
    FILE*pf=fopen("intermedia.txt","w+");
    if(!pf){
        printf("Error al guardar el arbol\n");
        return;
    }
    fprintf(pf,"%-32s|\t%-32s|\t%-32s\n","PADRE","HIJO IZQ","HIJO DER");
    recorrer_guardando(arbol,pf);
    fclose(pf);
}

void recorrerGenerandoCodigo(t_nodo* nodo, FILE* pf)
{/*
    if(nodo)
    {
        if(strcmp(nodo->info.valor,"if")==0){
            contIf++;
            contCondiciones=1;
            t_info info;
            info.nro=contIf;
            ponerEnPila(&pilaNroIf,crearHoja(&info));
            if(strcmp(nodo->izq->info.valor,"AllEqual")!=0)
                fprintf(pf,"if_%d:\n",contIf);
            esCondWhile=0;
            if(strcmp(nodo->der->info.valor,"bloque if")==0){
                nroElse++;
            }
        }
        if(strcmp(nodo->info.valor,"AllEqual")==0){
            contIf++;
            t_info info;
            info.nro=contIf;
            ponerEnPila(&pilaNroIf,crearHoja(&info));
            fprintf(pf,"if_%d:\n",contIf);
            esCondWhile=0;
            if(strcmp(nodo->der->info.valor,"bloque if")==0){
                nroElse++;
            }
        }
        if(strcmp(nodo->info.valor,"while")==0){
            contCondiciones=1;
            contWhile++;
            t_info info;
            info.nro=contWhile;
            ponerEnPila(&pilaNroWhile,crearHoja(&info));
            fprintf(pf,"while_%d:\n",contWhile);
            esCondWhile=1;
        }
        if(strcmp(nodo->info.valor,"or")==0){
                esOr=1;
                if(nroElse != 0)
                    nroElse++;
        }
        if(strcmp(nodo->info.valor,"and")==0){
                if(nroElse != 0)
                    nroElse++;
        }
        recorrerGenerandoCodigo(nodo->izq,pf);
        if(strcmp(nodo->info.valor,"if")==0){
            fprintf(pf,"true_if_%d:\n",(*pilaNroIf).info.info.nro);
        }
        if(strcmp(nodo->info.valor,"while")==0){
            fprintf(pf,"true_while_%d:\n",(*pilaNroWhile).info.info.nro);
        }
        if(strcmp(nodo->info.valor,"bloque if")==0){
            fprintf(pf,"\tjmp\t end_if_%d\n",(*pilaNroIf).info.info.nro);
            fprintf(pf,"else_if_%d:\n",(*pilaNroIf).info.info.nro);
        }
        recorrerGenerandoCodigo(nodo->der,pf);
        if(esHoja(nodo)==0&&esHoja(nodo->izq)&&esHoja(nodo->der)){ //Si solo tiene hijos hojas
            grabarOperacionAssembler(nodo->izq,nodo->der,nodo,pf);
        }
    }*/
}

void recorrerGenerandoViz(const t_nodo* nodo, FILE* pf)
{
    if(nodo)
    {
        if(nodo->izq!=NULL&&nodo->der!=NULL){
            fprintf(pf,"\t%d[label=<%s>]\n", nodo->info.nroNodo,nodo->info.valor);
            fprintf(pf,"\t%d[label=<%s>]\n", nodo->izq->info.nroNodo,nodo->izq->info.valor);
            fprintf(pf,"\t%d[label=<%s>]\n", nodo->der->info.nroNodo,nodo->der->info.valor);
            if(nodo->izq)
                fprintf(pf,"\t%d--%d\n", nodo->info.nroNodo,nodo->izq->info.nroNodo);
            if(nodo->der)
                fprintf(pf,"\t%d--%d\n", nodo->info.nroNodo,nodo->der->info.nroNodo);
        }
        recorrerGenerandoViz(nodo->izq,pf);
        recorrerGenerandoViz(nodo->der,pf);
    }
}

void enumerarNodos(t_nodo *n){
    if(n){
        n->info.nroNodo=nroNodo;
        nroNodo++;
        enumerarNodos(n->izq);
        enumerarNodos(n->der);
    }
}

void generarArchivoGraphViz(t_nodo *raiz){
    nroNodo=0;
    enumerarNodos(raiz);
    FILE*pf=fopen("graphInfo.dot","w+");
    if(!pf){
        printf("\nError al generar el archivo para GraphViz\n");
        return;
    }
    fprintf(pf, "graph g{\n");
    recorrerGenerandoViz(raiz,pf);
    fprintf(pf, "}\n");
    fclose(pf);
}

char * reemplazarCaracter(char const * const original,  char const * const pattern,  char const * const replacement) 
{
	size_t const replen = strlen(replacement);
	size_t const patlen = strlen(pattern);
	size_t const orilen = strlen(original);
	size_t patcnt = 0;
	const char * oriptr;
	const char * patloc;
	for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen){
		patcnt++;
	}
  {
	size_t const retlen = orilen + patcnt * (replen - patlen);
    char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );
    if (returned != NULL)
    {
	char * retptr = returned;
	for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen){
	size_t const skplen = patloc - oriptr;
	strncpy(retptr, oriptr, skplen);
	retptr += skplen;
	strncpy(retptr, replacement, replen);
	retptr += replen;
	}
	strcpy(retptr, oriptr);
	}
	return returned;
	}
}