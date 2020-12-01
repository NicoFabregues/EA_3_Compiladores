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

typedef struct s_nodoPila{
    t_nodo info;
    struct s_nodoPila* psig;
}t_nodoPila;

typedef t_nodoPila *t_pila;

// Variables globales
int nroNodo;
int nroAux = 1;
int cantErroresMsg = 0;
int nroIf = 0;
t_pila pilaIf;
t_pila pilaImpares;
t_pila pilaElementos;
int cantLista = 0;
int nroLista = 0;
char ultimoPivot[50];
int nroErPivotInvalido =0;

/*  FUNCIONES TS  */
void crearArchivoTS(void);
int cargarEnTS ( char *nombre, int val );
int existeId(char * id);

/*  FUNCIONES PILA  */
int apilar(t_pila* pp,t_nodo* nodo);
int desapilar(t_pila* pp,t_nodo* info);
void crear_pila(t_pila* pp);

/*  FUNCIONES ARBOL  */
t_nodo * crearHoja(const t_info *d);
t_nodo * crearNodo(const t_info *d, t_nodo * hijo_izq, t_nodo * hijo_der);
t_nodo * crearHojaT(const char* info);
void insertarHijo (t_nodo ** puntero, t_nodo * hijo);
int contarAux(t_nodo* nodo);
void recorrer_en_orden(const t_nodo* nodo);
void recorrer_guardando(const t_nodo* nodo, FILE* pf);
void grabarArbol(t_nodo* arbol);
void recorrerGenerandoViz(const t_nodo* nodo, FILE* pf);
void generarArchivoGraphViz(t_nodo *raiz);
void recorrerGenerandoCodigo(t_nodo* nodo, FILE* fp);
void enumerarNodos(t_nodo *n);

/*  FUNCIONES ASM  */
void escribirASM(t_nodo* op1, t_nodo *op2, t_nodo *opr, FILE* pf);
void cargarDATA(FILE* fp, t_nodo *arbol);
void generarAssembler(t_nodo* arbol);
void generarCodigo (FILE* fp, t_nodo *arbol);
char * obtenerTipodb_dd(int tipoDato);
int obtenerTipo(char* valor);
int obtenerIndiceTS(char* nombre);

/*  FUNCIONES CREAR NODOS SINTACTICO  */
t_nodo * crearNodoBloqueIf(int x, char * id);
t_nodo * crearNodoCondicion();
t_nodo * asignarPivot();
t_nodo * sumarValor(int x, char * id);
t_nodo * restarCantidad();
t_nodo * crearMensajeValidacion(char* error);
t_nodo * crearCondicionValidacion(int cantLista, char * idPivot, char* valor);
t_nodo * poner_id_en_cero(char*);

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
                if( val == 4 )//Si es CTE STR, cuento su longitud
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

int contarAux(t_nodo* nodo){ //Cuenta los operadores(?)
    if(nodo){
        if( strcmp(nodo->info.valor,"+") == 0 || strcmp(nodo->info.valor,"-") == 0 )
            return 1+contarAux(nodo->izq)+contarAux(nodo->der);
        else 
            return contarAux(nodo->izq)+contarAux(nodo->der);
    }
    return 0;
}

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

void recorrerGenerandoCodigo(t_nodo* nodo, FILE* fp)
{
    if(nodo)
	{
        if(strcmp(nodo->info.valor,"IF_val")==0)
        {
            t_nodo *aux = nodo->izq;
            fprintf(fp, "\tfld %s\n", aux->izq->info.valor);
            fprintf(fp, "\tfld %s\n", aux->der->info.valor);
            fprintf(fp, "\tfxch\n");
            fprintf(fp, "\tfcomp\n");
            fprintf(fp, "\tffree St(0)\n");
            fprintf(fp, "\tfstsw ax\n");
            fprintf(fp, "\tsahf\n");
            
            fprintf(fp, "\tjae _erPivotInvalido%d\n", nroErPivotInvalido );
            fprintf(fp, "\tdisplayString @erPivotInvalido\n");            
            fprintf(fp, "\tjmp finalizar_por_error\n");
            fprintf(fp, "_erPivotInvalido%d:\n", nroErPivotInvalido);
            nroErPivotInvalido++;

        }

        if(strcmp(nodo->info.valor,"IF1")==0)
        {
            t_nodo *aux = nodo->izq;
            fprintf(fp, "\tfld %s\n", aux->izq->info.valor);
            fprintf(fp, "\tfld %s\n", aux->der->info.valor);
            fprintf(fp, "\tfxch\n");
            fprintf(fp, "\tfcomp\n");
            fprintf(fp, "\tffree St(0)\n");
            fprintf(fp, "\tfstsw ax\n");
            fprintf(fp, "\tsahf\n");
            fprintf(fp, "\tjna _if%d\n", nroIf);
            t_info info;
            info.nro = nroIf;
            t_nodo * nodoPila= crearHoja(&info);
            apilar(&pilaIf, nodoPila);
            nroIf++;
        }

        if(strcmp(nodo->info.valor,"IF")==0 )
        {
            t_nodo * aux = (t_nodo *) malloc (sizeof(t_nodo));;
            if(desapilar(&pilaIf, aux) != 0)
            {
                fprintf(fp, "\tjmp _if%d\n", aux->info.nro + 1);
                fprintf(fp, "_if%d:\n", aux->info.nro);
            }
            aux = nodo->izq;
            fprintf(fp, "\tfld %s\n", aux->izq->info.valor);
            fprintf(fp, "\tfld %s\n", aux->der->info.valor);
            fprintf(fp, "\tfxch\n");
            fprintf(fp, "\tfcomp\n");
            fprintf(fp, "\tffree St(0)\n");
            fprintf(fp, "\tfstsw ax\n");
            fprintf(fp, "\tsahf\n");
            fprintf(fp, "\tjna _if%d\n", nroIf);
            t_info info;
            info.nro = nroIf;
            t_nodo * nodoPila= crearHoja(&info);
            apilar(&pilaIf, nodoPila);
            nroIf++;
        }
        
        if(strcmp(nodo->info.valor,"else")==0)
        {
            t_nodo * aux = (t_nodo *) malloc (sizeof(t_nodo));;
            if(desapilar(&pilaIf, aux) != 0)
            {
                fprintf(fp, "\tjmp _if%d\n", aux->info.nro + 1);
                fprintf(fp, "_if%d:\n", aux->info.nro);
            }
        }

        recorrerGenerandoCodigo(nodo->izq,fp);
        recorrerGenerandoCodigo(nodo->der,fp);
        if(nodo->izq!=NULL&&nodo->der!=NULL)
        {            
            escribirASM(nodo->izq,nodo->der,nodo,fp);
        }
    }
}

void escribirASM(t_nodo* op1, t_nodo *op2, t_nodo *opr, FILE* pf)
{  
    t_nodo *aux;

    if(strcmp(opr->info.valor,"WRITE")==0)
    {
        if(strcmp(op1->info.valor,"stdout")==0){
            if(obtenerTipo(op2->info.valor)==5 )
            {
                fprintf(pf,"\tdisplayFloat %s,2\n", op2->info.valor);
            }
            else if(strstr(op2->info.valor, "@"))
            {
                fprintf(pf,"\tdisplayString %s\n", op2->info.valor);
            }
            else
            {
                fprintf(pf,"\tdisplayString @str%d\n", obtenerIndiceTS(op2->info.valor));
            }
            fprintf(pf, "\tnewLine\n");
        }
        
        if(strcmp(op1->info.valor,"stderr")==0){

            if(strcmp(op2->info.valor, "@erListaVacia") == 0 || strcmp(op2->info.valor, "@erSinImpares") == 0)
            {
                fprintf(pf,"\tdisplayString %s\n", op2->info.valor);
                fprintf(pf, "\tnewLine\n");
                //fprintf(pf, "jmp _fin\n");
            }

            if(strcmp(op2->info.valor, "@erElementosInsuficientes") == 0 || strcmp(op2->info.valor, "@erImparesInsuficientes") == 0 )
            {
                fprintf(pf,"\tdisplayString %s\n", op2->info.valor);
                fprintf(pf, "\tnewLine\n");
                fprintf(pf, "\tjmp fin_lista%d\n", nroLista);
            }
            
            if(strcmp(op2->info.valor, "@erPivotInvalido") == 0 )
            {
                //fprintf(pf, "jmp _fin\n");
            }
        }
    }

    if(strcmp(opr->info.valor,"READ")==0)
    {
        if(obtenerTipo(op2->info.valor)==5)
        {
            fprintf(pf,"\tgetFloat %s,0\n", op2->info.valor);
            /*
            fprintf(pf, "getFloat %s,2\n", op2->info.valor);
            fprintf(pf, "fld %s\n", op2->info.valor);
            fprintf(pf, "fld @rangoMinimo\n");
            fprintf(pf, "fxch\n");
            fprintf(pf, "fcomp\n");
            fprintf(pf, "ffree St(0)\n");
            fprintf(pf, "fstsw ax\n");
            fprintf(pf, "sahf\n");

            nroErPivotInvalido++;*/
            strcpy(ultimoPivot, op2->info.valor);
            //for(int j=0;j<cantLista;j++)
            //{
                fprintf(pf, "\tfld %s\n", ultimoPivot);
                fprintf(pf, "\tfstp @pivot\n");
            //}
            
        }
    }

    if(strcmp(opr->info.valor,"+")==0)
    {
        if(strstr(op1->info.valor,"@") || obtenerTipo(op1->info.valor) == 5)
        {
            fprintf(pf,"\tfld %s\n", op1->info.valor);
        }
        else
        {
            fprintf(pf,"\tfld @int%d\n", obtenerIndiceTS(op1->info.valor));
        }
        
        if(strstr(op2->info.valor,"@") || obtenerTipo(op2->info.valor) == 5)
        {
            fprintf(pf,"\tfld %s\n", op2->info.valor);
        }
        else
        {
            fprintf(pf,"\tfld @int%d\n", obtenerIndiceTS(op2->info.valor));
        }
        fprintf(pf,"\tfadd\n");
        fprintf(pf,"\tfstp @aux%d\n", nroAux);
        nroAux++;
    }

    if(strcmp(opr->info.valor,"-")==0)
    {
        if(strstr(op1->info.valor,"@") || obtenerTipo(op1->info.valor) == 5)
        {
            fprintf(pf,"\tfld %s\n", op1->info.valor);
        }
        else
        {
            fprintf(pf,"\tfld @int%d\n", obtenerIndiceTS(op1->info.valor));
        }

        if(strstr(op2->info.valor,"@") || obtenerTipo(op2->info.valor) == 5)
        {
            fprintf(pf,"\tfld %s\n", op2->info.valor);
        }
        else
        {
            fprintf(pf,"\tfld @int%d\n", obtenerIndiceTS(op2->info.valor));
        }
        fprintf(pf,"\tfsub\n");
        fprintf(pf,"\tfstp @aux%d\n", nroAux);
        nroAux++;
    }

    if(strcmp(opr->info.valor,"sent")==0 && strcmp(op2->info.valor,"IF1")==0){
        fprintf(pf,"fin_lista%d:\n", nroLista);
        nroLista++;
    }
    
    if(strcmp(opr->info.valor,"=")==0)
    {
        if(obtenerTipo(op2->info.valor) == 5)
        {
            fprintf(pf,"\tfld %s\n", op2->info.valor);
        }
        else
        {
            if(strcmp(op2->info.valor,"@valor_cero")==0){
                fprintf(pf,"\tfld %s\n", op2->info.valor);
            } else {
                fprintf(pf,"\tfld @aux%d\n", nroAux - 1);
            }
            
        }
        fprintf(pf,"\tfstp %s\n", op1->info.valor);
        
    }

    if(strcmp(opr->info.valor,"bloque_if")==0)
    {
        aux = (t_nodo *) malloc (sizeof(t_nodo));;
        if(desapilar(&pilaIf, aux) != 0)
        {
            fprintf(pf, "_if%d:\n", aux->info.nro);
        }
    }

    if(strcmp(opr->info.valor,"cuerpo")==0)
    {
        aux = (t_nodo *) malloc (sizeof(t_nodo));;
        if(desapilar(&pilaIf, aux) != 0)
        {
            fprintf(pf, "_if%d:\n", aux->info.nro);
        }
    }
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
    FILE*pf=fopen("intermedia.dot","w+");
    if(!pf){
        printf("\nError al generar el archivo para GraphViz\n");
        return;
    }
    fprintf(pf, "graph g{\n");
    recorrerGenerandoViz(raiz,pf);
    fprintf(pf, "}\n");
    fclose(pf);
}

void cargarDATA(FILE* fp, t_nodo *arbol){
    fprintf(fp, ".DATA\n\n");

    for (int x = 0; x < 100; x++)
    {
        if( simbolo[x].flag == 1 )
        {
            if(simbolo[x].tipoDato == 2)
            {
                fprintf(fp,  "@int%d \t%s %.2f\n", x, obtenerTipodb_dd(simbolo[x].tipoDato),atof(simbolo[x].valor));
            }
            else if(simbolo[x].tipoDato == 4)
            {
                fprintf(fp,  "@str%d \t%s %s, \"$\", %d dup (?)\n", x, obtenerTipodb_dd(simbolo[x].tipoDato),simbolo[x].valor, simbolo[x].longitud);
            }
            else{
                fprintf(fp,  "%s \t%s ?\n", simbolo[x].nombre, obtenerTipodb_dd(simbolo[x].tipoDato));
            }
        }else{
            break;
        }    		
	}

    //DECLARACION DE AUXILIARES
    int cantAux=contarAux(arbol);
    int j;
    for(j=0;j<cantAux;j++){
         fprintf(fp,  "@aux%d\tdd ?\n",j+1);
    }
    
    //Variables para mensajes de errores
    /*
    @erPivotInvalido "El valor debe ser >= 1"
    @erSinImpares "Elementos impares no encontrados"
    @erImparesInsuficientes "No existen suficientes elementos impares para el calculo"
    @erElementosInsuficientes "La lista tiene menos elementos que el indicado"
    @erListaVacia "La lista esta vacia"
    */
    fprintf(fp,  "@erPivotInvalido \t db \"El valor debe ser >= 1\", \"$\", 22 dup (?)\n");
    fprintf(fp,  "@erSinImpares \t db \"Elementos impares no encontrados\", \"$\", 32 dup (?)\n");
    fprintf(fp,  "@erImparesInsuficientes \t db \"No existen suficientes elementos impares para el calculo\", \"$\", 56 dup (?)\n");
    fprintf(fp,  "@erElementosInsuficientes \t db \"La lista tiene menos elementos que el indicado\", \"$\", 46 dup (?)\n");
    fprintf(fp,  "@erListaVacia \t db \"La lista esta vacia\", \"$\", 19 dup (?)\n");

    fprintf(fp,  "@rangoMinimo  dd 1.00\n");
    fprintf(fp,  "@valor_cero  dd 0.00\n");
    fprintf(fp,  "@valor_uno  dd 1.00\n");
    fprintf(fp,  "@pivot  dd ?\n");
    
    t_nodo * aux;
    for(int x = cantLista; x > 0; x--)
    {
        aux = (t_nodo *) malloc (sizeof(t_nodo));;
        if(desapilar(&pilaImpares, aux) != 0)
        {
            fprintf(fp,  "@cantImpares%d  dd %d.00\n", x, aux->info.nro);
        }

        if(desapilar(&pilaElementos, aux) != 0)
        {
            fprintf(fp,  "@cantElementos%d  dd %d.00\n", x, aux->info.nro);
        }
    }
    
}

char * obtenerTipodb_dd(int tipoDato){
    if(tipoDato==4){
        return "db";
    }else{
        return "dd";
    }
}

int obtenerTipo(char* valor){
    int i;
    for(i = 0; i<99; i++){
        if(simbolo[i].flag == 1)
        {
            if(strcmp(simbolo[i].nombre,valor)==0)
                return simbolo[i].tipoDato;
        }
    }
    return -1;
}

int obtenerIndiceTS(char* nombre)
{
    int x;
    for (x=0; x<99; x++ ){
        if ( strcmp(simbolo[x].valor, nombre) == 0 && simbolo[x].flag==1)
        {
            return x;
        }
    }
    return -1;
}

void generarAssembler(t_nodo* arbol){
    FILE*fp=fopen("Final.asm","w+");
    if(!fp){
        printf("Error al generar el assembler.\n");
        return;
    }
    fprintf(fp, "include macros.asm\n");
    fprintf(fp, "include number.asm\n\n");          
    fprintf(fp, ".MODEL LARGE\n");
    fprintf(fp, ".386\n");
    fprintf(fp, ".STACK 200h\n");

    cargarDATA(fp, arbol);
    
    nroAux=1;

    generarCodigo(fp, arbol);

    fprintf(fp, "finalizar_por_error:\n");
    fprintf(fp, "MOV EAX, 4C00h\n");
    fprintf(fp, "INT 21h\n\n");
    fprintf(fp, "END START");
    fprintf(fp, "\n");
	fclose(fp);
	
    printf("\n\nFin generacion codigo asm.\n");
}

void generarCodigo (FILE* fp, t_nodo *arbol) {
    fprintf(fp, "\n.CODE\n\n");
    fprintf(fp, "START:\n");
    fprintf(fp, "MOV EAX, @DATA\n");
    fprintf(fp, "MOV DS, EAX\n");
    fprintf(fp, "MOV ES, EAX\n\n");
    //recorrer_en_orden(arbol);
    recorrerGenerandoCodigo(arbol, fp);    
}

t_nodo * crearNodoBloqueIf(int x, char * id)
{
    t_nodo *aux;
    t_info info;
    strcpy(info.valor,"bloque_if");
    t_nodo *auxDer = restarCantidad();
    t_nodo *auxIzq = sumarValor(x, id);
    aux = crearNodo(&info,auxDer,auxIzq);

    return aux;
}

t_nodo * crearNodoCondicion()
{
    t_nodo *aux;
    t_info padre;
    strcpy(padre.valor,"Mayor");
    t_info hizq;
    strcpy(hizq.valor,"@pivot");
    t_info hder;
    strcpy(hder.valor,"@valor_cero");
    aux = crearNodo(&padre,crearHoja(&hizq),crearHoja(&hder));

    return aux;
}

t_nodo * asignarPivot(char * idPivot)
{
    t_nodo *aux;
    t_info padre;
    strcpy(padre.valor,"=");
    t_info hizq;
    strcpy(hizq.valor,"@pivot");
    t_info hder;
    strcpy(hder.valor,idPivot);
    aux = crearNodo(&padre,crearHoja(&hizq),crearHoja(&hder));

    return aux;
}

t_nodo * restarCantidad()
{
    t_nodo *aux;
    t_info rpadre;
    strcpy(rpadre.valor,"-");
    t_info rhizq;
    strcpy(rhizq.valor,"@pivot");
    t_info rhder;
    strcpy(rhder.valor,"@valor_uno");
    aux = crearNodo(&rpadre,crearHoja(&rhizq),crearHoja(&rhder));
    t_info apadre;
    strcpy(apadre.valor,"=");
    t_info ahizq;
    strcpy(ahizq.valor,"@pivot");
    aux = crearNodo(&apadre,crearHoja(&ahizq),aux);

    return aux;
}

t_nodo * sumarValor(int x, char * id)
{
    char valorString[100];
    t_nodo *aux;
    t_info rpadre;
    strcpy(rpadre.valor,"+");
    t_info rhizq;
    strcpy(rhizq.valor,id);
    t_info rhder;

    sprintf(valorString, "%d", x);
    strcpy(rhder.valor,valorString);
    aux = crearNodo(&rpadre,crearHoja(&rhizq),crearHoja(&rhder));
    t_info apadre;
    strcpy(apadre.valor,"=");
    t_info ahizq;
    strcpy(ahizq.valor,id);
    aux = crearNodo(&apadre,crearHoja(&ahizq),aux);

    return aux;
}

t_nodo * crearCondicionValidacion(int cantLista, char * idPivot, char* valor)
{
    t_nodo * aux;
    t_info padre;
    strcpy(padre.valor,"Mayor");
    t_info hizq;
    strcpy(hizq.valor,idPivot);
    t_info hder;
    char nroString[100];
    sprintf(nroString, "%d", cantLista);
    char valorString[100];
    strcpy(valorString, valor);
    strcat(valorString, nroString);
    strcpy(hder.valor,valorString);
    aux = crearNodo(&padre,crearHoja(&hizq),crearHoja(&hder));

    return aux;
}

t_nodo * crearMensajeValidacion(char* error)
{
    t_nodo * aux;
    t_info padre;
    strcpy(padre.valor,"WRITE");
    t_info hizq;
    strcpy(hizq.valor,"stderr");
    t_info hder;
    strcpy(hder.valor,error);
    aux = crearNodo(&padre,crearHoja(&hizq),crearHoja(&hder));

    return aux;
}

t_nodo * poner_id_en_cero(char* id){
    t_nodo * aux;
    t_info padre;
    strcpy(padre.valor,"=");
    t_info hizq;
    strcpy(hizq.valor, id);
    t_info hder;
    strcpy(hder.valor,"@valor_cero");
    aux = crearNodo(&padre,crearHoja(&hizq),crearHoja(&hder));

    return aux;
}

void crear_pila(t_pila* pp)
{
    *pp=NULL; 
}

int apilar(t_pila* pp,t_nodo* nodo)
{
    t_nodoPila* pn=(t_nodoPila*)malloc(sizeof(t_nodoPila));
    if(!pn)
        return 0;
    pn->info=*nodo;
    pn->psig=*pp;
    *pp=pn;
    return 1;
}

int desapilar(t_pila* pp,t_nodo* info)
{
    if(!*pp){
        return 0;
    }
    *info=(*pp)->info;
    *pp=(*pp)->psig;
    return 1;
}