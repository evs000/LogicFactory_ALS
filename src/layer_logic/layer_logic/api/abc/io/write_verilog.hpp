#pragma once

#include "layer_logic/logic_manager.hpp"

#include <cstdio>
#include <string>

#include "misc/util/abc_namespaces.h"
#include "misc/util/abc_global.h"
#include "base/io/io.h"
#include "misc/extra/extra.h"

#include "utility/string.hpp"

namespace lf
{

namespace logic
{

namespace abc
{

/**
 * @brief Reads a liberty file into the current logic network.
 * @example
 *  write_verilog [options] [filename]
 *  options: -K <k-feasible-cut> -f -a -m
 * @note
 */
void write_verilog( const std::string& file, int K = -1, bool is_fixed_format = false, bool is_only_ands = false, bool is_modules = false )
{
  if ( !lf::utility::endsWith( file, ".v" ) )
  {
    std::cerr << "Unmatched verilog suffix type." << std::endl;
    assert( false );
    return;
  }

  lfLmINST->update_logic( E_ToolLogicType::E_LOGIC_ABC_AIG );
  auto ntk_ptr = lfLmINST->current<babc::Abc_Frame_t*>(); // the the network from shared_ptr

  int argc = 2;
  if ( K > 0 )
  {
    assert( K >= 2 && K <= 4 );
    argc += 1;
  }
  if ( is_fixed_format )
    argc += 1;
  if ( is_only_ands )
    argc += 1;
  if ( is_modules )
    argc += 1;

  char** argv = ABC_ALLOC( char*, argc + 1 );

  int pos = 0;
  argv[pos++] = babc::Extra_UtilStrsav( "write_verilog" );

  if ( K > 0 )
    argv[pos++] = babc::Extra_UtilStrsav( std::string( " -K " + K ).c_str() );
  if ( is_fixed_format )
    argv[pos++] = babc::Extra_UtilStrsav( std::string( " -f " ).c_str() );
  if ( is_only_ands )
    argv[pos++] = babc::Extra_UtilStrsav( std::string( " -a " ).c_str() );
  if ( is_modules )
    argv[pos++] = babc::Extra_UtilStrsav( std::string( " -m " ).c_str() );

  argv[pos++] = babc::Extra_UtilStrsav( file.c_str() );

  babc::IoCommandWriteVerilog( ntk_ptr, argc, argv );
}

} // namespace abc

} // namespace logic

} // end namespace lf