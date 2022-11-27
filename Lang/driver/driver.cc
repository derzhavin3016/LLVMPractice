#include "driver.hh"

namespace yy
{

parser::token_type Driver::yylex(parser::semantic_type *yylval) const
{
  auto token = static_cast<parser::token_type>(m_lexer->yylex());

  switch (token)
  {
  case parser::token_type::INT:
    yylval->as<int>() = std::atoi(m_lexer->YYText());
    break;

  case parser::token_type::NAME:
    yylval->emplace<std::string>(m_lexer->YYText());
    break;

  default:
    break;
  }

  return token;
}

void Driver::dumpIR(std::ostream &out) const
{
  std::string buf;
  llvm::raw_string_ostream os(buf);
  m_ctx.pModule->print(os, nullptr);
  os.flush();
  out << buf;
}

} // namespace yy
