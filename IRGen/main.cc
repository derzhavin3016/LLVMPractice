#include "gen.hh"

int main()
{
  irgen::Generator gen{};
  // @pActiveField = dso_local local_unnamed_addr global i8* null, align 8
  gen.makeGlobalPtr("pActiveField");
  // @pNextField = dso_local local_unnamed_addr global i8* null, align 8
  gen.makeGlobalPtr("pNextField");

  gen.makeGenRandomBool();

  gen.makeGetCell();

  gen.makeFillField();

  gen.makeGetNeighbours();

  gen.makeNewGeneration();

  gen.makePutPixel();

  gen.makeDrawField();

  gen.dump(std::cout);
}
