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

llvm::Value *ScanNode::codegen(CodegenCtx &ctx)
{
  auto scanFunc = ctx.pModule->getFunction(kScanFnName);
  if (scanFunc == nullptr)
    throw std::runtime_error{"Cannot find scan function"};

  return ctx.builder.CreateCall(scanFunc);
}

llvm::Value *ParamDeclNode::codegen(CodegenCtx &ctx)
{
  if (getAlloca() != nullptr)
    return ctx.builder.CreateLoad(getTy(), getAlloca());

  setAlloca(ctx.builder.CreateAlloca(getTy()));
  ctx.builder.CreateStore(m_arg, getAlloca());

  return nullptr;
}

void FuncDeclNode::makeFuncSig(CodegenCtx &ctx)
{
  if (m_func)
    return;

  std::vector<llvm::Type *> argTypes;
  argTypes.reserve(m_params.size());

  if (m_ret == nullptr)
    m_ret = ctx.builder.getVoidTy();

  for (auto &&param : m_params)
    argTypes.push_back(param->getTy());

  auto *funcTy = argTypes.empty()
                   ? llvm::FunctionType::get(m_ret, false)
                   : llvm::FunctionType::get(m_ret, argTypes, false);

  m_func = llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage,
                                  getName(), *ctx.pModule);

  // Add all allocas to scope
  for (std::size_t i = 0; i < m_params.size(); ++i)
    m_params[i]->setArg(m_func->getArg(static_cast<unsigned>(i)));
}

llvm::Value *FuncDeclNode::codegen(CodegenCtx &ctx)
{
  makeFuncSig(ctx);

  if (m_onlyDecl)
    return nullptr;

  auto initBB = llvm::BasicBlock::Create(ctx.context, "", m_func);
  ctx.builder.SetInsertPoint(initBB);

  for (auto &&param : m_params)
    param->codegen(ctx);

  m_body->codegen(ctx);
  ctx.builder.ClearInsertionPoint();
  return nullptr;
}

llvm::Value *FuncCallNode::codegen(CodegenCtx &ctx)
{
  auto fdecl = std::dynamic_pointer_cast<FuncDeclNode>(m_funcDecl.lock());

  std::vector<llvm::Value *> vals;
  vals.reserve(m_args.size());

  for (auto &&arg : m_args)
    vals.push_back(arg->codegen(ctx));

  auto *func = fdecl->getFunc();

  return ctx.builder.CreateCall(func, vals);
}

pDNode ScopeNode::findName(const std::string &name) const
{
  auto it = m_symtab.find(name);
  if (it != m_symtab.end())
    return it->second;

  auto parFnc = m_parentFunc.lock();
  if (parFnc && name == parFnc->getName())
    return parFnc;
  parFnc.reset();

  auto pParent = m_parent.lock();

  if (pParent)
    return pParent->findName(name);

  std::ostringstream ss;
  ss << "Can't find symbol with name '" << name << '\'';
  throw std::runtime_error{ss.str()};
}

llvm::Value *IfNode::codegen(CodegenCtx &ctx)
{
  auto cond = m_cond->codegen(ctx);
  auto cond_bool = ctx.builder.CreateBitCast(cond, ctx.builder.getInt1Ty());

  auto parFnc = m_parScope.lock()->getFunc();

  auto bbTrue =
    llvm::BasicBlock::Create(ctx.context, "true", parFnc->getFunc());
  auto bbFalse =
    llvm::BasicBlock::Create(ctx.context, "false", parFnc->getFunc());
  auto bbNext =
    llvm::BasicBlock::Create(ctx.context, "merge", parFnc->getFunc());
  ctx.builder.CreateCondBr(cond_bool, bbTrue, bbFalse);

  ctx.builder.SetInsertPoint(bbTrue);
  m_tScope->codegen(ctx);
  if (bbTrue->getTerminator() == nullptr)
    ctx.builder.CreateBr(bbNext);

  ctx.builder.SetInsertPoint(bbFalse);

  if (m_fScope != nullptr)
    m_fScope->codegen(ctx);

  if (bbFalse->getTerminator() == nullptr)
    ctx.builder.CreateBr(bbNext);
  ctx.builder.SetInsertPoint(bbNext);

  return nullptr;
}

llvm::Value *WhileNode::codegen(CodegenCtx &ctx)
{
  auto parFnc = m_parScope.lock()->getFunc();

  auto bbCond =
    llvm::BasicBlock::Create(ctx.context, "cond", parFnc->getFunc());
  auto bbBody =
    llvm::BasicBlock::Create(ctx.context, "body", parFnc->getFunc());
  auto bbNext =
    llvm::BasicBlock::Create(ctx.context, "next", parFnc->getFunc());

  ctx.builder.CreateBr(bbCond);
  ctx.builder.SetInsertPoint(bbCond);

  auto cond = m_cond->codegen(ctx);
  auto cond_bool = ctx.builder.CreateBitCast(cond, ctx.builder.getInt1Ty());
  ctx.builder.CreateCondBr(cond_bool, bbBody, bbNext);

  ctx.builder.SetInsertPoint(bbBody);
  m_body->codegen(ctx);

  if (bbBody->getTerminator() == nullptr)
    ctx.builder.CreateBr(bbCond);

  ctx.builder.SetInsertPoint(bbNext);

  return nullptr;
}
} // namespace langI
