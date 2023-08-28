#include "app.hpp"
// #include <iostream>
#include <boost/filesystem/path.hpp>
#include "camera.h"
#include "shader.h"
#include "model.h"
#include "mesh.h"
#include "threadpool.hpp"

class Myapp : public App
{
public:
    Myapp() = default;
    ~Myapp() = default;
    virtual void StartUp() final
    {
        shader = Shader("../src/shader/baopo/baopo.vs", "../src/shader/baopo/baopo.fs");
        ourmodel = Model(boost::filesystem::absolute("../asset/model/nanosuit/nanosuit.obj").c_str());
    }

    virtual void Update() final
    {
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("OpenGL Render", &control_window); // Create a window called "Hello, world!" and append into it.

        // ImGui::Button("reset",ImVec2(0,0));
        if(ImGui::Button("reset",ImVec2(0,0))) {
            scale =1.0;
            translate = glm::vec3(0.0f);
            clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        }

        ImGui::ColorEdit3("background", (float*)&clear_color);
        ImGui::SliderFloat("scale", &scale, 0.0f, 2.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat3("translate", glm::value_ptr(translate), -10.0f, 10.0f); 
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();

    }

    virtual void Render() final
    {
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(scale,scale,scale));
        view = glm::translate(view, translate);
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        shader.setFloat("time", static_cast<float>(glfwGetTime()));
        ourmodel.Draw(shader);
    }

private:
    bool control_window = true;
    bool render_window = true;
    bool show_demo_window  = false;
    Model ourmodel;
    Shader shader;
    float scale = 1.0;
    glm::vec3 translate = glm::vec3(0.0f);
};

int main(int argc, char *argv[])
{
    Myapp app;
    app.Run();
    return 0;
}



/*
// std::cout<<show_demo_window<<std::endl;
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
*/