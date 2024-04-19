/**
 * @author [yesky]
 * @email [a316606581@gmail.com]
 * @create date 2024-01-10 15:24:03
 * @modify date 2024-01-10 15:24:03
 * @desc [description]
 */

#include <boost/filesystem/path.hpp>
#include <iostream>

#include "app.hpp"
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"
#include "threadpool.hpp"

class Myapp : public App {
 public:
  Myapp() = default;
  ~Myapp() = default;
  virtual void StartUp() final {
    shader = std::make_shared<Shader>(
        Shader("../src/shader/baopo/baopo.vs", "../src/shader/baopo/baopo.fs"));
    lights.push_back(new DirLight());
    auto las = *(lights.end() - 1);
    setDirLight(dynamic_cast<DirLight *>(las), 0, shader);

    LightSpot *lightspot = new LightSpot();
    lightspot->postion = camera.Position;
    lightspot->direction = camera.Front;
    lightspot->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    lightspot->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    lightspot->specular = glm::vec3(1.0f, 1.0f, 1.0f);
    lights.push_back(lightspot);
    las = *(lights.end() - 1);
    setLightSpot(dynamic_cast<LightSpot *>(las), 0, shader);

    shader.get()->use();
    shader.get()->setVec3("viewPos", camera.Position);
    shader.get()->setFloat("shininess", 32.0f);
    shader.get()->setInt("lightcnt", lights.size() - 2);
    ourmodel = std::make_shared<Model>(Model(
        boost::filesystem::absolute("../asset/model/nanosuit/nanosuit.obj")
            .c_str()));
  }

  virtual void Update() final {
    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("OpenGL Render",
                 &control_window);  // Create a window called "Hello, world!"
                                    // and append into it.

    if (ImGui::Button("reset", ImVec2(0, 0))) {
      scale = 1.0;
      translate = glm::vec3(0.0f);
      clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
      lights[1]->postion = camera.Position;
      pointlightPos = glm::vec3(0.0f);
      while (lights.size() > 2) {
        lights.pop_back();
      }
      shader.get()->use();
      shader.get()->setInt("lightcnt", lights.size() - 2);
    }

    ImGui::ColorEdit3("background", (float *)&clear_color);
    ImGui::SliderFloat("scale", &scale, 0.0f,
                       2.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SliderFloat3("translate", glm::value_ptr(translate), -10.0f, 10.0f);
    ImGui::SliderFloat3("LightSpot_Pos", glm::value_ptr(lights[1]->postion),
                        -100.0f, 100.0f);
    ImGui::SliderFloat3("PointLight_Pos", glm::value_ptr(pointlightPos),
                        -1000.0f, 1000.0f);

    if (ImGui::Button("add pointlight", ImVec2(0, 0))) {
      lights.push_back(
          new PointLight(pointlightPos, glm::vec3(1.0f, 1.0f, 1.0f),
                         glm::vec3(0.8f, 0.8f, 0.8f),
                         glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f));
      auto las = *(lights.end() - 1);
      setPointLight(dynamic_cast<PointLight *>(las), lights.size() - 3, shader);

      shader.get()->use();
      shader.get()->setInt("lightcnt", lights.size() - 2);

      LightPosition.push_back(las->postion);
    }

    if (ImGui::Button("PolygonMode", ImVec2(0, 0))) {
      mode = (mode == GL_FILL ? GL_LINE : GL_FILL);
    }
    ImGui::Text("Number of PointLights is %d", int(lights.size() - 2));
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }

  virtual void Render() final {
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    view = glm::translate(view, translate);
    shader.get()->use();
    shader.get()->setMat4("projection", projection);
    shader.get()->setMat4("view", view);
    shader.get()->setMat4("model", model);
    shader.get()->setVec3("viewPos", camera.Position);
    ourmodel.get()->Draw(shader, mode);
  }

  void setPointLight(PointLight *light, const int id,
                     std::shared_ptr<Shader> &shader) {
    shader.get()->use();
    shader.get()->setVec3("pointLights[" + std::to_string(id) + "].position",
                          light->postion);
    shader.get()->setVec3("pointLights[" + std::to_string(id) + "].ambient",
                          light->ambient);
    shader.get()->setVec3("pointLights[" + std::to_string(id) + "].diffuse",
                          light->diffuse);
    shader.get()->setVec3("pointLights[" + std::to_string(id) + "].specular",
                          light->specular);
    shader.get()->setFloat("pointLights[" + std::to_string(id) + "].constant",
                           light->constant);
    shader.get()->setFloat("pointLights[" + std::to_string(id) + "].linear",
                           light->linear);
    shader.get()->setFloat("pointLights[" + std::to_string(id) + "].quadratic",
                           light->quadratic);
    shader.get()->setVec3("viewPos", light->angles);
  }

  void setLightSpot(LightSpot *light, const int id,
                    std::shared_ptr<Shader> &shader) {
    shader.get()->use();
    shader.get()->setVec3("spotLight.position", light->postion);
    shader.get()->setVec3("spotLight.ambient", light->ambient);
    shader.get()->setVec3("spotLight.diffuse", light->diffuse);
    shader.get()->setVec3("spotLight.specular", light->specular);
    shader.get()->setVec3("spotLight.direction", light->direction);
    shader.get()->setFloat("spotLight.cosPhyInner", light->cosPhyInner);
    shader.get()->setFloat("spotLight.cosPhyOuter", light->cosPhyOuter);
    shader.get()->setFloat("spotLight.constant", light->constant);
    shader.get()->setFloat("spotLight.linear", light->linear);
    shader.get()->setFloat("spotLight.quadratic", light->quadratic);
  }

  void setDirLight(DirLight *light, const int id,
                   std::shared_ptr<Shader> &shader) {
    shader.get()->use();
    shader.get()->setVec3("dirLight.ambient", light->ambient);
    shader.get()->setVec3("dirLight.diffuse", light->diffuse);
    shader.get()->setVec3("dirLight.specular", light->specular);
    shader.get()->setVec3("dirLight.direction", light->direction);
  }

 private:
  bool control_window = true;
  bool render_window = true;
  bool show_demo_window = false;
  std::shared_ptr<Model> ourmodel;
  std::shared_ptr<Shader> shader;
  // Model ourmodel;
  // Shader shader;
  float scale = 1.0;
  int mode = GL_FILL;
  glm::vec3 translate = glm::vec3(0.0f);
  glm::vec3 pointlightPos = glm::vec3(0.0f);
  std::vector<Light *> lights;
  std::vector<glm::vec3> LightPosition;
  glm::vec3 pointLightPositions[4] = {
      glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(5.0f, -5.0f, 4.0f),
      glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

  // pbr material
  
};

int main(int argc, char *argv[]) {
  Myapp app;
  app.Run();
  return 0;
}

/*
// std::cout<<show_demo_window<<std::endl;
        // 1. Show the big demo window (Most of the sample code is in
ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
ImGui!). if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End
pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello,
world!" and append into it.

            ImGui::Text("This is some useful text.");          // Display some
text (you can use a format strings too) ImGui::Checkbox("Demo Window",
&show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1
float using a slider from 0.0f to 1.0f ImGui::ColorEdit3("clear color", (float
*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked
(most widgets return true when edited/activated) counter++; ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f
/ ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a
pointer to our bool variable (the window will have a closing button that will
clear the bool when clicked) ImGui::Text("Hello from another window!"); if
(ImGui::Button("Close Me")) show_another_window = false; ImGui::End();
        }
*/
