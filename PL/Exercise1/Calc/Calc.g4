grammar Calc;

// Programs 

prog
	:	com* EOF
	;

// Commands

com
	:	PUT expr EOL         # put
	|	SET ID ASSN expr EOL # set
	;

// Expressions

expr
	:	prim (operator+=(PLUS | MINUS | TIMES | DIVIDE) prim)* # op
	;

prim		        
	:	NUM                  # num
	|	ID                   # id
	|	LPAR expr RPAR       # parens
	;

// Lexicon

PUT	:	'put' ;
SET	:	'set' ;

ASSN	:	'=' ;
PLUS	:	'+' ;
MINUS	:	'-' ;
TIMES	:	'*' ;
DIVIDE	:	'/' ;
LPAR	:	'(' ;
RPAR	:	')' ;

ID	:	'a'..'z' ;
NUM	:	'0'..'9'+ ;

EOL	:	'\r'? '\n' ;

SPACE	:	(' ' | '\t')+  -> skip ;
COMMENT :       '//' ;
LNGCOMM :	'/*';
