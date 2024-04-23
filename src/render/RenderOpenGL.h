/*
 * @Author: yesky a316606581@gmail.com
 * @LastEditors: yesky a316606581@gmail.com
 * @LastEditTime: 2024-04-21 13:38:11
 * @Date: 2024-04-19 19:17:01
 * @FilePath: RenderOpenGL.h
 * @Description: 
 * Copyright (c) 2024 by yesky email: a316606581@gmail.com, All Rights Reserved.
 */

#pragma once
#include "Renderer.h"

namespace YeskyGL {
class RenderOpenGL : public Renderer {
 private:
  /* data */
 public:
  RenderOpenGL(/* args */);
  ~RenderOpenGL();
};

RenderOpenGL::RenderOpenGL(/* args */) {}

RenderOpenGL::~RenderOpenGL() {}
}  // namespace YeskyGL
