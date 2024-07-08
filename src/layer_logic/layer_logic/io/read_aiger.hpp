#pragma once
#include "lorina/lorina.hpp"
#include "mockturtle/mockturtle.hpp"

#include "layer_logic/logic_manager.hpp"

#include <cstring>
#include <memory>

namespace lf
{

namespace logic
{

/**
 * @brief read aiger file into the network
 * @param filename
 * @param ntk
 * @return true/ false
 */
template<class Ntk = mockturtle::aig_network>
bool read_aiger( std::string const& filename, Ntk& ntk )
{
  lorina::text_diagnostics consumer;
  lorina::diagnostic_engine diag( &consumer );

  auto rc = lorina::read_aiger( filename, mockturtle::aiger_reader( ntk ), &diag );
  if ( rc != lorina::return_code::success )
  {
    std::cout << "parser wrong!" << std::endl;
    return false;
  }
  return true;
}

/**
 * @brief read aiger file, and return the network
 * @param filename
 * @return network
 */

template<class Ntk = mockturtle::aig_network>
Ntk read_aiger( std::string const& filename )
{
  Ntk ntk;
  lorina::text_diagnostics consumer;
  lorina::diagnostic_engine diag( &consumer );

  auto rc = lorina::read_aiger( filename, mockturtle::aiger_reader( ntk ), &diag );
  if ( rc != lorina::return_code::success )
  {
    std::cout << "parser wrong!" << std::endl;
    assert( false );
    return ntk();
  }
  return ntk;
}

/**
 * @brief read aiger file into the network
 * @param filename
 * @param ntk
 * @return true/ false
 */
template<class Ntk = lsils::aig_seq_network>
bool read_aiger( LogicManager& manager, std::string const& filename )
{
  using NtkBase = typename Ntk::base_type;
  if constexpr ( std::is_same_v<NtkBase, mockturtle::aig_network> )
  {
    manager.update_logic( E_ToolLogicType::E_LOGIC_MOCKTURTLE_AIG );
  }
  else if constexpr ( std::is_same_v<NtkBase, mockturtle::xag_network> )
  {
    manager.update_logic( E_ToolLogicType::E_LOGIC_MOCKTURTLE_XAG );
  }
  else if constexpr ( std::is_same_v<NtkBase, mockturtle::mig_network> )
  {
    manager.update_logic( E_ToolLogicType::E_LOGIC_MOCKTURTLE_MIG );
  }
  else if constexpr ( std::is_same_v<NtkBase, mockturtle::xmg_network> )
  {
    manager.update_logic( E_ToolLogicType::E_LOGIC_MOCKTURTLE_XMG );
  }
  else if constexpr ( std::is_same_v<NtkBase, mockturtle::gtg_network> )
  {
    manager.update_logic( E_ToolLogicType::E_LOGIC_MOCKTURTLE_GTG );
  }
  else
  {
    assert( false );
  }

  Ntk ntk;

  lorina::text_diagnostics consumer;
  lorina::diagnostic_engine diag( &consumer );

  lorina::return_code rc = lorina::read_aiger( filename, mockturtle::aiger_reader( ntk ), &diag );
  if ( rc != lorina::return_code::success )
  {
    std::cout << "parser wrong!" << std::endl;
    return false;
  }

  manager.set_current<Ntk>( ntk );
  return true;
}

} // end namespace logic

} // end namespace lf