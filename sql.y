%token SEMICOLON CREATE TABLE NAME LPAREN RPAREN COMMA INSERT INTO VALUES DELETE FROM LBRACKET RBRACKET

%%
program : statement SEMICOLON
        | program statement SEMICOLON
        ;

statement : create_table
          | insert_into
          | delete_from
          ;

create_table : CREATE TABLE NAME LPAREN columns RPAREN
             ;

columns : column
        | columns COMMA column
        ;

column : NAME NAME TYPE
       ;

insert_into : INSERT INTO NAME VALUES LPAREN values RPAREN
            ;

values : value
       | values COMMA value
       ;

value : NAME
      ;

delete_from : DELETE FROM NAME opt_id
            ;

opt_id : /* empty */
       | LBRACKET NAME RBRACKET
       ;
