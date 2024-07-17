#include "layer_arch/arch_manager.hpp"
#include "layer_arch/api/yosys/io/read_verilog.hpp"

int main( int argc, char** argv )
{
  std::string file = std::string( argv[1] );
  lfAmINST->start();

  lf::arch::yosys::read_verilog( file );

  return 1;
}