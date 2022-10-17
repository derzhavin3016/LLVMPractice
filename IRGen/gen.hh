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
    : context(),
      pModule(std::make_unique<llvm::Module>("top", context)),
      builder(context)
  {}

  ~Generator() = default;

  Generator(const Generator &) = delete;
  Generator &operator=(const Generator &) = delete;
  Generator(Generator &&) = delete;
  Generator &operator=(Generator &&) = delete;

  void dump(std::ostream &ost) const;

  void makeGlobalPtr(std::string_view name);

  void makeGetCell();

  void makeGenRandomBool();

  void makeFillField();

  void makeGetNeighbours();

  void makeNewGeneration();

  void makePutPixel();

  void makeDrawField();

  void makeSwapActiveField();

  void makeMain();

  void makeCreateWindow();
  void makeIsWindowOpen();
  void makeHandleEvents();
  void makeClearWindow();
  void makeFlushWindow();

private:
  llvm::Function *makeFuncDecl(const std::vector<llvm::Type *> &params,
                               llvm::Type *retTy, std::string_view name);
  llvm::Function *makeVoidFuncDecl(llvm::Type *retTy, std::string_view name);
  llvm::GlobalVariable *getGlobVar(std::string_view name);

  llvm::Function *getFunc(std::string_view name);

  template <typename T>
  void setAlignment(T *inst, std::uint64_t alignment)
  {
    inst->setAlignment(llvm::Align(alignment));
  }
};
} // namespace irgen

#endif /* __IRGEN_GEN_HH__ */
