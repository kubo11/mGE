#include "render_pipeline.hh"

namespace mge {
RenderPipeline::RenderPipeline() : m_actions{} {}

RenderPipeline::RenderPipeline(std::vector<std::unique_ptr<RenderPipelineAction>>&& actions)
    : m_actions{std::move(actions)} {}

void RenderPipeline::run() {
  for (auto& action : m_actions) {
    action->run();
  }

  for (auto& action : m_actions | std::views::reverse) {
    action->cleanup();
  }
}
}  // namespace mge
