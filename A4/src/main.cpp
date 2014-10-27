#include <iostream>
#include "scene_lua.hpp"

int main(int argc, char** argv)
{
  std::string filename = "nonhiersphere.lua";
  if (argc >= 2) {
    filename = argv[1];
  }
  std::cerr << "Processing " << filename << std::endl;
  if (!run_lua(filename)) {
    std::cerr << "Could not open " << filename << std::endl;
    return 1;
  }

}

