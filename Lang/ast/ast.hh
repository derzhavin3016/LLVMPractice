#ifndef __LANG_AST_AST_HH__
#define __LANG_AST_AST_HH__

#include "IRGen/llvmbase.hh"
#include "astdef.hh"
#include <algorithm>
#include <cassert>
#include <memory>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "stddefs.hh"

namespace langI
{

constexpr std::string_view kPrintFnName = "__I_print";
constexpr std::string_view kScanFnName = "__I_scan";

struct CodegenCtx final
{
  llvm::LLVMContext context;
  std::unique_ptr<llvm::Module> pModule;
  llvm::IRBuilder<> builder;

  CodegenCtx()
    : context(),
      pModule(std::make_unique<llvm::Module>("top", context)),
      builder(context)
  {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
  }

  auto getIntTy()
  {
    return builder.getInt64Ty();
  }

  auto getInt(IntT val)
  {
    return builder.getInt64(static_cast<std::uint64_t>(val));
  }

  static auto getArrTy(llvm::Type *tp, std::uint64_t size)
  {
    return llvm::ArrayType::get(tp, size);
  }
};

enum class BinOp
{
  Unknown,
  /* arithmetic */
  Add,
  Sub,
  Mul,
  Div,
  Mod,

  /* logic */
  And,
  Or,
  Xor,
  Eq,
  Neq,
  Greater,
  Less,
  GrEq,
  LeEq,
};

struct INode
{
  virtual llvm::Value *codegen(CodegenCtx &) = 0;
  virtual ~INode() = default;
};

class DeclNode : public INode
{
  std::string m_name;

public:
  DeclNode(const std::string &name) : m_name(name)
  {}

  const std::string &getName() const
  {
    return m_name;
  }

  virtual llvm::Value *codegen(CodegenCtx &) override = 0;
};

class IntNode final : public INode
{
  const IntT m_val;

public:
  IntNode(IntT val) : m_val(val)
  {}

  auto getVal() const
  {
    return m_val;
  }

  llvm::Value *codegen(CodegenCtx &ctx) override
  {
    return llvm::ConstantInt::get(ctx.getIntTy(),
                                  static_cast<std::uint64_t>(m_val), true);
  }
};

class BinOpNode : public INode
{
  pINode m_left{}, m_right{};
  BinOp m_op{BinOp::Unknown};

public:
  BinOpNode(pINode left, BinOp op, pINode right)
    : m_left(left), m_right(right), m_op(op)
  {}

  llvm::Value *codegen(CodegenCtx &ctx) override;
};

class PrintNode : public INode
{
  pINode m_expr;

public:
  PrintNode(pINode expr) : m_expr(expr)
  {}

  llvm::Value *codegen(CodegenCtx &ctx) override;
};

class ScanNode : public INode
{
public:
  llvm::Value *codegen(CodegenCtx &ctx) override;
};

class ScopeNode : public INode
{
public:
  using SymTab = std::unordered_map<std::string, pDNode>;

private:
  std::vector<pINode> m_children{};
  pwSNode m_parent{};
  SymTab m_symtab{};
  std::vector<pSNode> m_childScopes{};
  std::weak_ptr<FuncDeclNode> m_parentFunc{};

public:
  ScopeNode() = default;
  ScopeNode(pSNode par) : m_parent(par), m_parentFunc(par->m_parentFunc)
  {}

  void setFunc(std::shared_ptr<FuncDeclNode> pFunc)
  {
    m_parentFunc = pFunc;
  }

  auto getFunc() const
  {
    return m_parentFunc.lock();
  }

  auto getParent() const
  {
    return m_parent.lock();
  }

  void pushNode(pINode child)
  {
    m_children.push_back(child);
  }

  void pushScope(pSNode child)
  {
    m_childScopes.push_back(child);
  }

  void addDecl(pDNode decl, bool push = false)
  {
    assert(decl != nullptr);
    m_symtab[decl->getName()] = decl;
    if (push)
      pushNode(decl);
  }

  pDNode findName(const std::string &name) const;

  llvm::Value *codegen(CodegenCtx &ctx) override
  {
    for (auto &&child : m_children)
      if (auto bb = ctx.builder.GetInsertBlock();
          bb == nullptr || bb->getTerminator() == nullptr)
        child->codegen(ctx);

    return nullptr;
  }
};

struct IStoreable : public DeclNode
{
  IStoreable(const std::string &name) : DeclNode(name)
  {}
  virtual void store(CodegenCtx &ctx, llvm::Value *val) = 0;
};

class AsNode : public INode
{
  pIStoreable m_modifiable;
  pINode m_rhs;

public:
  AsNode(pIStoreable lhs, pINode rhs) : m_modifiable(lhs), m_rhs(rhs)
  {}

  llvm::Value *codegen(CodegenCtx &ctx) override
  {
    auto rhs = m_rhs->codegen(ctx);

    m_modifiable->store(ctx, rhs);
    return nullptr;
  }
};

class VarDeclNode : public IStoreable
{
  llvm::Value *m_alloca = nullptr;
  llvm::Type *m_type = nullptr;
  pINode m_initExpr = nullptr;

public:
  VarDeclNode(llvm::Type *type, const std::string &name,
              pINode initExpr = nullptr)
    : IStoreable(name), m_type(type), m_initExpr(initExpr)
  {}
  VarDeclNode(const VarDeclNode &) = default;
  VarDeclNode &operator=(const VarDeclNode &) = default;

  void store(CodegenCtx &ctx, llvm::Value *val) override
  {
    if (m_alloca == nullptr)
      throw std::runtime_error{"Trying to assign value to undeclared variable"};

    if (!m_type->isIntegerTy())
      throw std::runtime_error{"Trying to assign value to non-scalar variable"};

    ctx.builder.CreateStore(val, m_alloca);
  }

  auto *getTy() const
  {
    return m_type;
  }

  auto getInitExpr() const
  {
    return m_initExpr;
  }

  llvm::Value *codegen(CodegenCtx &ctx) override
  {
    auto &bld = ctx.builder;
    if (m_alloca != nullptr)
      return bld.CreateLoad(m_type, m_alloca);

    m_alloca = bld.CreateAlloca(m_type);
    if (m_type->isIntegerTy())
    {
      if (m_initExpr)
        bld.CreateStore(m_initExpr->codegen(ctx), m_alloca);

      return nullptr;
    }

    if (m_type->isArrayTy())
      return nullptr;

    throw std::runtime_error("Unknown type");
  }

  void setAlloca(llvm::Value *alloca)
  {
    m_alloca = alloca;
  }

  auto getAlloca() const
  {
    return m_alloca;
  }
};

class ArrAccNode : public IStoreable
{
  std::weak_ptr<VarDeclNode> m_decl{};
  pINode m_idx{};

public:
  ArrAccNode(const std::string &name, std::shared_ptr<VarDeclNode> pvar,
             pINode idx)
    : IStoreable(name), m_decl(pvar), m_idx(idx)
  {}
  auto *makeGep(CodegenCtx &ctx)
  {
    auto *idx = m_idx->codegen(ctx);
    auto decl = m_decl.lock();
    auto *ty = decl->getTy();

    if (!ty->isArrayTy())
      throw std::runtime_error{"Trying to index a non-array symbol"};

    return ctx.builder.CreateInBoundsGEP(ty, decl->getAlloca(),
                                         {ctx.builder.getInt64(0), idx});
  }

  llvm::Value *codegen(CodegenCtx &ctx) override
  {
    return ctx.builder.CreateLoad(m_decl.lock()->getTy()->getArrayElementType(),
                                  makeGep(ctx));
  }
  void store(CodegenCtx &ctx, llvm::Value *val) override
  {
    ctx.builder.CreateStore(val, makeGep(ctx));
  }
};

class GlobDeclNode : public VarDeclNode
{
public:
  GlobDeclNode(std::shared_ptr<VarDeclNode> pvar) : VarDeclNode(*pvar)
  {}

  llvm::Value *codegen(CodegenCtx &ctx) override
  {
    if (getAlloca() != nullptr)
      return ctx.builder.CreateLoad(getTy(), getAlloca());

    ctx.pModule->getOrInsertGlobal(getName(), getTy());
    auto *ptr = ctx.pModule->getNamedGlobal(getName());
    ptr->setLinkage(llvm::GlobalVariable::ExternalLinkage);

    if (getTy()->isArrayTy())
      ptr->setInitializer(llvm::ConstantAggregateZero::get(getTy()));
    else if (getTy()->isIntegerTy())
      ptr->setInitializer(llvm::ConstantInt::get(ctx.getIntTy(), 0));
    else
      throw std::runtime_error("Uknown type");

    ptr->setConstant(false);
    setAlloca(ptr);

    return ptr;
  }
};

class ParamDeclNode : public VarDeclNode
{
  llvm::Argument *m_arg = nullptr;

public:
  ParamDeclNode(llvm::Type *type, const std::string &name)
    : VarDeclNode(type, name)
  {}

  ParamDeclNode(const ParamDeclNode &) = default;
  ParamDeclNode &operator=(const ParamDeclNode &) = default;

  void setArg(llvm::Argument *arg)
  {
    m_arg = arg;
  }

  llvm::Value *codegen(CodegenCtx &ctx) override;
};

using pParamDNode = std::shared_ptr<ParamDeclNode>;

class FuncDeclNode : public DeclNode
{
  llvm::Type *m_ret = nullptr;
  std::vector<pParamDNode> m_params;
  llvm::Function *m_func = nullptr;
  pSNode m_body{};
  bool m_onlyDecl = false;

public:
  FuncDeclNode(const std::string &name, const std::vector<pParamDNode> &params,
               pSNode body, llvm::Type *ret = nullptr)
    : DeclNode(name), m_ret(ret), m_params(params), m_body(body)
  {
    for (auto &&param : m_params)
      m_body->addDecl(param);
  }

  FuncDeclNode(const FuncDeclNode &) = default;
  FuncDeclNode &operator=(const FuncDeclNode &) = default;

  auto *getFunc() const
  {
    return m_func;
  }

  llvm::Value *codegen(CodegenCtx &ctx) override;

  void markAsDecl()
  {
    m_onlyDecl = true;
  }

private:
  void makeFuncSig(CodegenCtx &ctx);
};

class FuncCallNode : public INode
{
  std::weak_ptr<INode> m_funcDecl{};
  std::vector<pINode> m_args{};

public:
  FuncCallNode(pINode fdecl, const std::vector<pINode> &args = {})
    : m_funcDecl(fdecl), m_args(args)
  {}
  llvm::Value *codegen(CodegenCtx &ctx) override;
};

class IfNode : public INode
{
  pSNode m_tScope, m_fScope;
  pwSNode m_parScope;
  pINode m_cond;

public:
  IfNode(pSNode tScope, pINode cond, pSNode par, pSNode fScope = nullptr)
    : m_tScope(tScope), m_fScope(fScope), m_parScope(par), m_cond(cond)
  {}
  llvm::Value *codegen(CodegenCtx &ctx) override;
};

class WhileNode : public INode
{
  pSNode m_body;
  pwSNode m_parScope;
  pINode m_cond;

public:
  WhileNode(pINode cond, pSNode body, pSNode par)
    : m_body(body), m_parScope(par), m_cond(cond)
  {}

  llvm::Value *codegen(CodegenCtx &ctx) override;
};

class RetNode : public INode
{
  pINode m_expr;

public:
  RetNode(pINode expr = nullptr) : m_expr(expr)
  {}

  llvm::Value *codegen(CodegenCtx &ctx) override
  {
    auto *expr = m_expr ? m_expr->codegen(ctx) : nullptr;

    return ctx.builder.CreateRet(expr);
  }
};

} // namespace langI

#endif /* __LANG_AST_AST_HH__ */
