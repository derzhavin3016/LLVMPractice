#include <iostream>
#include <sstream>

#include "driver.hh"

int main()
{
  std::string inp = "routine main : integer is\n\tprint 2 + 2;\nend\n";
  std::istringstream is(inp);

  yy::Driver driver(is, std::cout);
  auto res = driver.parse();

  if (res)
    std::cout << "Successful parsing" << std::endl;

  driver.codegen();
  driver.dumpIR(std::cout);

  return 0;
}
