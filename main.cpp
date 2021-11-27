#include "engine.hpp"
#include "model.hpp"

constexpr int width = 1920;
constexpr int heigh = 1080;

int main(int argc, char** argv)
{
    Engine engine("RealEngine", width, heigh);

    Camera camera(glm::vec3(0.0f, 1000.0f, 2500.0f));
    camera.SetSpeed(350);
    engine.SetCamera(camera);

    Model city;
    city.Load("../models/nimbasa", "Nimbasa City.obj");

    engine.AddModel(city);

    engine.Run();

    return 0;
}