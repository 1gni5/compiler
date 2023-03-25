grammar cc;

axiom : program EOF ;

program : 'int' 'main' '(' ')' compound ;
compound : '{' statement* '}' ;

statement : assignement | declaration | returnStmt;
returnStmt: 'return' expr ';' ;
declaration: 'int' IDENTIFIER (',' IDENTIFIER)* ('=' expr)? ';' ;
assignement: IDENTIFIER '=' expr ';' ;

expr: 
    op=('-' | '!' | '+' | '~') expr # unaryExpr
    | expr op=( '*' | '/' | '%' ) expr # multExpr
    | expr op=( '+' | '-' ) expr # addExpr
    | expr op=( '>=' | '<=' | '>' | '<' ) expr # cmpExpr
    | expr op=( '==' | '!=' ) expr # eqExpr
    | expr '&' expr # bitAndExpr
    | expr '^' expr # bitXorExpr
    | expr '|' expr # bitOrExpr
    | IDENTIFIER # varExpr
    | CONST # cstExpr
    | '(' expr ')' # parenthesis;

IDENTIFIER : [a-zA-Z]+;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
