#ifndef MGE_RENDERER_DRAWABLE_HH
#define MGE_RENDERER_DRAWABLE_HH

#include "../mgepch.hh"

namespace mge {
class Drawable {
 public:
  Drawable() {}
  virtual ~Drawable() {}

  virtual void init_draw_data() = 0;
  virtual void draw() const = 0;
};
}  // namespace mge

#endif  // MGE_RENDERER_DRAWABLE_HH