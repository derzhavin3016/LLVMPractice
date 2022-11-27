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

namespace langI
{
using IntT = std::int64_t;

constexpr std::string_view kPrintFnName = "__I_print";

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

enum class Types
{

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

class ScopeNode : public INode
{
public:
  using SymTab = std::unordered_map<std::string, pDNode>;

private:
  std::vector<pINode> m_children{};
  pwSNode m_parent{};
  SymTab m_symtab{};

public:
  ScopeNode() = default;
  ScopeNode(pSNode par) : m_parent(par)
  {}

  void pushNode(pINode child)
  {
    m_children.push_back(child);
  }

  void addName(pDNode decl, bool push = false)
  {
    m_symtab[decl->getName()] = decl;
    if (push)
      pushNode(decl);
  }

  pDNode findName(const std::string &name) const;

  llvm::Value *codegen(CodegenCtx &ctx) override
  {
    for (auto &&child : m_children)
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

  llvm::Value *codegen(CodegenCtx &ctx) override
  {
    auto &bld = ctx.builder;
    if (m_alloca != nullptr)
      m_alloca = bld.CreateAlloca(m_type);
    if (m_type->isIntegerTy())
    {
      if (m_initExpr)
        bld.CreateStore(m_initExpr->codegen(ctx), m_alloca);

      return bld.CreateLoad(m_type, m_alloca);
    }

    if (m_type->isArrayTy())
      return bld.CreateInBoundsGEP(m_type, m_alloca,
                                   {ctx.getInt(0), ctx.getInt(0)});

    throw std::runtime_error("Unknown type");
  }
};

class IfNode : public INode
{
  pSNode m_tScope, m_fScope, m_parScope;
  pINode m_cond;

public:
  IfNode(pSNode tScope, pINode cond, pSNode par, pSNode fScope = nullptr)
    : m_tScope(tScope), m_fScope(fScope), m_parScope(par), m_cond(cond)
  {}
  llvm::Value *codegen(CodegenCtx &ctx) override;
};

class RetNode : public INode
{
  pINode m_expr;

public:
  RetNode(pINode expr) : m_expr(expr)
  {}

  llvm::Value *codegen(CodegenCtx &ctx) override
  {
    auto expr = m_expr->codegen(ctx);

    return ctx.builder.CreateRet(expr);
  }
};

struct RetVoidNode : public INode
{
  RetVoidNode() = default;

  llvm::Value *codegen(CodegenCtx &ctx) override
  {
    return ctx.builder.CreateRetVoid();
  }
};

} // namespace langI

#endif /* __LANG_AST_AST_HH__ */
