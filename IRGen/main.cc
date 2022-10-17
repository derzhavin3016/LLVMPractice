#include "gen.hh"

int main()
{
  try
  {
    irgen::Generator gen{};
    gen.generateIR();
    gen.dump(std::cout);
  }
  catch (const std::exception &ex)
  {
    std::cout << "Caught an exception in main().\nReason: " << ex.what()
              << std::endl;
    return 1;
  }
}
