#ifndef MGE_RENDERER_MATERIAL_HH
#define MGE_RENDERER_MATERIAL_HH

namespace mge {
struct Material {
  glm::ivec3 color;
  float ambient;
  float diffuse;
  float specular;
  float shininess;

  Material(glm::ivec3 color, float ambient, float diffuse, float specular,
           float shininess)
      : color(color),
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        shininess(shininess) {}
};
}  // namespace mge

#endif  // MGE_RENDERER_MATERIAL_HH