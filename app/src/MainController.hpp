#ifndef MAIN_CONTROLLER_HPP
#define MAIN_CONTROLLER_HPP

#include <engine/core/Controller.hpp>

#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

class MainController final : public engine::core::Controller {
protected:
    void initialize() override;
    void update() override;
    void draw() override;
    void terminate() override;
    void end_draw() override;

private:
    void create_floor();
    void create_walls();
    void create_podium();
    void create_cube();

    void draw_textured_object(
        unsigned int vao,
        int vertex_count,
        unsigned int texture_id,
        const glm::mat4 &model
    );

    void draw_podium(const glm::vec3 &position);
    void draw_depth_scene(const glm::mat4 shadowMatrices[6]);
    void draw_depth_object(unsigned int vao, int vertex_count, const glm::mat4 &model);
    void create_framebuffer();
    void create_point_shadow_buffer();
    void create_screen_quad();
    void draw_screen_quad();

private:
    unsigned int m_floor_vao = 0;
    unsigned int m_floor_vbo = 0;

    unsigned int m_back_wall_vao = 0;
    unsigned int m_back_wall_vbo = 0;

    unsigned int m_left_wall_vao = 0;
    unsigned int m_left_wall_vbo = 0;

    unsigned int m_right_wall_vao = 0;
    unsigned int m_right_wall_vbo = 0;

    unsigned int m_podium_vao = 0;
    unsigned int m_podium_vbo = 0;

    unsigned int m_cube_vao = 0;
    unsigned int m_cube_vbo = 0;

    int m_podium_vertex_count = 0;

    float m_time = 0.0f;

    glm::vec3 m_pointLightPos{4.0f, 6.0f, 4.0f};
    glm::vec3 m_pointLightColor{1.0f, 0.85f, 0.65f};
    float m_pointLightIntensity = 1.4f;
    bool m_pointLightEnabled = true;

    glm::vec3 m_directionalLightDir{-0.4f, -1.0f, -0.25f};
    glm::vec3 m_directionalLightColor{0.55f, 0.65f, 1.0f};
    float m_directionalLightIntensity = 0.45f;
    bool m_directionalLightEnabled = true;

    int m_eventStage = 0;
    float m_eventTimer = 0.0f;
    float m_porscheSpeed = 1.0f;

    glm::vec3 m_cameraPos{0.0f, 3.0f, 11.0f};
    float m_cameraYaw = -90.0f;
    float m_cameraPitch = -10.0f;
    float m_cameraMoveSpeed = 6.0f;
    float m_cameraLookSpeed = 70.0f;

    unsigned int m_framebuffer = 0;
    unsigned int m_colorTexture = 0;
    unsigned int m_depthStencilRbo = 0;


    unsigned int m_screen_vao = 0;
    unsigned int m_screen_vbo = 0;

    unsigned int m_shadowFbo = 0;
    unsigned int m_depthCubemap = 0;
    const float m_shadowNearPlane = 0.1f;
    const float m_shadowFarPlane = 30.0f;

    bool m_postProcessEnabled = true;
    bool m_pointShadowsEnabled = true;

    int m_framebufferWidth = 1280;
    int m_framebufferHeight = 720;
    int m_shadowSize = 1024;

    std::vector<float> m_podium_vertices;
};

#endif // MAIN_CONTROLLER_HPP
