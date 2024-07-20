#include "instancing_application.hh"
#include "instancing_layer.hh"

InstancingApplication::InstancingApplication() { push_layer(std::move(std::make_unique<InstancingLayer>())); }