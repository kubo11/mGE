#include "gpu_canvas.hh"

namespace mge {
GPUCanvas::GPUCanvas() {}

void GPUCanvas::init(const CPUBitmap& bitmap) {
  m_texture_shader = std::make_unique<mge::Shader>(
      fs::current_path() / "src" / "shaders" / "canvas.vert",
      fs::current_path() / "src" / "shaders" / "canvas.frag");

  float vertices[] = {
      0.6f,  1.0f,  0.0f, 1.0f, 1.0f,  // top right
      0.6f,  -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom left
      -1.0f, 1.0f,  0.0f, 0.0f, 1.0f   // top left
  };
  unsigned int indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

  unsigned int vbo, ebo;
  glGenVertexArrays(1, &m_vao_id);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(m_vao_id);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenTextures(1, &m_texture_id);
  glBindTexture(GL_TEXTURE_2D, m_texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.get_width(),
               bitmap.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.data());
  glGenerateMipmap(GL_TEXTURE_2D);
}

void GPUCanvas::update(const CPUBitmap& bitmap) {
  glBindTexture(GL_TEXTURE_2D, m_texture_id);

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, bitmap.get_width(),
                  bitmap.get_height(), GL_RGB, GL_UNSIGNED_BYTE, bitmap.data());
}

void GPUCanvas::draw() {
  glActiveTexture(GL_TEXTURE0);
  m_texture_shader->use();
  glBindVertexArray(m_vao_id);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
}  // namespace mge
