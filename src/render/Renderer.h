/**
 * @author [yesky]
 * @email [a316606581@gmail.com]
 * @create date 2024-01-10 15:25:07
 * @modify date 2024-01-10 15:25:07
 * @desc [description]
 */

#pragma once

namespace YeskyGL {
enum RendererType {
  Renderer_OPENGL,
  Renderer_VULKAN,
};
class Renderer {
 private:
  /* data */
 public:
  virtual RendererType type() = 0;
  virtual bool create() { return true; };
  virtual void distory(){};
  // framebuffer
};

}  // namespace YeskyGL