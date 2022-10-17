#include "gen.hh"

int main()
{
  try
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
    gen.makeSwapActiveField();

    gen.makeCreateWindow();
    gen.makeIsWindowOpen();
    gen.makeHandleEvents();
    gen.makeClearWindow();
    gen.makeFlushWindow();

    gen.makeMain();

    gen.dump(std::cout);
  }
  catch (const std::exception &ex)
  {
    std::cout << "Caught an exception in main().\nReason: " << ex.what()
              << std::endl;
    return 1;
  }
}
