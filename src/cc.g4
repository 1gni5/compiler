grammar cc;

axiom : program EOF ;

program : 'int' 'main' '(' ')' compound ;
compound : '{' statement* '}' ;

statement : assignement | declaration | returnStmt;
returnStmt: 'return' expr ';' ;
declaration: 'int' IDENTIFIER (',' IDENTIFIER)* ('=' expr)?';' ;
assignement: IDENTIFIER '=' expr ';' ;
expr: 
    expr op=( '*' | '/' ) expr # multiplication
    | expr op=( '+' | '-' ) expr # addition
    | expr op=( '>=' | '<=' | '==' | '!=' | '>' | '<' ) expr # comparison
    | op=('-' | '!') expr # unary
    | CONST # constExpression
    | IDENTIFIER # varExpression
    | '(' expr ')' # parenthesis;

IDENTIFIER : [a-zA-Z]+;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
