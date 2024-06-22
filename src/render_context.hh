#ifndef MGE_RENDER_CONTEXT_HH
#define MGE_RENDER_CONTEXT_HH

#include "mgepch.hh"

namespace mge {
class RenderContext {
  struct BoundBufferData {
    GLuint id;
    bool is_mapped;
  };

 public:
  ~RenderContext();

  static std::shared_ptr<RenderContext> create();
  void terminate();

  static size_t glSizeofType(GLenum type);
  static GLenum glCheckError_(const char* file, int line);

  inline static RenderContext& get_instance() { return *s_instance; }

  GLuint create_shader(GLenum type);
  void destroy_shader(GLuint id);

  void set_shader_source(GLuint id, const char* source);
  void compile_shader(GLuint id);

  GLuint create_shader_program();
  void destroy_shader_program(GLuint id);

  void bind_shader_program(GLuint id);
  void unbind_shader_program(GLuint id);
  GLuint get_bound_shader_program() const;

  std::vector<std::pair<std::string, GLenum>> get_uniforms(GLuint id);
  GLuint get_uniform_location(GLuint id, const std::string& name);

  void attach_shader(GLuint program_id, GLuint shader_id);
  void link_shader_program(GLuint id);

  GLuint create_buffer();
  void destroy_buffer(GLuint id);

  void bind_buffer(GLenum type, GLuint id);
  void unbind_buffer(GLenum type, GLuint id);
  GLuint get_bound_buffer(GLenum type) const;

  void* map_buffer(GLenum type, GLuint id, GLenum access_mode);
  void unmap_buffer(GLenum type, GLuint id);
  GLuint get_mapped_buffer(GLenum type) const;

  void copy_buffer(GLenum type, GLuint id, unsigned int size, void* data, GLenum usage);
  void copy_buffer_subregion(GLenum type, GLuint id, unsigned int offset, unsigned int size, void* data);

  GLuint create_vertex_array();
  void destroy_vertex_array(GLuint id);

  void bind_vertex_array(GLuint id);
  void unbind_vertex_array(GLuint id);
  GLuint get_bound_vertex_array() const;

  void add_vertex_array_attribute(GLuint array_id, GLuint attrib_idx, GLuint size, GLenum type, GLsizei stride,
                                  const void* ptr);
  void add_vertex_array_instanced_attribute(GLuint array_id, GLuint attrib_idx, GLuint size, GLenum type,
                                            GLsizei stride, const void* ptr, GLuint divisor);

  void set_viewport_dims(GLuint minx, GLuint miny, GLuint maxx, GLuint maxy);

 private:
  static std::shared_ptr<RenderContext> s_instance;
  GLuint m_bound_program;
  std::unordered_map<GLenum, BoundBufferData> m_bound_buffers;
  GLuint m_bound_vertex_array;

  RenderContext();
  void init();
};

#define glCheckError() RenderContext::glCheckError_(__FILE__, __LINE__)
}  // namespace mge

#endif  // MGE_RENDER_CONTEXT_HH