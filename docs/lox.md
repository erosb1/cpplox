This is the entire grammar of the cool programming language in EBNF:
```
program          :=  declaration* EOF;

declaration      :=  funDecl
                  |  varDecl
                  |  statement ;

funDecl          :=  "fun" function ;
varDecl          :=  "var" IDENTIFIER [ "=" expression ] ";" ;

statement        :=  exprStmt
                  |  ifStmt
                  |  printStmt 
                  |  returnStmt 
                  |  whileStmt 
                  |  block ;
                  
exprStmt         :=  expression ";" ;
ifStmt           :=  "if" "(" expression ")" statement [ "else" statement ] ;
printStmt        :=  "print" expression ";" ;
returnStmt       :=  "return" [ expression ] ";" ;
whileStmt        :=  "while" "(" expression ")" statement ;
block            :=  "{" declaration* "}" ;

expression       :=  assignment ;
assignment       :=  IDENTIFIER "=" assignment | logic_or ;
logic_or         :=  logic_and ( "or" logic_and )* ;
logic_and        :=  equality ( "and" equality )* ;
equality         :=  comparison ( ( "!=" | "==" ) comparison )* ;
comparison       :=  term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term             :=  factor ( ( "-" | "+" ) factor )* ;
factor           :=  unary ( ( "/" | "*" ) unary )* ;
unary            :=  ( "!" | "-" ) unary ;
primary          :=  "true" | "false" | "nil" | 
                     | NUMBER | STRING | IDENTIFIER 
                     | "(" expression ")" | "." IDENTIFIER ;

function         :=  IDENTIFIER "(" [ parameters ] ")" block ;
parameters       :=  IDENTIFIER ( "," IDENTIFIER )* ;
arguments        :=  expression ( "," expression )* ;

NUMBER           :=  DIGIT+ [ "." DIGIT+ ] ;
STRING           :=  "\"" <any char except "\"">* "\"" ;
IDENTIFIER       :=  ALPHA ( ALPHA | DIGIT )* ;
ALPHA            :=  "a" ... "z" | "A" ... "Z" | "_" ;
DIGIT            :=  "0" ... "9" ;
```


## Operator Precedence Levels
| Precedence | Operators | Description    |
|------------|-----------|----------------|
| 1          | ! -       | Unary          |
| 2          | / *       | Multiplicative |
| 3          | + -       | Additive       |
| 4          | > >= < <= | Relational     |
| 5          | == !=     | Equality       |
| 6          | and       | Logical AND    |
| 7          | or        | Logical OR     |
| 8          | =         | Assignment     |


## Tokens
| Token Type    | Lexeme    |
|---------------|-----------|
| LEFT_PAREN    | (         |
| RIGHT_PAREN   | )         |
| LEFT_BRACE    | {         |
| RIGHT_BRACE   | }         |
| COMMA         | ,         |
| DOT           | .         |
| MINUS         | -         |
| PLUS          | +         |
| SEMICOLON     | ;         |
| SLASH         | /         |
| STAR          | *         |
| BANG          | !         |
| BANG_EQUAL    | !=        |
| EQUAL         | =         |
| EQUAL_EQUAL   | ==        |
| GREATER       | \>        |
| GREATER_EQUAL | \>=       |
| LESS          | <         |
| LESS_EQUAL    | <=        |
| IDENTIFIER    | <chars>   |
| STRING        | "<chars>" |
| NUMBER        | <digits>  |
| AND           | and       |
| OR            | or        |
| TRUE          | true      |
| FALSE         | false     |
| FUN           | fun       |
| IF            | if        |
| ELSE          | else      |
| NIL           | nil       |
| PRINT         | print     |
| RETURN        | return    |
| VAR           | var       |
| WHILE         | while     |
| ERROR         |           |
| EOF           |           |


