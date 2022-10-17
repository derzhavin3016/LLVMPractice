#include "cont.hh"

int main()
{
  try
  {
    ircont::Context cont{};
    cont.genAndRun();
  }
  catch (const std::exception &ex)
  {
    std::cout << "Caught an exception in main().\nReason: " << ex.what()
              << std::endl;
    return 1;
  }
}
