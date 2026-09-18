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

    glm::vec3 m_pointLightPos{-3.2f, 1.0f, -4.0f};
    glm::vec3 m_pointLightAmbient{0.05f, 0.05f, 0.05f};
    glm::vec3 m_pointLightDiffuse{1.0f, 0.8f, 0.6f};
    glm::vec3 m_pointLightSpecular{1.0f, 1.0f, 1.0f};

    glm::vec3 m_directionalLightDirection{-0.2f, -1.0f, -0.3f};
    glm::vec3 m_directionalLightAmbient{0.05f, 0.05f, 0.05f};
    glm::vec3 m_directionalLightDiffuse{0.8f, 0.8f, 1.0f};
    glm::vec3 m_directionalLightSpecular{1.0f, 1.0f, 1.0f};

    int m_eventStage{0};
    float m_eventTimer{0.0f};
    float m_sunScale{0.08f};
    bool m_bigBang{false};
};

}

#endif // MAINCONTROLLER_HPP