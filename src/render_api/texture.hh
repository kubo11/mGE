#ifndef MGE_RENDER_API_TEXTURE
#define MGE_RENDER_API_TEXTURE

#include "../mgepch.hh"

namespace mge {
class Texture {
 public:
  Texture(glm::uvec2 size);
  ~Texture();

  PREVENT_COPY(Texture);

  void use(unsigned int slot = 0);
  void copy(const glm::u8vec3* data);

  unsigned int get_id() const;

 private:
  glm::uvec2 m_size;
  unsigned int m_texture_id;
};
  
} // namespace mge


#endif // MGE_RENDER_API_TEXTURE