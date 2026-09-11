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

    std::vector<float> m_podium_vertices;
};

#endif // MAIN_CONTROLLER_HPP