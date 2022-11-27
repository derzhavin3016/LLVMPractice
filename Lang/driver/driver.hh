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
  langI::pSNode m_curScope{};

  friend parser;

public:
  Driver(std::istream &in, std::ostream &out)
    : m_lexer(std::make_unique<yyFlexLexer>(in, out)),
      m_curScope(std::make_shared<langI::ScopeNode>())
  {}

  parser::token_type yylex(parser::semantic_type *yylval) const;

  [[nodiscard]] bool parse()
  {
    parser par(this);
    return !par.parse();
  }

  void codegen()
  {
    m_curScope->codegen(m_ctx);
  }
  void dumpIR(std::ostream &out) const;

  ~Driver() = default;
};

} // namespace yy

#endif /* __LANG_DRIVER_DRIVER_HH__ */
