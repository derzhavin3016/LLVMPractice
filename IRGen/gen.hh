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

  void makeGetNeighbours()
  {
    auto *func = makeFuncDecl({builder.getInt32Ty(), builder.getInt32Ty()},
                              builder.getInt8Ty(), "getNeighbours");
    auto *mainBB = llvm::BasicBlock::Create(context, "", func);
    builder.SetInsertPoint(mainBB);

    // %3 = zext i32 %0 to i64
    auto *i3 = builder.CreateZExt(func->getArg(0), builder.getInt64Ty());
    // %4 = zext i32 %1 to i64
    auto *i4 = builder.CreateZExt(func->getArg(1), builder.getInt64Ty());
    // %5 = add nuw nsw i64 %3, 399
    auto *i5 = builder.CreateAdd(i3, builder.getInt64(399), "", true, true);
    // %6 = urem i64 %5, 400
    auto *i6 = builder.CreateURem(i5, builder.getInt64(400));
    // %7 = add nuw nsw i64 %4, 399
    auto *i7 = builder.CreateAdd(i4, builder.getInt64(399), "", true, true);
    // %8 = urem i64 %7, 400
    auto *i8 = builder.CreateURem(i7, builder.getInt64(400));
    // %9 = load i8*, i8** @pActiveField, align 8, !tbaa !5
    auto *i9 =
      builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pActiveField"));
    // %10 = mul nuw nsw i64 %8, 400
    auto *i10 = builder.CreateMul(i8, builder.getInt64(400), "", true, true);
    // %11 = add nuw nsw i64 %10, %6
    auto *i11 = builder.CreateAdd(i10, i6, "", true, true);
    // %12 = getelementptr inbounds i8, i8* %9, i64 %11
    auto *i12 = builder.CreateGEP(builder.getInt8Ty(), i9, i11);
    // %13 = load i8, i8* %12, align 1, !tbaa !9, !range !11
    auto *i13 = builder.CreateLoad(builder.getInt8Ty(), i12);
    // %14 = add nuw nsw i64 %3, 400
    auto *i14 = builder.CreateAdd(i3, builder.getInt64(400), "", true, true);
    // %15 = urem i64 %14, 400
    auto *i15 = builder.CreateURem(i14, builder.getInt64(400));
    // %16 = add nuw nsw i64 %10, %15
    auto *i16 = builder.CreateAdd(i10, i15, "", true, true);
    // %17 = getelementptr inbounds i8, i8* %9, i64 %16
    auto *i17 = builder.CreateGEP(builder.getInt8Ty(), i9, i16);
    // %18 = load i8, i8* %17, align 1, !tbaa !9, !range !11
    auto *i18 = builder.CreateLoad(builder.getInt8Ty(), i17);
    // %19 = add nuw nsw i8 %18, %13
    auto *i19 = builder.CreateAdd(i18, i13, "", true, true);
    // %20 = add nuw nsw i64 %3, 401
    auto *i20 = builder.CreateAdd(i3, builder.getInt64(401), "", true, true);
    // %21 = urem i64 %20, 400
    auto *i21 = builder.CreateURem(i20, builder.getInt64(400));
    // %22 = add nuw nsw i64 %10, %21
    auto *i22 = builder.CreateAdd(i10, i21, "", true, true);
    // %23 = getelementptr inbounds i8, i8* %9, i64 %22
    auto *i23 = builder.CreateGEP(builder.getInt8Ty(), i9, i22);
    // %24 = load i8, i8* %23, align 1, !tbaa !9, !range !11
    auto *i24 = builder.CreateLoad(builder.getInt8Ty(), i23);
    // %25 = add nuw nsw i8 %19, %24
    auto *i25 = builder.CreateAdd(i19, i24, "", true, true);
    // %26 = add nuw nsw i64 %4, 400
    auto *i26 = builder.CreateAdd(i4, builder.getInt64(400), "", true, true);
    // %27 = urem i64 %26, 400
    auto *i27 = builder.CreateURem(i26, builder.getInt64(400));
    // %28 = mul nuw nsw i64 %27, 400
    auto *i28 = builder.CreateMul(i27, builder.getInt64(400), "", true, true);
    // %29 = add nuw nsw i64 %28, %21
    auto *i29 = builder.CreateAdd(i28, i21, "", true, true);
    // %30 = getelementptr inbounds i8, i8* %9, i64 %29
    auto *i30 = builder.CreateGEP(builder.getInt8Ty(), i9, i29);
    // %31 = load i8, i8* %30, align 1, !tbaa !9, !range !11
    auto *i31 = builder.CreateLoad(builder.getInt8Ty(), i30);
    // %32 = add nuw nsw i8 %25, %31
    auto *i32 = builder.CreateAdd(i25, i31, "", true, true);
    // %33 = add nuw nsw i64 %4, 401
    auto *i33 = builder.CreateAdd(i4, builder.getInt64(401), "", true, true);
    // %34 = urem i64 %33, 400
    auto *i34 = builder.CreateURem(i33, builder.getInt64(400));
    // %35 = mul nuw nsw i64 %34, 400
    auto *i35 = builder.CreateMul(i34, builder.getInt64(400), "", true, true);
    // %36 = add nuw nsw i64 %35, %21
    auto *i36 = builder.CreateAdd(i35, i21, "", true, true);
    // %37 = getelementptr inbounds i8, i8* %9, i64 %36
    auto *i37 = builder.CreateGEP(builder.getInt8Ty(), i9, i36);
    // %38 = load i8, i8* %37, align 1, !tbaa !9, !range !11
    auto *i38 = builder.CreateLoad(builder.getInt8Ty(), i37);
    // %39 = add nuw nsw i8 %32, %38
    auto *i39 = builder.CreateAdd(i32, i38, "", true, true);
    // %40 = add nuw nsw i64 %35, %15
    auto *i40 = builder.CreateAdd(i35, i15, "", true, true);
    // %41 = getelementptr inbounds i8, i8* %9, i64 %40
    auto *i41 = builder.CreateGEP(builder.getInt8Ty(), i9, i40);
    // %42 = load i8, i8* %41, align 1, !tbaa !9, !range !11
    auto *i42 = builder.CreateLoad(builder.getInt8Ty(), i41);
    // %43 = add nuw nsw i8 %39, %42
    auto *i43 = builder.CreateAdd(i39, i42, "", true, true);
    // %44 = add nuw nsw i64 %35, %6
    auto *i44 = builder.CreateAdd(i35, i6, "", true, true);
    // %45 = getelementptr inbounds i8, i8* %9, i64 %44
    auto *i45 = builder.CreateGEP(builder.getInt8Ty(), i9, i44);
    // %46 = load i8, i8* %45, align 1, !tbaa !9, !range !11
    auto *i46 = builder.CreateLoad(builder.getInt8Ty(), i45);
    // %47 = add nuw nsw i8 %43, %46
    auto *i47 = builder.CreateAdd(i43, i46, "", true, true);
    // %48 = add nuw nsw i64 %28, %6
    auto *i48 = builder.CreateAdd(i28, i6, "", true, true);
    // %49 = getelementptr inbounds i8, i8* %9, i64 %48
    auto *i49 = builder.CreateGEP(builder.getInt8Ty(), i9, i48);
    // %50 = load i8, i8* %49, align 1, !tbaa !9, !range !11
    auto *i50 = builder.CreateLoad(builder.getInt8Ty(), i49);
    // %51 = add nuw nsw i8 %47, %50
    auto *i51 = builder.CreateAdd(i47, i50, "", true, true);
    // ret i8 %51
    builder.CreateRet(i51);
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
  llvm::Function *makeFuncDecl(const std::vector<llvm::Type *> &params,
                               llvm::Type *retTy, std::string_view name)
  {
    llvm::ArrayRef<llvm::Type *> argsRef(params);
    auto *fType = llvm::FunctionType::get(retTy, argsRef, false);

    return llvm::Function::Create(fType,
                                  llvm::Function::LinkageTypes::ExternalLinkage,
                                  name, pModule.get());
  }

  llvm::Function *makeVoidFuncDecl(llvm::Type *retTy, std::string_view name)
  {
    auto *fType = llvm::FunctionType::get(retTy, false);

    return llvm::Function::Create(fType,
                                  llvm::Function::LinkageTypes::ExternalLinkage,
                                  name, pModule.get());
  }

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
