#ifndef __IRGEN_RUN_HH__
#define __IRGEN_RUN_HH__

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "llvmbase.hh"
#include "llvm/ExecutionEngine/Interpreter.h"

#include "Life/graph.h"

namespace irrun
{
class Runner final
{
private:
  llvm::Module *pModule{};
  std::unique_ptr<llvm::ExecutionEngine> pExec{};
  std::unordered_map<std::string_view, void *> func_map{};

public:
  Runner(std::unique_ptr<llvm::Module> pM);

  auto run()
  {
    return pExec->runFunction(pModule->getFunction("main"), {});
  }

private:
  template <typename T>
  void addToFuncMap(std::string_view name, T func)
  {
    func_map[name] = reinterpret_cast<void *>(func);
  }
};

Runner::Runner(std::unique_ptr<llvm::Module> pM)
  : pModule(pM.get()), pExec(llvm::EngineBuilder(std::move(pM)).create())
{
  addToFuncMap("genRandomBool", genRandomBool);
  addToFuncMap("createWindow", createWindow);
  addToFuncMap("isWindowOpen", isWindowOpen);
  addToFuncMap("handleEvents", handleEvents);
  addToFuncMap("clearWindow", clearWindow);
  addToFuncMap("putPixel", putPixel);
  addToFuncMap("flushWindow", flushWindow);

  pExec->InstallLazyFunctionCreator(
    [this](const std::string &name) { return func_map[name]; });
  pExec->finalizeObject();
}
} // namespace irrun

#endif /* __IRGEN_RUN_HH__ */
