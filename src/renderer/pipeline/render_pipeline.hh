#ifndef MGE_RENDERER_PIPELINE_RENDER_PIPELINE
#define MGE_RENDERER_PIPELINE_RENDER_PIPELINE

#include "../../mgepch.hh"

#include "render_pipeline_action.hh"

namespace mge {
class RenderPipeline {
 public:
  ~RenderPipeline() = default;

  PREVENT_COPY(RenderPipeline);

  void run();

 private:
  RenderPipeline();
  RenderPipeline(std::vector<std::unique_ptr<RenderPipelineAction>>&& actions);

  std::vector<std::unique_ptr<RenderPipelineAction>> m_actions;

  friend class RenderPipelineBuilder;
};
}  // namespace mge

#endif  // MGE_RENDERER_PIPELINE_RENDER_PIPELINEste