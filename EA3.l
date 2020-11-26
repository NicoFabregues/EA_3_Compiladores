%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "y.tab.h"

    void informarError(char *);

    FILE *yyin;
    char *yyltext;
    int MAXINT = 32768;
    int MININT = -32768;
    char lexema[100];

%}

%option noyywrap
%option yylineno

/*/----Palabras Reservadas----/*/
WRITE               "WRITE"
READ                "READ"
SUMA_IMPAR          "sumaimpar"

/*----Caracteres Especiales----*/

ASIGNA              "="
C_A                 "["
C_C                 "]"
PAR_A               "("
PAR_C               ")"
PYC                 ";"
COMA                ","
ESP                 " "
COM                 "\""

DOS_PUNTOS          ":"
ADMIRACION_ABRE     "¡"
ADMIRACION_CIERRA   "!"
OP_PORCENTAJE       "%"
PUNTO               "."
ARROBA              "@"
NUMERAL             "#"
PREGUNTA_ABRE       "¿"
PREGUNTA_CIERRA     "?"
GUION               "-"

DIGITO              [0-9]
LETRA               [a-zA-ZáéíóúñÁÉÍÓÚÑ]
SIMBOLOS            {ASIGNA}*|{C_A}*|{C_C}*|{PAR_A}*|{PAR_C}*|{PYC}*|{COMA}*|{DOS_PUNTOS}*|{ADMIRACION_ABRE}*|{ADMIRACION_CIERRA}*|{OP_PORCENTAJE}*|{PUNTO}*|{ARROBA}*|{NUMERAL}*|{PREGUNTA_ABRE}*|{PREGUNTA_CIERRA}*|{GUION}*

ID                  {LETRA}({LETRA}|{DIGITO})*
CTE_STRING          {COM}({DIGITO}*|{LETRA}*|{ESP}*|{SIMBOLOS}*)*{COM}   
CTE                 {DIGITO}+

%%

{WRITE}                 {/*printf("\nWRITE");*/return WRITE;};
{READ}                  {/*printf("\nREAD");*/return READ;};
{SUMA_IMPAR}            {/*printf("\nSUMA_IMPAR");*/return SUMA_IMPAR;};

{ASIGNA}                {/*printf("\nASIGNA");*/return ASIGNA;};
{C_A}                   {/*printf("\nC_A");*/return C_A;};
{C_C}                   {/*printf("\nC_C");*/return C_C;};
{PAR_A}                 {/*printf("\nPAR_A");*/return PAR_A;};
{PAR_C}                 {/*printf("\nPAR_C");*/return PAR_C;};
{PYC}                   {/*printf("\nPYC");*/return PYC;};
{COMA}                  {/*printf("\nCOMA");*/return COMA;};

{ID}                    {
                            /*
                            sprintf(lexema,"%s", yytext);
                            if(strlen(yytext) > 100) {
                                informarError(yytext);
                            }
                                
                            yylval.str_val=lexema;
                            
                            printf("\nID");*/
                            sprintf(lexema,"%s", yytext);
                            yylval.str_val=lexema;
                            return ID;
                        }
                    
{CTE_STRING}            {
                            /*
                            if(strlen(yytext) > 100) {
                                informarError(yytext);
                            }
                            yylval.str_val=yytext;
                            
                            printf("\nCTE_STRING");*/
                            yylval.str_val=yytext;
                            return CTE_STRING;
                        }

{CTE}                   { 
                            /*
                            if (strtol(yytext,(char **)NULL,10) < MININT ){
                                printf("\nConstante entera fuera de rango: %s, minimo permitido %d", yytext, MININT);
                            }else{
                                if(strtol(yytext,(char **)NULL,10) > MAXINT){
                                    printf("\nConstante entera fuera de rango: %s, maximo permitido %d", yytext, MAXINT);
                                } else {
                                    yylval.int_val=strtol(yytext,(char **)NULL,10);
                                    return CTE;
                                }
                            }
                            
                            printf("\nCTE");*/
                            yylval.int_val=strtol(yytext,(char **)NULL,10);
                            return CTE;
                        };
     
{ESP}
"\n"
"\t"
"\n\t"
"\r\n"

.                       {informarError("Caracter no reconocido");}

%%

void informarError(char *error) {
  printf("\n\n¡ERROR LEXICO! %s.\n", error);
  exit(1);
}

