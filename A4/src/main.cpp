#include <iostream>
#include "scene_lua.hpp"
#include <time.h>
#include <stdlib.h> 
#include <string.h>

int main(int argc, char** argv)
{

  srand(time(NULL));

  std::string filename = "nonhiersphere.lua";
  bool enableSuperSample = false;
  if (argc >= 2) {
    filename = argv[1];
  }
  if (argc >= 3) {
    if(argv[2] == std::string("-ss")){
      enableSuperSample = argv[2];  
    }
  }

  if (!run_lua(filename, enableSuperSample)) {
    std::cerr << "Could not open " << filename << std::endl;
    return 1;
  }

}

