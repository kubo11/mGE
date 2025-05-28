#include "texture.hh"

#include "../render_context.hh"

namespace mge {
Texture::Texture(glm::uvec2 size) : m_size(size) {
  glGenTextures(1, &m_texture_id);
  glCheckError();
  glBindTexture(GL_TEXTURE_2D, m_texture_id);
  glCheckError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glCheckError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glCheckError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glCheckError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glCheckError();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_size.x, m_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glCheckError();
}

Texture::~Texture() {
  glDeleteTextures(1, &m_texture_id);
  glCheckError();
}

void Texture::use(unsigned int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glCheckError();
  glBindTexture(GL_TEXTURE_2D, m_texture_id);
  glCheckError();
}

void Texture::copy(const glm::u8vec3* data) {
  glBindTexture(GL_TEXTURE_2D, m_texture_id);
  glCheckError();
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_size.x, m_size.y, GL_RGB, GL_UNSIGNED_BYTE, data);
  glCheckError();
}

unsigned int Texture::get_id() const {
  return m_texture_id;
}
} // namespace mge
