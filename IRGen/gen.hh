#ifndef __IRGEN_GEN_HH__
#define __IRGEN_GEN_HH__

#include <iostream>
#include <memory>
#include <sstream>
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

  auto makeFuncDecl(const std::vector<llvm::Type *> &params, llvm::Type *retTy,
                    std::string_view name)
  {
    llvm::ArrayRef<llvm::Type *> argsRef(params);
    auto *fType = llvm::FunctionType::get(retTy, argsRef, false);

    return llvm::Function::Create(fType,
                                  llvm::Function::LinkageTypes::ExternalLinkage,
                                  name, pModule.get());
  }

  auto makeVoidFuncDecl(llvm::Type *retTy, std::string_view name)
  {
    auto *fType = llvm::FunctionType::get(retTy, false);

    return llvm::Function::Create(fType,
                                  llvm::Function::LinkageTypes::ExternalLinkage,
                                  name, pModule.get());
  }

  void makeGetCell()
  {
    auto *func = makeFuncDecl({builder.getInt64Ty(), builder.getInt64Ty()},
                              builder.getInt1Ty(), "getCell");

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
    auto *i7 =
      builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pActiveField"));
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

  void makeGenRandomBool()
  {
    makeVoidFuncDecl(builder.getInt1Ty(), "genRandomBool");
  }

  void makeFillField()
  {
    auto *func = makeVoidFuncDecl(builder.getVoidTy(), "fillField");

    auto *b0 = llvm::BasicBlock::Create(context, "", func);
    auto *b1 = llvm::BasicBlock::Create(context, "", func);
    builder.SetInsertPoint(b0);

    // br label %1
    builder.CreateBr(b1);

    builder.SetInsertPoint(b1);
    // %2 = phi i64 [ 0, %0 ], [ %6, %5 ]
    auto *i2 = builder.CreatePHI(builder.getInt64Ty(), 2);
    i2->addIncoming(builder.getInt64(0), b0);
    // %3 = mul nuw nsw i64 %2, 400
    auto *i3 = builder.CreateMul(i2, builder.getInt64(400), "", true, true);
    /**/
    auto *b4 = llvm::BasicBlock::Create(context, "", func);
    builder.SetInsertPoint(b4);
    // ret void
    builder.CreateRetVoid();

    auto *b5 = llvm::BasicBlock::Create(context, "", func);
    builder.SetInsertPoint(b5);
    // %6 = add nuw nsw i64 %2, 1
    auto *i6 = builder.CreateAdd(i2, builder.getInt64(1), "", true, true);
    i2->addIncoming(i6, b5);
    // %7 = icmp eq i64 %6, 400
    auto *i7 = builder.CreateICmpEQ(i6, builder.getInt64(400));
    // br i1 %7, label %4, label %1, !llvm.loop !12
    builder.CreateCondBr(i7, b4, b1);

    auto *b8 = llvm::BasicBlock::Create(context, "", func);
    builder.SetInsertPoint(b1);
    // br label %8 -> From b1
    builder.CreateBr(b8);
    /**/
    builder.SetInsertPoint(b8);

    // %9 = phi i64 [ 0, %1 ], [ %15, %8 ]
    auto *i9 = builder.CreatePHI(builder.getInt64Ty(), 2);
    i9->addIncoming(builder.getInt64(0), b1);
    // %10 = tail call zeroext i1 @genRandomBool() #7
    auto *i10 = builder.CreateCall(getFunc("genRandomBool"));
    // %11 = load i8*, i8** @pActiveField, align 8, !tbaa !5
    auto *i11 =
      builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pActiveField"));
    // %12 = add nuw nsw i64 %3, %9
    auto *i12 = builder.CreateAdd(i3, i9, "", true, true);
    // %13 = getelementptr inbounds i8, i8* %11, i64 %12
    auto *i13 = builder.CreateGEP(builder.getInt8Ty(), i11, i12);
    // %14 = zext i1 %10 to i8
    auto *i14 = builder.CreateZExt(i10, builder.getInt8Ty());
    // store i8 %14, i8* %13, align 1, !tbaa !9
    builder.CreateStore(i14, i13);
    // %15 = add nuw nsw i64 %9, 1
    auto *i15 = builder.CreateAdd(i9, builder.getInt64(1), "", true, true);
    i9->addIncoming(i15, b8);
    // %16 = icmp eq i64 %15, 400
    auto *i16 = builder.CreateICmpEQ(i15, builder.getInt64(400));
    // br i1 %16, label %5, label %8
    builder.CreateCondBr(i16, b5, b8);
  }

  void dump(std::ostream &ost) const
  {
    std::string buffer;
    llvm::raw_string_ostream os(buffer);

    pModule->print(os, nullptr);
    os.flush();

    ost << buffer;
  }

private:
  llvm::GlobalVariable *getGlobVar(std::string_view name)
  {
    auto *var = pModule->getGlobalVariable(name);
    if (var == nullptr)
    {
      std::ostringstream ss;
      ss << "Global variable with name " << name << " was not declared\n";
      throw std::logic_error(ss.str());
    }
    return var;
  }

  llvm::Function *getFunc(std::string_view name)
  {
    auto *func = pModule->getFunction(name);
    if (func == nullptr)
    {
      std::ostringstream ss;
      ss << "Function with name " << name << " was not declared\n";
      throw std::logic_error(ss.str());
    }
    return func;
  }
};
} // namespace irgen

#endif /* __IRGEN_GEN_HH__ */
