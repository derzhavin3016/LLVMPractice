#include "stddefs.hh"
#include <iostream>

extern "C" void __I_print(langI::IntT val)
{
  std::cout << val << std::endl;
}

extern "C" langI::IntT __I_scan()
{
  langI::IntT res;

  std::cin >> res;
  if (!std::cin)
  {
    std::cerr << "Problem while reading stdin. Exiting...\n";
    std::exit(1);
  }

  return res;
}
