#ifndef MGE_RENDERER_COMPONENTS_TAG_COMPONENT_HH
#define MGE_RENDERER_COMPONENTS_TAG_COMPONENT_HH

#include "../../../mgepch.hh"

namespace mge {
struct TagComponent {
  TagComponent(const std::string& tag = "") : m_tag(tag) {}

  inline void set_tag(const std::string& tag) { m_tag = tag; }
  const std::string& get_tag() const { return m_tag; }

  inline operator std::string() const { return m_tag; }

 private:
  std::string m_tag;
};
}  // namespace mge

#endif  // MGE_RENDERER_COMPONENTS_TAG_COMPONENT_HH