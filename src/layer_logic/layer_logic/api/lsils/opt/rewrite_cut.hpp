#pragma once

#include "layer_logic/logic_manager.hpp"

#include "mockturtle/algorithms/cut_rewriting.hpp"
#include "mockturtle/algorithms/node_resynthesis/xag_npn.hpp"

namespace lf
{

namespace logic
{

namespace lsils
{

/**
 * @brief balance current network
 * @param ntk   network
 * @param rebalancing_fn function
 * @param ps    params
 * @param pst   stats
 * @code
 *  mockturtle::aig_network aig;
 *  // ...
 *  mockturtle::aig_network aig_balanced = sop_balance(aig);
 *
 */
// template<class Ntk, class RewritingFn = {}, class NodeCostFn = unit_cost<Ntk>>
template<class Ntk>
void rewrite_cut( LogicManager& manager )
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

  auto ntk = manager.current<Ntk>();

  mockturtle::xag_npn_resynthesis<Ntk> npn_resyn;
  mockturtle::cut_rewriting_params ps;
  ps.cut_enumeration_ps.cut_size = 4u;
  Ntk ntk_new = mockturtle::cut_rewriting<Ntk, mockturtle::xag_npn_resynthesis<Ntk>>( ntk, npn_resyn, ps );
  manager.set_current<Ntk>( ntk_new );
}

} // end namespace lsils

} // end namespace logic

} // end namespace lf
