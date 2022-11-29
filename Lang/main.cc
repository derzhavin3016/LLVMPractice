#include <iostream>

#include "driver.hh"

int main()
{
  yy::Driver driver(std::cin, std::cout);

  if (!driver.parse())
    std::cout << "Parsing failed" << std::endl;

  driver.codegen();
  driver.dumpIR(std::cout);

  return 0;
}
