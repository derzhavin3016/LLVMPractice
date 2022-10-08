#include <iostream>
#include <memory>

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

int main()
{
  llvm::LLVMContext context{};

  // ; ModuleID = 'top'
  // source_filename = "top"
  auto llvmModule = std::make_unique<llvm::Module>("top", context);

  llvm::IRBuilder builder(context);

  // declare void @main()
  auto *funcType = llvm::FunctionType::get(builder.getInt32Ty(), false);
  auto *mainFunc = llvm::Function::Create(
    funcType, llvm::Function::ExternalLinkage, "main", llvmModule.get());

  // entrypoint:
  auto *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
  builder.SetInsertPoint(entry);

  // @pActiveField = dso_local local_unnamed_addr global i8* null, align 8
  auto *nullVal = llvm::ConstantPointerNull::get(builder.getInt8PtrTy());
  auto *pActiveField = new llvm::GlobalVariable(
    *llvmModule, builder.getInt8PtrTy(), false,
    llvm::GlobalVariable::CommonLinkage, nullVal, "pActiveField");

  pActiveField->setAlignment(llvm::MaybeAlign(8));

  // @pNextField = dso_local local_unnamed_addr global i8* null, align 8
  auto *pNextField = new llvm::GlobalVariable(
    *llvmModule, builder.getInt8PtrTy(), false,
    llvm::GlobalVariable::CommonLinkage, nullVal, "pNextField");

  pNextField->setAlignment(llvm::MaybeAlign(8));

  std::size_t fieldSize = 160000;
  auto *field1 =
    builder.CreateAlloca(llvm::ArrayType::get(builder.getInt8Ty(), fieldSize));
  auto *field2 =
    builder.CreateAlloca(llvm::ArrayType::get(builder.getInt8Ty(), fieldSize));

  // declare i32 @puts(i8*)
  // std::vector<llvm::Type *> putsArgs;
  // putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
  // llvm::ArrayRef<llvm::Type *> argsRef(putsArgs);
  // auto *putsType =
  //   llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
  // auto putsFunc = llvmModule->getOrInsertFunction("puts", putsType);

  // %0 = call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @0,
  // i32 0, i32 0))
  // builder.CreateCall(putsFunc, helloWorld);
  // ret i32 0
  builder.CreateRet(llvm::ConstantInt::get(builder.getInt32Ty(), 0));

  // Dump LLVM IR
  std::string buffer;
  llvm::raw_string_ostream os(buffer);

  llvmModule->print(os, nullptr);
  os.flush();

  std::cout << buffer;
}
