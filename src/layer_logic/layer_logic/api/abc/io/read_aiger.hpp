#pragma once

#include "layer_logic/logic_manager.hpp"

#include "misc/util/abc_namespaces.h"
#include "misc/util/abc_global.h"
#include "base/io/io.h"
#include "misc/extra/extra.h"

#include "utility/string.hpp"

#include <cstdio>
#include <string>

namespace lf
{

namespace logic
{

namespace abc
{

/**
 * @brief Reads a genlib file into the current logic network.
 * @example
 *  read_aiger [options] [filename]
 *  options: -c
 * @note
 */
void read_aiger( const std::string& file, bool is_checking = false )
{
  if ( !lf::utility::endsWith( file, ".genlib" ) )
  {
    std::cerr << "Unmatched genlib suffix type." << std::endl;
    assert( false );
    return;
  }

  lfLmINST->update_logic( E_ToolLogicType::E_LOGIC_ABC_AIG );
  auto ntk_ptr = lfLmINST->current<babc::Abc_Frame_t*>(); // the the network from shared_ptr

  int argc = 2;
  if ( is_checking )
    argc += 1;

  char** argv = ABC_ALLOC( char*, argc + 1 );

  int pos = 0;

  argv[pos++] = babc::Extra_UtilStrsav( "read" );

  if ( is_checking )
    argv[pos++] = babc::Extra_UtilStrsav( " -c " );

  argv[pos++] = babc::Extra_UtilStrsav( file.c_str() );

  babc::IoCommandReadAiger( ntk_ptr, argc, argv );
}

} // namespace abc

} // namespace logic

} // end namespace lf