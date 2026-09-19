#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <string_view>
#include <glm/glm.hpp>

namespace app {

class MainController : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "app::MainController";
    }

private:
    void initialize() override;
    bool loop() override;
    void draw_sun();

    void update_camera();
    void update() override;

    void begin_draw() override;
    void draw_skybox();
    void draw() override;
    void end_draw() override;

    struct PointLight {
        glm::vec3 position{-3.2f, 1.0f, -4.0f};
        glm::vec3 ambient{0.05f, 0.05f, 0.05f};
        glm::vec3 diffuse{1.0f, 0.8f, 0.6f};
        glm::vec3 specular{1.0f, 1.0f, 1.0f};

        float constant{1.0f};
        float linear{0.09f};
        float quadratic{0.032f};
    };

    struct DirectionalLight {
        glm::vec3 direction{-0.2f, -1.0f, -0.3f};
        glm::vec3 ambient{0.05f, 0.05f, 0.05f};
        glm::vec3 diffuse{0.8f, 0.8f, 1.0f};
        glm::vec3 specular{1.0f, 1.0f, 1.0f};
    };

    PointLight m_point_light;
    DirectionalLight m_directional_light;

    int m_event_stage{0};
    float m_event_timer{0.0f};
    float m_sun_scale{0.08f};
    bool m_big_bang{false};
};

}

#endif // MAINCONTROLLER_HPP