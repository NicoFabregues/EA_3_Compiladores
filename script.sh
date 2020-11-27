clear
flex EA3.l
bison -dyv EA3.y
gcc lex.yy.c y.tab.c -o compilador
./compilador test.txt
dot -Tpng -O graphInfo.dot
rm lex.yy.c
rm y.output
rm y.tab.c
rm y.tab.h
#rm graphInfo.dot
