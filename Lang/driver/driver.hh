#ifndef __LANG_DRIVER_DRIVER_HH__
#define __LANG_DRIVER_DRIVER_HH__

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include <iostream>
#include <memory>
#include <string>

#include "ast.hh"
#include "parser.hh"

namespace yy
{

class Driver final
{
private:
  std::unique_ptr<yyFlexLexer> m_lexer = nullptr;
  langI::CodegenCtx m_ctx{};
  langI::pSNode m_gScope{}, m_curScope{};
  std::vector<langI::pParamDNode> m_curFunParams{};
  std::vector<langI::pINode> m_curArgs{};
  std::shared_ptr<langI::FuncDeclNode> m_curFunc{};

  friend parser;

public:
  Driver(std::istream &in, std::ostream &out)
    : m_lexer(std::make_unique<yyFlexLexer>(in, out)),
      m_gScope(std::make_shared<langI::ScopeNode>()),
      m_curScope(m_gScope)
  {}

  parser::token_type yylex(parser::semantic_type *yylval) const;

  [[nodiscard]] bool parse()
  {
    parser par(this);
    return !par.parse();
  }

  void codegen()
  {
    auto printTy = llvm::FunctionType::get(m_ctx.builder.getVoidTy(),
                                           {m_ctx.getIntTy()}, false);

    llvm::Function::Create(printTy, llvm::Function::ExternalLinkage,
                           langI::kPrintFnName, *m_ctx.pModule);

    auto scanTy = llvm::FunctionType::get(m_ctx.getIntTy(), false);

    llvm::Function::Create(scanTy, llvm::Function::ExternalLinkage,
                           langI::kScanFnName, *m_ctx.pModule);

    m_gScope->codegen(m_ctx);
  }
  void dumpIR(std::ostream &out) const;

  ~Driver() = default;

private:
  void makeGlobalScopeChild()
  {
    m_curScope = std::make_shared<langI::ScopeNode>(m_gScope);
  }

  void makeCurScopeChild()
  {
    auto par = m_curScope;
    m_curScope = std::make_shared<langI::ScopeNode>(par);
    par->pushScope(m_curScope);
  }

  void resetScope()
  {
    auto par = m_curScope->getParent();
    m_curScope = par == nullptr ? m_gScope : par;
  }
};

} // namespace yy

#endif /* __LANG_DRIVER_DRIVER_HH__ */
