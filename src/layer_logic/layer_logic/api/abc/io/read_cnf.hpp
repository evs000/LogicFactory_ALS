#pragma once

#include "layer_logic/logic_manager.hpp"
#include "layer_logic/api/abc/frame.hpp"
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
 * @brief Reads file into the current logic network.
 * @example
 *  read_cnf [options] [filename]
 *  options: -m
 * @note
 */
void read_cnf( const std::string& file, bool is_multi_output = false )
{
  if ( !lf::utility::endsWith( file, ".cnf" ) )
  {
    std::cerr << "Unmatched cnf suffix type." << std::endl;
    assert( false );
    return;
  }

  lfLmINST->update_logic( lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_ABC_NTK_LOGIC_SOP );
  auto ntk_ptr = lfLmINST->current<babc::Abc_Frame_t*>(); // the the network from shared_ptr

  int argc = 2;
  if ( is_multi_output )
    argc += 1;

  char** argv = ABC_ALLOC( char*, argc + 1 );

  int pos = 0;

  argv[pos++] = babc::Extra_UtilStrsav( "read_cnf" );

  if ( is_multi_output )
    argv[pos++] = babc::Extra_UtilStrsav( " -m " );

  argv[pos++] = babc::Extra_UtilStrsav( file.c_str() );

  babc::IoCommandReadAiger( ntk_ptr, argc, argv );
}

} // namespace abc

} // namespace logic

} // end namespace lf