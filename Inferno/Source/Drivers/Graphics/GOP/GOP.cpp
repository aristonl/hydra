#include "GOP.hpp"

Framebuffer* framebuffer;

void SetGlobalFramebuffer(Framebuffer* temp) {
  framebuffer = temp;
}