#ifndef __IRGEN_RUN_HH__
#define __IRGEN_RUN_HH__

#include <memory>
#include <string_view>
#include <unordered_map>

#include "llvmbase.hh"

#include "Life/graph.h"

namespace irrun
{
class Runner final
{
private:
  std::unique_ptr<llvm::ExecutionEngine> pExec{};
  std::unordered_map<std::string_view, void *> func_map{};

public:
  Runner(std::unique_ptr<llvm::Module> pM)
    : pExec(llvm::EngineBuilder(std::move(pM)).create())
  {}
};
} // namespace irrun

#endif /* __IRGEN_RUN_HH__ */
