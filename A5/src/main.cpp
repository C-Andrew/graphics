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
  bool enableMultiThreading = false;
  if (argc >= 2) {
    filename = argv[1];
  }

  switch(argc){
    case 4: 
      if(argv[3] == std::string("-mt")){
        enableMultiThreading = true;  
      }
    case 3:
      if(argv[2] == std::string("-ss")){
        enableSuperSample = true;  
      }
    case 2:
      filename = argv[1];
    default:
      break;
  }

  if (!run_lua(filename, enableSuperSample, enableMultiThreading)) {
    std::cerr << "Could not open " << filename << std::endl;
    return 1;
  }

}

