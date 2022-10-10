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
    std::vector<llvm::Type *> argsVec = {builder.getInt64Ty(),
                                         builder.getInt64Ty()};

    llvm::ArrayRef<llvm::Type *> argsRef(argsVec);
    auto *fType = llvm::FunctionType::get(builder.getInt1Ty(), argsRef, false);

    auto *func = llvm::Function::Create(
      fType, llvm::Function::LinkageTypes::ExternalLinkage, "getCell",
      pModule.get());

    auto *b2 = llvm::BasicBlock::Create(context, "", func);
    builder.SetInsertPoint(b2);

    // %3 = add i64 %0, 400
    auto *i3 = builder.CreateAdd(func->getArg(0), builder.getInt64(400));
    // %4 = urem i64 %3, 400
    auto *i4 = builder.CreateURem(i3, builder.getInt64(400));
    // %5 = add i64 %1, 400
    auto *i5 = builder.CreateAdd(func->getArg(1), builder.getInt64(400));
    // %6 = urem i64 %5, 400
    auto *i6 = builder.CreateURem(i5, builder.getInt64(400));
    // %7 = load i8*, i8** @pActiveField, align 8, !tbaa !5
    auto *pActiveField = pModule->getGlobalVariable("pActiveField");
    if (pActiveField == nullptr)
      throw std::logic_error("Globals were not initialized");
    auto *i7 = builder.CreateLoad(builder.getInt8PtrTy(), pActiveField);
    // %8 = mul nuw nsw i64 %6, 400
    auto *i8 = builder.CreateMul(i6, builder.getInt64(400), "", true, true);
    // %9 = add nuw nsw i64 %8, %4
    auto *i9 = builder.CreateAdd(i8, i4, "", true, true);
    // %10 = getelementptr inbounds i8, i8* %7, i64 %9
    auto *i10 = builder.CreateGEP(builder.getInt8Ty(), i7, i9);
    // %11 = load i8, i8* %10, align 1, !tbaa !9, !range !11
    auto *i11 = builder.CreateLoad(builder.getInt8Ty(), i10);
    // %12 = icmp ne i8 %11, 0
    auto *i12 = builder.CreateICmpNE(i11, builder.getInt8(0));
    // ret i1 %12
    builder.CreateRet(i12);
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
