#include "gen.hh"

int main()
{
  irgen::Generator gen{};
  // @pActiveField = dso_local local_unnamed_addr global i8* null, align 8
  gen.makeGlobalPtr("pActiveField");
  // @pNextField = dso_local local_unnamed_addr global i8* null, align 8
  gen.makeGlobalPtr("pNextField");

  gen.dump(std::cout);
}
