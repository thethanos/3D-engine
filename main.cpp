#include "engine.hpp"
#include "model.hpp"

constexpr int width = 1920;
constexpr int heigh = 1080;

int main(int argc, char** argv)
{
    Engine engine("RealEngine", width, heigh);

    Camera camera(glm::vec3(0.0f, 10.0f, 25.0f));
    camera.SetSpeed(35);
    engine.SetCamera(camera);

    Model city;
    city.Load("../models/destroyer", "destroyer.obj");

    engine.AddModel(city);

    engine.Run();

    return 0;
}