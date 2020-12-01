include macros.asm
include number.asm

.MODEL LARGE
.386
.STACK 200h
.DATA

@str0 	db "Ingrese un valor pivot mayor o igual a 1:", "$", 41 dup (?)
pivot 	dd ?
resul 	dd ?
@int3 	dd 1.00
@int4 	dd 2.00
@int5 	dd 3.00
@int6 	dd 4.00
@int7 	dd 5.00
@str8 	db "El resultado es: ", "$", 17 dup (?)
@aux1	dd ?
@aux2	dd ?
@aux3	dd ?
@aux4	dd ?
@aux5	dd ?
@aux6	dd ?
@erPivotInvalido 	 db "El valor debe ser >= 1", "$", 22 dup (?)
@erSinImpares 	 db "Elementos impares no encontrados", "$", 32 dup (?)
@erImparesInsuficientes 	 db "No existen suficientes elementos impares para el calculo", "$", 56 dup (?)
@erElementosInsuficientes 	 db "La lista tiene menos elementos que el indicado", "$", 46 dup (?)
@erListaVacia 	 db "La lista esta vacia", "$", 19 dup (?)
@rangoMinimo  dd 1.00
@valor_cero  dd 0.00
@valor_uno  dd 1.00
@pivot  dd ?
@cantImpares1  dd 3.00
@cantElementos1  dd 5.00

.CODE

START:
MOV EAX, @DATA
MOV DS, EAX
MOV ES, EAX

	displayString @str0
	newLine
	getFloat pivot,0
	fld pivot
	fstp @pivot
	fld @pivot
	fld @valor_uno
	fxch
	fcomp
	ffree St(0)
	fstsw ax
	sahf
	jae _erPivotInvalido0
	displayString @erPivotInvalido
	jmp finalizar_por_error
_erPivotInvalido0:
	fld pivot
	fld @cantElementos1
	fxch
	fcomp
	ffree St(0)
	fstsw ax
	sahf
	jna _if0
	displayString @erElementosInsuficientes
	newLine
	jmp fin_lista0
	jmp _if1
_if0:
	fld pivot
	fld @cantImpares1
	fxch
	fcomp
	ffree St(0)
	fstsw ax
	sahf
	jna _if1
	displayString @erImparesInsuficientes
	newLine
	jmp fin_lista0
	jmp _if2
_if1:
	fld pivot
	fstp @pivot
	fld @valor_cero
	fstp resul
	fld @pivot
	fld @valor_cero
	fxch
	fcomp
	ffree St(0)
	fstsw ax
	sahf
	jna _if2
	fld @pivot
	fld @valor_uno
	fsub
	fstp @aux1
	fld @aux1
	fstp @pivot
	fld resul
	fld @int3
	fadd
	fstp @aux2
	fld @aux2
	fstp resul
_if2:
	fld @pivot
	fld @valor_cero
	fxch
	fcomp
	ffree St(0)
	fstsw ax
	sahf
	jna _if3
	fld @pivot
	fld @valor_uno
	fsub
	fstp @aux3
	fld @aux3
	fstp @pivot
	fld resul
	fld @int5
	fadd
	fstp @aux4
	fld @aux4
	fstp resul
_if3:
	fld @pivot
	fld @valor_cero
	fxch
	fcomp
	ffree St(0)
	fstsw ax
	sahf
	jna _if4
	fld @pivot
	fld @valor_uno
	fsub
	fstp @aux5
	fld @aux5
	fstp @pivot
	fld resul
	fld @int7
	fadd
	fstp @aux6
	fld @aux6
	fstp resul
_if4:
fin_lista0:
	displayString @str8
	newLine
	displayFloat resul,2
	newLine
finalizar_por_error:
MOV EAX, 4C00h
INT 21h

END START
