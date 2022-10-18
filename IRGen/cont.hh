#ifndef __IRGEN_CONT_HH__
#define __IRGEN_CONT_HH__

#include "gen.hh"
#include "run.hh"

namespace ircont
{
class Context final
{
private:
  llvm::LLVMContext context{};

public:
  Context() : context()
  {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
  }

  void genAndRun()
  {
    auto pModule = std::make_unique<llvm::Module>("top", context);
    irgen::Generator gen{context, pModule.get()};

    gen.generateIR();

    gen.dump(std::cout);

    irrun::Runner runner(std::move(pModule));
    std::cout << "[RUNNING]\n";
    runner.run();
    std::cout << "[WAS RUN]\n";
  }
};
} // namespace ircont

#endif /* __IRGEN_CONT_HH__ */
