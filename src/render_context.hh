#ifndef MGE_RENDER_CONTEXT_HH
#define MGE_RENDER_CONTEXT_HH

#include "mgepch.hh"

namespace mge {
class RenderContext {
 public:
  ~RenderContext();

  static RenderContext& create();
  void terminate();

 private:
  static std::unique_ptr<RenderContext> s_instance;

  RenderContext();
  void init();
};
}  // namespace mge

#endif  // MGE_RENDER_CONTEXT_HH