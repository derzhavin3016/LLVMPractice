#include "ast.hh"

namespace langI
{
llvm::Value *BinOpNode::codegen(CodegenCtx &ctx)
{
  auto *lhs = m_left->codegen(ctx);
  auto *rhs = m_right->codegen(ctx);
  auto &bld = ctx.builder;

  switch (m_op)
  {
  /* Arithmetic */
  case BinOp::Add:
    return bld.CreateAdd(lhs, rhs);
  case BinOp::Sub:
    return bld.CreateSub(lhs, rhs);
  case BinOp::Mul:
    return bld.CreateMul(lhs, rhs);
  case BinOp::Div:
    return bld.CreateSDiv(lhs, rhs);
  case BinOp::Mod:
    return bld.CreateSRem(lhs, rhs);

  /* Logic */
  case BinOp::And:
    return bld.CreateAnd(lhs, rhs);
  case BinOp::Or:
    return bld.CreateOr(lhs, rhs);
  case BinOp::Xor:
    return bld.CreateXor(lhs, rhs);
  case BinOp::Eq:
    return bld.CreateICmpEQ(lhs, rhs);
  case BinOp::Neq:
    return bld.CreateICmpNE(lhs, rhs);
  case BinOp::Greater:
    return bld.CreateICmpSGT(lhs, rhs);
  case BinOp::Less:
    return bld.CreateICmpSLT(lhs, rhs);
  case BinOp::GrEq:
    return bld.CreateICmpSGE(lhs, rhs);
  case BinOp::LeEq:
    return bld.CreateICmpSLE(lhs, rhs);
  default:
    break;
  }

  throw std::runtime_error("Unrecognized binop type");
}

llvm::Value *PrintNode::codegen(CodegenCtx &ctx)
{
  auto *val = m_expr->codegen(ctx);

  auto printFnc = ctx.pModule->getFunction(kPrintFnName);
  if (printFnc == nullptr)
    throw std::runtime_error{"Cannot find print function"};

  return ctx.builder.CreateCall(printFnc, {val});
}

pDNode ScopeNode::findName(const std::string &name) const
{
  auto it = m_symtab.find(name);
  if (it != m_symtab.end())
    return it->second;
  auto pParent = m_parent.lock();

  if (pParent)
    return pParent->findName(name);

  std::ostringstream ss;
  ss << "Can't find symbol with name '" << name << '\'';
  throw std::runtime_error{ss.str()};
}

llvm::Value *IfNode::codegen(CodegenCtx &ctx)
{}
} // namespace langI
