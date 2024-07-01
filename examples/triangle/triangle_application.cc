#include "triangle_application.hh"
#include "triangle_layer.hh"

TriangleApplication::TriangleApplication() { push_layer(std::move(std::make_unique<TriangleLayer>())); }