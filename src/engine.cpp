#include <stdexcept>

#include "engine.hpp"

#include "glm/gtc/matrix_transform.hpp"

Engine::Engine(const std::string& title, uint width, uint height)
{
    screen_width  = width;
    screen_height = height;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_pWnd = std::unique_ptr<GLFWwindow, deleter>(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr));
    glfwMakeContextCurrent(m_pWnd.get());

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to load GLAD");

    glfwSetWindowSizeCallback(m_pWnd.get(), [](GLFWwindow* m_pWnd, int width, int height){ glViewport(0, 0, width, height);});
    glfwSetInputMode(m_pWnd.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::Run()
{   
    ShaderProgram sp("vertex.vs", "fragment.fs", "../shaders");

    sp.use();
    sp.set_uniform3fv("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
    sp.set_uniform3fv("light_pos", glm::vec3(0.0f, 100.0f, 0.0f));
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    sp.set_uniform_matrix4fv("model", model);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 120000.0f);
    sp.set_uniform_matrix4fv("projection", projection);
    
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(m_pWnd.get()))
    {   
        process_keyboard();
        process_mouse();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.5f, 0.5f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sp.set_uniform_matrix4fv("view", m_camera.GetView());
        for (auto& mesh : m_DrawQue)
            mesh.Draw(sp);

        glfwPollEvents();
        glfwSwapBuffers(m_pWnd.get());
    }
}

void Engine::process_keyboard()
{
    if (glfwGetKey(m_pWnd.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_pWnd.get(), true);
    
    if (glfwGetKey(m_pWnd.get(), GLFW_KEY_W) == GLFW_PRESS)
        m_camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
    if (glfwGetKey(m_pWnd.get(), GLFW_KEY_S) == GLFW_PRESS)
        m_camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
    if (glfwGetKey(m_pWnd.get(), GLFW_KEY_A) == GLFW_PRESS)
        m_camera.ProcessKeyboard(Camera::LEFT, deltaTime);
    if (glfwGetKey(m_pWnd.get(), GLFW_KEY_D) == GLFW_PRESS)
        m_camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
}

void Engine::process_mouse()
{   
    double xpos(0), ypos(0);
    glfwGetCursorPos(m_pWnd.get(), &xpos, &ypos);

    m_camera.ProcessMouse(xpos, ypos);
}