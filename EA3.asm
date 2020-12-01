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
@int9 	dd 21.00
@int10 	dd 7.00
@int11 	dd 44.00
@int12 	dd 40.00
@int13 	dd 33.00
@aux1	dd ?
@aux2	dd ?
@aux3	dd ?
@aux4	dd ?
@aux5	dd ?
@aux6	dd ?
@aux7	dd ?
@aux8	dd ?
@aux9	dd ?
@aux10	dd ?
@aux11	dd ?
@aux12	dd ?
@aux13	dd ?
@aux14	dd ?
@erPivotInvalido 	 db "El valor debe ser >= 1", "$", 22 dup (?)
@erSinImpares 	 db "Elementos impares no encontrados", "$", 32 dup (?)
@erImparesInsuficientes 	 db "No existen suficientes elementos impares para el calculo", "$", 56 dup (?)
@erElementosInsuficientes 	 db "La lista tiene menos elementos que el indicado", "$", 46 dup (?)
@erListaVacia 	 db "La lista esta vacia", "$", 19 dup (?)
@rangoMinimo  dd 1.00
@valor_cero  dd 0.00
@valor_uno  dd 1.00
@pivot  dd ?
@cantImpares2  dd 4.00
@cantElementos2  dd 11.00
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
	jae _erPivotInvalido1
	displayString @erPivotInvalido
	jmp finalizar_por_error
_erPivotInvalido1:
	displayString @erSinImpares
	newLine
	displayString @str8
	newLine
	displayFloat resul,2
	newLine
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
	jae _erPivotInvalido2
	displayString @erPivotInvalido
	jmp finalizar_por_error
_erPivotInvalido2:
	displayString @erListaVacia
	newLine
	displayString @str8
	newLine
	displayFloat resul,2
	newLine
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
	jae _erPivotInvalido3
	displayString @erPivotInvalido
	jmp finalizar_por_error
_erPivotInvalido3:
	fld pivot
	fld @cantElementos2
	fxch
	fcomp
	ffree St(0)
	fstsw ax
	sahf
	jna _if5
	displayString @erElementosInsuficientes
	newLine
	jmp fin_lista1
	jmp _if6
_if5:
	fld pivot
	fld @cantImpares2
	fxch
	fcomp
	ffree St(0)
	fstsw ax
	sahf
	jna _if6
	displayString @erImparesInsuficientes
	newLine
	jmp fin_lista1
	jmp _if7
_if6:
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
	jna _if7
	fld @pivot
	fld @valor_uno
	fsub
	fstp @aux7
	fld @aux7
	fstp @pivot
	fld resul
	fld @int9
	fadd
	fstp @aux8
	fld @aux8
	fstp resul
_if7:
	fld @pivot
	fld @valor_cero
	fxch
	fcomp
	ffree St(0)
	fstsw ax
	sahf
	jna _if8
	fld @pivot
	fld @valor_uno
	fsub
	fstp @aux9
	fld @aux9
	fstp @pivot
	fld resul
	fld @int10
	fadd
	fstp @aux10
	fld @aux10
	fstp resul
_if8:
	fld @pivot
	fld @valor_cero
	fxch
	fcomp
	ffree St(0)
	fstsw ax
	sahf
	jna _if9
	fld @pivot
	fld @valor_uno
	fsub
	fstp @aux11
	fld @aux11
	fstp @pivot
	fld resul
	fld @int13
	fadd
	fstp @aux12
	fld @aux12
	fstp resul
_if9:
	fld @pivot
	fld @valor_cero
	fxch
	fcomp
	ffree St(0)
	fstsw ax
	sahf
	jna _if10
	fld @pivot
	fld @valor_uno
	fsub
	fstp @aux13
	fld @aux13
	fstp @pivot
	fld resul
	fld @int7
	fadd
	fstp @aux14
	fld @aux14
	fstp resul
_if10:
fin_lista1:
	displayString @str8
	newLine
	displayFloat resul,2
	newLine
finalizar_por_error:
MOV EAX, 4C00h
INT 21h

END START
