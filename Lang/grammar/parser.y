%language "c++"
%require "3.7"
%skeleton "lalr1.cc"

%define parse.trace
%define parse.lac full

%define api.value.type variant
%define parse.error detailed
%locations
%param {Driver* driver}

%code requires {
    #include <string>
    #include <memory>
    #include "astdef.hh"
    namespace yy { class Driver; }
    namespace llvm { class Type; }
}

%code {
    #include "ast.hh"
    #include "driver.hh"
    namespace yy {parser::token_type yylex(parser::semantic_type* yylval, parser::location_type *yylloc, Driver* driver);}
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
%nterm<langI::pDNode>  RoutineDeclaration;

%nterm<std::string> RoutineName;
%nterm<langI::pINode> Program
                      Parameters
                      ParamDecl
                      Statement
                      ReturnStatement
                      Assignment
                      RoutineCall
                      Arguments
                      WhileLoop
                      IfStatement
                      Expression
                      ExpressionTerm
                      Primary
                      ;

%nterm <std::shared_ptr<langI::VarDeclNode>> VariableDeclaration GVarDecl;
%nterm <langI::pIStoreable> ModPrimary
%nterm <llvm::Type *> Type PrimitiveType ArrayType;

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

Program : RoutineDeclaration          { driver->m_gScope->addDecl($1, true); }
        | GVarDecl                    { driver->m_gScope->addDecl($1, true); }
        | Program GVarDecl            { driver->m_gScope->addDecl($2, true); }
        | Program RoutineDeclaration  { driver->m_gScope->addDecl($2, true); }

GVarDecl : VariableDeclaration { $$ = std::make_shared<langI::GlobDeclNode>($1); }

VariableDeclaration : VAR NAME COLON Type IS Expression SCOLON { $$ = std::make_shared<langI::VarDeclNode>($4, $2, $6); }
                    | VAR NAME COLON Type SCOLON              { $$ = std::make_shared<langI::VarDeclNode>($4, $2); }

RoutineDeclaration : RoutineName {
                                  driver->m_curFunc = std::make_shared<langI::FuncDeclNode>($1, driver->m_curFunParams, driver->m_curScope);
                                  driver->m_curScope->setFunc(driver->m_curFunc);
                                 } FuncBody {
                                              $$ = driver->m_curScope->getFunc();
                                            }
                   | RoutineName COLON Type {
                                              driver->m_curFunc = std::make_shared<langI::FuncDeclNode>($1, driver->m_curFunParams, driver->m_curScope, $3);
                                              driver->m_curScope->setFunc(driver->m_curFunc);
                                            } FuncBody {
                                                          $$ = driver->m_curScope->getFunc();
                                                        }
                   | RoutineName LP Parameters RP {
                                                    driver->m_curFunc = std::make_shared<langI::FuncDeclNode>($1, driver->m_curFunParams, driver->m_curScope);
                                                    driver->m_curScope->setFunc(driver->m_curFunc);
                                                  } FuncBody {
                                                                $$ = driver->m_curScope->getFunc();
                                                                driver->m_curFunParams.clear();
                                                              }
                   | RoutineName LP Parameters RP COLON Type {
                                                               driver->m_curFunc = std::make_shared<langI::FuncDeclNode>($1, driver->m_curFunParams, driver->m_curScope, $6);
                                                               driver->m_curScope->setFunc(driver->m_curFunc);
                                                             } FuncBody {
                                                                          $$ = driver->m_curScope->getFunc();
                                                                          driver->m_curFunParams.clear();
                                                                        }

FuncBody : IS Body END {}

RoutineName : ROUTINE NAME { driver->makeGlobalScopeChild(); $$ = $2; }

Parameters : ParamDecl {}
           | Parameters COMMA ParamDecl {}

ParamDecl : NAME COLON Type { driver->m_curFunParams.push_back(std::make_shared<langI::ParamDeclNode>($3, $1)); }

Type : PrimitiveType { $$ = $1; }
     | ArrayType     { $$ = $1; }

PrimitiveType : INTEGER { $$ = driver->m_ctx.getIntTy(); }

ArrayType : ARRAY LBT INT RBT PrimitiveType { $$ = langI::CodegenCtx::getArrTy($5, $3); }

Body : Statement                { driver->m_curScope->pushNode($1); }
     | VariableDeclaration      { driver->m_curScope->addDecl($1, true); }
     | Body Statement           { driver->m_curScope->pushNode($2); }
     | Body VariableDeclaration { driver->m_curScope->addDecl($2, true); }

Statement : Assignment SCOLON       { $$ = $1; }
          | RoutineCall SCOLON      { $$ = $1; }
          | PRINT Expression SCOLON { $$ = std::make_shared<langI::PrintNode>($2); }
          | WhileLoop               { $$ = $1; }
          | IfStatement             { $$ = $1; }
          | ReturnStatement SCOLON  { $$ = $1; }

ReturnStatement : RETURN            { $$ = std::make_shared<langI::RetVoidNode>(); }
                | RETURN Expression { $$ = std::make_shared<langI::RetNode>($2); }

Assignment : ModPrimary ASSIGN Expression { $$ = std::make_shared<langI::AsNode>($1, $3); }

RoutineCall : NAME LP RP { $$ = std::make_shared<langI::FuncCallNode>(driver->m_curScope->findName($1)); }
            | NAME LP Arguments RP {
                                     $$ = std::make_shared<langI::FuncCallNode>(driver->m_curScope->findName($1), driver->m_curArgs);
                                     driver->m_curArgs.clear();
                                   }

Arguments : Expression { driver->m_curArgs.push_back($1); }
          | Arguments COMMA Expression { driver->m_curArgs.push_back($3); }

WhileLoop : WHILE Expression LOOP Body END {}

IfStatement : IF Expression THEN Body END {}
            | IF Expression THEN Body ELSE Body END {}


Expression : Expression OR Expression      { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::Or, $3); }
           | Expression AND Expression     { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::And, $3); }
           | Expression XOR Expression     { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::Xor, $3); }
           | Expression IS_EQ Expression   { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::Eq, $3); }
           | Expression NOT_EQ Expression  { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::Neq, $3); }
           | Expression GREATER Expression { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::Greater, $3); }
           | Expression LESS Expression    { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::Less, $3); }
           | Expression LS_EQ Expression   { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::LeEq, $3); }
           | Expression GR_EQ Expression   { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::GrEq, $3); }
           | Expression ADD Expression     { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::Add, $3); }
           | Expression SUB Expression     { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::Sub, $3); }
           | Expression MUL Expression     { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::Mul, $3); }
           | Expression DIV Expression     { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::Div, $3); }
           | Expression MOD Expression     { $$ = std::make_shared<langI::BinOpNode>($1, langI::BinOp::Mod, $3); }
           | ExpressionTerm                { $$ = $1; }

ExpressionTerm : LP Expression RP { $$ = $2; }
               | Primary { $$ = $1; }

Primary : INT         { $$ = std::make_shared<langI::IntNode>($1); }
        | ModPrimary  { $$ = $1; }
        | SCAN        { $$ = std::make_shared<langI::ScanNode>(); }
        | RoutineCall { $$ = $1; }

ModPrimary : NAME { $$ = std::dynamic_pointer_cast<langI::IStoreable>(driver->m_curScope->findName($1)); }
           | NAME LBT Expression RBT {}


%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval, parser::location_type *yylloc, Driver* driver)
{
  return driver->yylex(yylval);
}

void parser::error(const parser::location_type &loc, const std::string& msg)
{
  std::cout << loc << ':' << msg << std::endl;
}
}
