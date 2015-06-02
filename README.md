# Squirrel

Squirrel é *(ou vai ser)* uma linguagem imperativa, mas com sonhos de OO,
tendo como principais influências python, c++ e java.

Este projeto está sendo desenvolvido como parte da disciplina de
Engenharia de linguagens do curso de Engenharia de Sofware na UFRN.

## Compilando o compilador

Para desenvolver Squirrel estamos utilizando o
[Bison](https://www.gnu.org/software/bison/) e o
[Flex](http://flex.sourceforge.net/), implementações do YACC e Lex, respectivamente.

### Compilar "na mão"
Para gerar um executável do compilador, é possível utilizar o flex e bison 
(junto com algum compilador C) diretamente. O modo recomendável, no entanto, é 
utilizando o waf (veja abaixo).

0. Gerar o léxico:

    lex lexer.l

0. Converter o arquivo yacc:

        yacc -d parser.y
    ou
    
        bison --define=y.tab.h parser.y

0. Gerar o executável:

        cc *.c src/commons/*.c libs/datastructs-c/src/*.c src/compiler/*.c 
        -I src/commons -I libs/datastructs-c/src -I src/compiler -std=gnu99 -o squirrel

### Utilizando o *Waf*

Outra opção é utilizar o [waf](https://github.com/waf-project/waf).

    ./waf configure build

Waf é um sistema de build que permite definir o script de construção do programa 
em python (ver o arquivo *wscript*).

O *waf* em si é um programa em python e pode ser incluso junto ao projeto.

O script irá criar o executável do compilador (squirrel) na pasta build.
