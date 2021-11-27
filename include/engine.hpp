#pragma once

#include "shader_program.hpp"
#include "camera.hpp"
#include "model.hpp"

#include <string>
#include <memory>
#include <list>

using uint = unsigned int;
struct deleter{ void operator()(GLFWwindow* pWnd) { glfwDestroyWindow(pWnd);} };

class Engine
{
public:
    Engine(const std::string& title, uint width, uint height);
    ~Engine() { glfwTerminate(); }

public:
    void Run();
    void SetCamera(Camera& camera) { m_camera = std::move(camera); };
    void AddModel(Model& model) { m_DrawQue.push_back(std::move(model));}

private:
    void process_keyboard();
    void process_mouse();

private:
    uint screen_width;
    uint screen_height;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    Camera m_camera;
    std::unique_ptr<GLFWwindow, deleter> m_pWnd;
    std::list<Model> m_DrawQue;
};