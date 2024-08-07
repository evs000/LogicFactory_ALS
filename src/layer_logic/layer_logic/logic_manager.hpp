#pragma once

#include "layer_logic/api/abc/frame.hpp"
#include "layer_logic/api/lsils/frame.hpp"

#include "layer_logic/aux/convert.hpp"

#include "misc/anchor.hpp"
#include "misc/ports.hpp"

#include <assert.h>
#include <iostream>

// logic manager instance
#define lfLmINST lf::logic::LogicManager::get_instance()

// logic network type instance
#define lfLntINST lf::logic::LogicNtkType::get_instance()

namespace lf
{

namespace logic
{
/**
 * @class LogicManager
 * @brief manager the logic type for the logic tools, also it namanger all the list netowrks
 *
 * @code
 *
 */
class LogicManager
{
public:
  static LogicManager* get_instance()
  {
    if ( instance_ == nullptr )
    {
      instance_ = new LogicManager;
    }
    return instance_;
  }

  void start()
  {
    babc::Abc_Start();
    frame_abc_ = babc::Abc_FrameGetGlobalFrame();
  }

  void stop()
  {
    babc::Abc_Stop();
  }

  /**
   * @brief set the current logic type, and transform the data strcuture to the current logic type
   * @param logic_type
   * @example
   *
   */
  void update_logic( lf::misc::E_LF_ANCHOR logic_type )
  {
    /// update the logic status
    lfAnchorINST->set_anchor( logic_type );

    auto anchor_prev = lfAnchorINST->get_anchor_prev();
    auto anchor_curr = lfAnchorINST->get_anchor_curr();

    if ( anchor_prev != lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_ABC_NTK_STRASH_AIG &&
         anchor_prev != lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_AIG &&
         anchor_prev != lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_XAG &&
         anchor_prev != lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_MIG &&
         anchor_prev != lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_XMG &&
         anchor_prev != lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_GTG )
    {
      return;
    }
    if ( anchor_curr == lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_ABC_NTK_NETLIST_ASIC ||
         anchor_curr == lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_ABC_NTK_NETLIST_FPGA ||
         anchor_curr == lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_NETLIST_ASIC ||
         anchor_curr == lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_NETLIST_FPGA )
    {
      return;
    }
    if ( anchor_prev == anchor_curr )
    {
      return;
    }
    // condition2: different logic type
    else
    {
      using NtkIR = lsils::gtg_seq_network; // gtech is more general, thus the conversion will not affect the original logic
      NtkIR ntk;
      babc::Abc_Ntk_t* pNtk = babc::Abc_FrameReadNtk( frame_abc_ );

      // step1:  previous logic-based data structure -> IR
      switch ( anchor_prev )
      {
      case lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_ABC_NTK_STRASH_AIG:
      {
        ntk = lf::logic::convert_abc_2_lsils<NtkIR>( pNtk );
        break;
      }
      case lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_AIG:
      {
        ntk = lf::logic::convert_lsils_internal<NtkIR, lsils::aig_seq_network>( frame_lsils_.curr_aig );
        break;
      }
      case lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_XAG:
      {
        ntk = lf::logic::convert_lsils_internal<NtkIR, lsils::xag_seq_network>( frame_lsils_.curr_xag );
        break;
      }
      case lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_MIG:
      {
        ntk = lf::logic::convert_lsils_internal<NtkIR, lsils::mig_seq_network>( frame_lsils_.curr_mig );
        break;
      }
      case lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_XMG:
      {
        ntk = lf::logic::convert_lsils_internal<NtkIR, lsils::xmg_seq_network>( frame_lsils_.curr_xmg );
        break;
      }
      case lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_GTG:
      {
        ntk = lf::logic::convert_lsils_internal<NtkIR, lsils::gtg_seq_network>( frame_lsils_.curr_gtg );
        break;
      }
      default:
      {
        assert( false );
        break;
      }
      }

      // step2:  IR -> current logic-based data structure
      switch ( anchor_curr )
      {
      case lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_ABC_NTK_STRASH_AIG:
      {
        pNtk = lf::logic::convert_lsils_2_abc<NtkIR>( ntk );
        babc::Abc_FrameSetCurrentNetwork( frame_abc_, pNtk );
        break;
      }
      case lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_AIG:
      {
        frame_lsils_.curr_aig = lf::logic::convert_lsils_internal<lsils::aig_seq_network, NtkIR>( ntk );
        break;
      }
      case lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_XAG:
      {
        frame_lsils_.curr_xag = lf::logic::convert_lsils_internal<lsils::xag_seq_network, NtkIR>( ntk );
        break;
      }
      case lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_MIG:
      {
        frame_lsils_.curr_mig = lf::logic::convert_lsils_internal<lsils::mig_seq_network, NtkIR>( ntk );
        break;
      }
      case lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_XMG:
      {
        frame_lsils_.curr_xmg = lf::logic::convert_lsils_internal<lsils::xmg_seq_network, NtkIR>( ntk );
        break;
      }
      case lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_NTK_LOGIC_GTG:
      {
        frame_lsils_.curr_gtg = lf::logic::convert_lsils_internal<lsils::gtg_seq_network, NtkIR>( ntk );
        break;
      }
      default:
      {
        assert( false );
        break;
      }
      }
    }
  }

  /**
   * @brief get the current network for the given type
   * @tparam T current network type
   * @return ntk
   */
  template<typename T>
  T current()
  {
    if constexpr ( std::is_same_v<T, babc::Abc_Frame_t*> )
    {
      return frame_abc_;
    }
    else if constexpr ( std::is_same_v<T, lsils::Lsils_Frame_t> )
    {
      return frame_lsils_;
    }
    else if constexpr ( std::is_same_v<T, lsils::aig_seq_network> )
    {
      return frame_lsils_.curr_aig;
    }
    else if constexpr ( std::is_same_v<T, lsils::xag_seq_network> )
    {
      return frame_lsils_.curr_xag;
    }
    else if constexpr ( std::is_same_v<T, lsils::mig_seq_network> )
    {
      return frame_lsils_.curr_mig;
    }
    else if constexpr ( std::is_same_v<T, lsils::xmg_seq_network> )
    {
      return frame_lsils_.curr_xmg;
    }
    else if constexpr ( std::is_same_v<T, lsils::gtg_seq_network> )
    {
      return frame_lsils_.curr_gtg;
    }
    else if constexpr ( std::is_same_v<T, lsils::blut_seq_network> )
    {
      return frame_lsils_.netlist_asic;
    }
    else if constexpr ( std::is_same_v<T, lsils::klut_seq_network> )
    {
      return frame_lsils_.netlist_fpga;
    }
    else if constexpr ( std::is_same_v<T, lsils::lib_gates> )
    {
      return frame_lsils_.gates;
    }
    else
    {
      std::cerr << "Unhandled current type provided." << std::endl;
      assert( false );
      return nullptr;
    }
  }

  template<typename T>
  void set_current( T obj )
  {
    // Update specific network pointer based on the type of T
    if constexpr ( std::is_same_v<T, lsils::Lsils_Frame_t> )
    {
      frame_lsils_ = obj;
    }
    else if constexpr ( std::is_same_v<T, lsils::aig_seq_network> )
    {
      frame_lsils_.curr_aig = obj;
    }
    else if constexpr ( std::is_same_v<T, lsils::xag_seq_network> )
    {
      frame_lsils_.curr_xag = obj;
    }
    else if constexpr ( std::is_same_v<T, lsils::mig_seq_network> )
    {
      frame_lsils_.curr_mig = obj;
    }
    else if constexpr ( std::is_same_v<T, lsils::xmg_seq_network> )
    {
      frame_lsils_.curr_xmg = obj;
    }
    else if constexpr ( std::is_same_v<T, lsils::gtg_seq_network> )
    {
      frame_lsils_.curr_gtg = obj;
    }
    else if constexpr ( std::is_same_v<T, lsils::cvg_seq_network> )
    {
      frame_lsils_.curr_cvg = obj;
    }
    else if constexpr ( std::is_same_v<T, lsils::blut_seq_network> )
    {
      frame_lsils_.netlist_asic = obj;
    }
    else if constexpr ( std::is_same_v<T, lsils::klut_seq_network> )
    {
      frame_lsils_.netlist_fpga = obj;
    }
    else if constexpr ( std::is_same_v<T, lsils::lib_gates> )
    {
      frame_lsils_.gates = obj;
    }
    else
    {
      std::cerr << "Unhandled current type provided." << std::endl;
      assert( false );
    }
  }

  // process the ports
  lf::misc::Ports& ports() { return ports_; }
  lf::misc::Ports ports() const { return ports_; }

private:
  LogicManager() = default;
  ~LogicManager() = default;
  LogicManager( const LogicManager& ) = delete;
  LogicManager& operator=( const LogicManager& ) = delete;

private:
  static LogicManager* instance_;

  babc::Abc_Frame_t* frame_abc_ = nullptr; // store the abc frame
  lsils::Lsils_Frame_t frame_lsils_;       // store the lsils frame
  lf::misc::Ports ports_;                  // store the input/output port names
}; // class LogicManager

LogicManager* LogicManager::instance_ = nullptr;

enum E_LF_LOGIC_NTK_TYPE
{
  E_LF_LOGIC_NTK_TYPE_AIG = 0,
  E_LF_LOGIC_NTK_TYPE_XAG,
  E_LF_LOGIC_NTK_TYPE_MIG,
  E_LF_LOGIC_NTK_TYPE_XMG,
  E_LF_LOGIC_NTK_TYPE_GTG,
  E_LF_LOGIC_NTK_TYPE_CVG,
  E_LF_LOGIC_NTK_TYPE_BLG,
  E_LF_LOGIC_NTK_TYPE_KLUT,
  E_LF_LOGIC_NTK_TYPE_BLUT,
}; // enum E_LF_LOGIC_NTK_TYPE

/**
 * @class
 * @brief
 */
class LogicNtkType
{
public:
  static LogicNtkType* get_instance()
  {
    if ( instance_ == nullptr )
    {
      instance_ = new LogicNtkType;
    }
    return instance_;
  }

  void set_nkt_type( E_LF_LOGIC_NTK_TYPE ntk_type ) { ntk_type_ = ntk_type; }

  void set_nkt_type( const std::string& ntk_type )
  {
    if ( ntk_type == "aig" )
      ntk_type_ = E_LF_LOGIC_NTK_TYPE_AIG;
    else if ( ntk_type == "xag" )
      ntk_type_ = E_LF_LOGIC_NTK_TYPE_XAG;
    else if ( ntk_type == "mig" )
      ntk_type_ = E_LF_LOGIC_NTK_TYPE_MIG;
    else if ( ntk_type == "xmg" )
      ntk_type_ = E_LF_LOGIC_NTK_TYPE_XMG;
    else if ( ntk_type == "gtg" )
      ntk_type_ = E_LF_LOGIC_NTK_TYPE_GTG;
    else if ( ntk_type == "cvg" )
      ntk_type_ = E_LF_LOGIC_NTK_TYPE_CVG;
    else if ( ntk_type == "blg" )
      ntk_type_ = E_LF_LOGIC_NTK_TYPE_BLG;
    else if ( ntk_type == "klut" )
      ntk_type_ = E_LF_LOGIC_NTK_TYPE_KLUT;
    else if ( ntk_type == "blut" )
      ntk_type_ = E_LF_LOGIC_NTK_TYPE_BLUT;
  }

  E_LF_LOGIC_NTK_TYPE get_nkt_type() const { return ntk_type_; }

private:
  LogicNtkType() = default;
  ~LogicNtkType() = default;
  LogicNtkType( const LogicNtkType& ) = delete;
  LogicNtkType& operator=( const LogicNtkType& ) = delete;

private:
  static LogicNtkType* instance_;
  E_LF_LOGIC_NTK_TYPE ntk_type_{ E_LF_LOGIC_NTK_TYPE_AIG };
}; // class LogicManager

LogicNtkType* LogicNtkType::instance_ = nullptr;

} // end namespace logic

} // end namespace lf