grammar cc;

axiom : program EOF ;

program : 'int' 'main' '(' ')' '{' returnStmt '}' ;

returnStmt: RETURN expr ';' ;
expr: 
    expr op=( '*' | '/' ) expr # multiplication
    | expr op=( '+' | '-' ) expr # addition
    | CONST # constExpression
    | '(' expr ')' # parenthesis;

RETURN : 'return' ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
