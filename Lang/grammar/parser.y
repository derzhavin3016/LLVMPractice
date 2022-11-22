%language "c++"
%skeleton "lalr1.cc"

%define api.value.type variant
%param {Driver* driver}

%code requires {
    #include <string>
    #include <memory>
    namespace yy { class Driver; }
    namespace langI { class INode; class ScopeNode; class FuncDeclNode; }
}

%code {
    #include "driver.hh"
    #include "ast.hh"
    namespace yy {parser::token_type yylex(parser::semantic_type* yylval, Driver* driver);}
}

%token <std::string> NAME
%token <int> INT
%token ADD SUB
       MUL DIV
       MOD ASSIGN
       OR AND
       XOR GREATER
       LESS GR_EQ
       LS_EQ IS_EQ
       NOT_EQ COMMA
       COLON SCOLON
       LP RP
       LB RB
       LBT RBT
       IF THEN ELSE
       WHILE RETURN
       ROUTINE VAR
       IS LOOP
       END ARRAY
       INTEGER
       SCAN PRINT
       ;

// %nterm<std::shared_ptr<glang::ScopeN>>     globalScope
// %nterm<std::shared_ptr<glang::ScopeN>>     scope
// %nterm<std::shared_ptr<glang::ScopeN>>     closeSc
// %nterm<std::shared_ptr<glang::ScopeN>>     openSc
// %nterm<std::shared_ptr<glang::INode>>      func
// %nterm<std::shared_ptr<glang::FuncDeclN>>  funcSign
// %nterm<std::shared_ptr<glang::INode>>      argList
// %nterm<std::shared_ptr<glang::INode>>      stm
// %nterm<std::shared_ptr<glang::INode>>      declVar
// %nterm<std::shared_ptr<glang::INode>>      lval
// %nterm<std::shared_ptr<glang::INode>>      if
// %nterm<std::shared_ptr<glang::INode>>      while
// %nterm<std::shared_ptr<glang::INode>>      expr1
// %nterm<std::shared_ptr<glang::INode>>      expr2
// %nterm<std::shared_ptr<glang::INode>>      expr3
// %nterm<std::shared_ptr<glang::INode>>      condition
// %nterm<std::shared_ptr<glang::INode>>      output
// %nterm<std::shared_ptr<glang::INode>>      stms
// %nterm<std::shared_ptr<glang::INode>>      return
// %nterm<std::shared_ptr<glang::INode>>      funcCall
// %nterm<std::shared_ptr<glang::INode>>      globalArrDecl
// %nterm<std::shared_ptr<glang::INode>>      arrAccess

%right ASSIGN

%right ELSE THEN

%left OR
%left AND
%left XOR
%left IS_EQ NOT_EQ
%left
      GREATER LESS
      GR_EQ LS_EQ
      ;

%left ADD SUB
%left MUL DIV MOD

%%

Parse   : Program {YYACCEPT;}

Program : RoutineDeclaration {}
        | VariableDeclaration {}
        | Program VariableDeclaration {}
        | Program RoutineDeclaration {}

VariableDeclaration : VAR NAME COLON Type IS Expression {}
                    | VAR NAME COLON Type {}

RoutineDeclaration : ROUTINE NAME IS Body END {}
                   | ROUTINE NAME COLON Type IS Body END {}
                   | ROUTINE NAME LP Parameters RP IS Body END {}
                   | ROUTINE NAME LP Parameters RP COLON Type IS Body END {}

Parameters : ParamDecl {}
           | Parameters COMMA ParamDecl {}

ParamDecl : NAME COLON Type {}

Type : PrimitiveType {}
     | ArrayType {}

PrimitiveType : INTEGER {}

ArrayType : ARRAY LBT Expression RBT PrimitiveType

Body : Statement {}
     | VariableDeclaration {}
     | Body Statement {}
     | Body VariableDeclaration {}

Statement : Assignment SCOLON {}
          | RoutineCall SCOLON {}
          | PRINT Expression SCOLON {}
          | SCAN NAME SCOLON {}
          | WhileLoop {}
          | IfStatement {}
          | ReturnStatement SCOLON {}

ReturnStatement : RETURN {}
                | RETURN Expression {}

Assignment : NAME ASSIGN Expression {}

RoutineCall : NAME LP RP {}
            | NAME LP Arguments RP {}

Arguments : Expression {}
          | Arguments COMMA Expression {}

WhileLoop : WHILE Expression LOOP Body END {}

IfStatement : IF Expression THEN Body END {}
            | IF Expression THEN Body ELSE Body END {}


Expression : Expression OR Expression {}
           | Expression AND Expression {}
           | Expression XOR Expression {}
           | Expression IS_EQ Expression {}
           | Expression NOT_EQ Expression {}
           | Expression GREATER Expression {}
           | Expression LESS Expression {}
           | Expression LS_EQ Expression {}
           | Expression GR_EQ Expression {}
           | Expression ADD Expression {}
           | Expression SUB Expression {}
           | Expression MUL Expression {}
           | Expression DIV Expression {}
           | Expression MOD Expression {}
           | ExpressionTerm {}

ExpressionTerm : LP Expression RP {}
               | Primary {}

Primary : INT {}
        | ModPrimary {}

ModPrimary : NAME {}
           | NAME LBT Expression RBT {}
           | RoutineCall {}

%%

namespace yy {

parser::token_type yylex (parser::semantic_type* yylval, Driver* driver)
{
  return driver->yylex(yylval);
}

void parser::error (const std::string& msg)
{
  std::cout << msg << " in line: " << std::endl;
}
}
