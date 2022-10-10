#ifndef __IRGEN_GEN_HH__
#define __IRGEN_GEN_HH__

#include <iostream>
#include <memory>
#include <string_view>

/******************************** LLVM headers ********************************/
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
/******************************************************************************/

namespace irgen
{

class Generator final
{
private:
  llvm::LLVMContext context{};
  std::unique_ptr<llvm::Module> pModule{};
  llvm::IRBuilder<> builder;

public:
  Generator()
    : context(), pModule(new llvm::Module("top", context)), builder(context)
  {}

  ~Generator() = default;

  Generator(const Generator &) = delete;
  Generator &operator=(const Generator &) = delete;
  Generator(Generator &&) = delete;
  Generator &operator=(Generator &&) = delete;

  void makeGlobalPtr(std::string_view name)
  {
    auto *nullVal = llvm::ConstantPointerNull::get(builder.getInt8PtrTy());
    auto *ptr = new llvm::GlobalVariable(
      *pModule, builder.getInt8PtrTy(), false,
      llvm::GlobalVariable::CommonLinkage, nullVal, name);

    ptr->setAlignment(llvm::MaybeAlign(8));
  }

  void makeGetCell()
  {
    std::vector<llvm::Type *> argsVec = {builder.getInt64Ty()->getPointerTo(),
                                         builder.getInt64Ty()->getPointerTo()};

    llvm::ArrayRef<llvm::Type *> argsRef(argsVec);
    auto *fType = llvm::FunctionType::get(builder.getInt1Ty(), argsRef, false);

    auto *func = llvm::Function::Create(
      fType, llvm::Function::LinkageTypes::ExternalLinkage, "getCell",
      pModule.get());

    auto *entry = llvm::BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(entry);
    builder.CreateRet(llvm::ConstantInt::get(builder.getInt1Ty(), 0));
  }

  void dump(std::ostream &ost) const
  {
    std::string buffer;
    llvm::raw_string_ostream os(buffer);

    pModule->print(os, nullptr);
    os.flush();

    ost << buffer;
  }
};
} // namespace irgen

#endif /* __IRGEN_GEN_HH__ */
