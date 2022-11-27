#include <iostream>

#include "driver.hh"

int main()
{
  yy::Driver driver(std::cin, std::cout);
  auto res = driver.parse();

  if (res)
    std::cout << "Successful parsing" << std::endl;

  driver.codegen();
  driver.dumpIR(std::cout);

  return 0;
}
