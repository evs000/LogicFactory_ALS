#pragma once

#include "layer_logic/logic_manager.hpp"

namespace lf
{

namespace logic
{

namespace lsils
{

/**
 * @brief
 *
 */
void write_verilog( const std::string& file )
{
  mockturtle::write_verilog_params ports;
  ports.module_name = lfLmINST->ports().get_module_name();
  for ( auto port : lfLmINST->ports().get_inputs() )
  {
    ports.input_names.push_back( std::make_pair( port, 1u ) );
  }
  for ( auto port : lfLmINST->ports().get_outputs() )
  {
    ports.output_names.push_back( std::make_pair( port, 1u ) );
  }

  std::ofstream os( file.c_str(), std::ofstream::out );

  auto ntktype = lfLntINST->get_ntktype_curr();
  lfLmINST->update_logic( ntktype );
  if ( ntktype == lf::misc::E_LF_LOGIC_NTK_TYPE::E_LF_LOGIC_NTK_TYPE_LSILS_STRASH_AIG )
  {
    lf::logic::lsils::aig_seq_network ntk = lfLmINST->current<lf::logic::lsils::aig_seq_network>();
    mockturtle::write_verilog( ntk, os, ports );
  }
  else if ( ntktype == lf::misc::E_LF_LOGIC_NTK_TYPE::E_LF_LOGIC_NTK_TYPE_LSILS_STRASH_XAG )
  {
    lf::logic::lsils::xag_seq_network ntk = lfLmINST->current<lf::logic::lsils::xag_seq_network>();
    mockturtle::write_verilog( ntk, os, ports );
  }
  else if ( ntktype == lf::misc::E_LF_LOGIC_NTK_TYPE::E_LF_LOGIC_NTK_TYPE_LSILS_STRASH_XMG )
  {
    lf::logic::lsils::xmg_seq_network ntk = lfLmINST->current<lf::logic::lsils::xmg_seq_network>();
    mockturtle::write_verilog( ntk, os, ports );
  }
  else if ( ntktype == lf::misc::E_LF_LOGIC_NTK_TYPE::E_LF_LOGIC_NTK_TYPE_LSILS_STRASH_MIG )
  {
    lf::logic::lsils::mig_seq_network ntk = lfLmINST->current<lf::logic::lsils::mig_seq_network>();
    mockturtle::write_verilog( ntk, os, ports );
  }
  else if ( ntktype == lf::misc::E_LF_LOGIC_NTK_TYPE::E_LF_LOGIC_NTK_TYPE_LSILS_STRASH_GTG )
  {
    lf::logic::lsils::gtg_seq_network ntk = lfLmINST->current<lf::logic::lsils::gtg_seq_network>();
    mockturtle::write_verilog( ntk, os, ports );
  }
  else if ( ntktype == lf::misc::E_LF_LOGIC_NTK_TYPE::E_LF_LOGIC_NTK_TYPE_LSILS_NETLIST_FPGA )
  {
    std::cerr << "TODO ing!\n";
  }
  else if ( ntktype == lf::misc::E_LF_LOGIC_NTK_TYPE::E_LF_LOGIC_NTK_TYPE_LSILS_NETLIST_ASIC )
  {
    lf::logic::lsils::blut_seq_network ntk = lfLmINST->current<lf::logic::lsils::blut_seq_network>();
    mockturtle::write_verilog_with_binding( ntk, os, ports );
  }
  else
  {
    std::cerr << "unsupport network type!\n";
    assert( false );
  }

  os.close();
}

} // end namespace lsils

} // end namespace logic

} // end namespace lf
