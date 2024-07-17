#pragma once
#include "kernel/yosys.h"

#define lfAmINST lf::arch::ArchManager::get_instance()

namespace lf
{
namespace arch
{

enum class E_ToolArchType
{
  E_ARCH_YOSYS,
  E_ARCH_IVERILOG,
};

class ArchManager
{
public:
  static ArchManager* get_instance()
  {
    if ( instance_ == nullptr )
    {
      instance_ = new ArchManager;
    }
    return instance_;
  }

  void start()
  {
    Yosys::log_streams.push_back( &std::cout );
    Yosys::log_error_stderr = true;
    Yosys::yosys_setup();
    frame_yosys_ = Yosys::yosys_get_design();
  }

  void stop()
  {
    Yosys::yosys_shutdown();
  }

  template<typename T>
  T current()
  {
    if constexpr ( std::is_same_v<T, Yosys::RTLIL::Design*> )
    {
      return frame_yosys_;
    }
  }

private:
  ArchManager() = default;
  ~ArchManager() = default;
  ArchManager( const ArchManager& ) = delete;
  ArchManager& operator=( const ArchManager& ) = delete;

private:
  static ArchManager* instance_;

  Yosys::RTLIL::Design* frame_yosys_;
  E_ToolArchType type_prev_ = { E_ToolArchType::E_ARCH_YOSYS };
  E_ToolArchType type_curr_ = { E_ToolArchType::E_ARCH_YOSYS };

}; // class ArchManager

ArchManager* ArchManager::instance_ = nullptr;

} // namespace arch

} // namespace lf