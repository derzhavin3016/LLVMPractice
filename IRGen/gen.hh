#ifndef __IRGEN_GEN_HH__
#define __IRGEN_GEN_HH__

#include <iostream>
#include <memory>
#include <sstream>
#include <string_view>

#include "llvmbase.hh"

namespace irgen
{

class Generator final
{
private:
  llvm::LLVMContext &context;
  llvm::Module *pModule;
  llvm::IRBuilder<> builder;

public:
  Generator(llvm::LLVMContext &cont, llvm::Module *mod)
    : context(cont),
      pModule(mod),
      builder(context)
  {}

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

  void generateIR();

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
