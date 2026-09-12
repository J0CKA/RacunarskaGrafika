#include "MainController.hpp"

#include <engine/core/Controller.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/Model.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <engine/resources/Shader.hpp>
#include <engine/resources/Texture.hpp>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace {

constexpr float PI = 3.14159265359f;

const float FLOOR_VERTICES[] = {
    // position                 // texture
    15.0f, -0.5f,  15.0f,       15.0f,  0.0f,
   -15.0f, -0.5f,  15.0f,        0.0f,  0.0f,
   -15.0f, -0.5f, -15.0f,        0.0f, 15.0f,

    15.0f, -0.5f,  15.0f,       15.0f,  0.0f,
   -15.0f, -0.5f, -15.0f,        0.0f, 15.0f,
    15.0f, -0.5f, -15.0f,       15.0f, 15.0f
};

const float BACK_WALL_VERTICES[] = {
    15.0f, 7.0f, -15.0f, 6.0f, 3.0f,
   -15.0f, 7.0f, -15.0f, 0.0f, 3.0f,
   -15.0f, -0.5f, -15.0f, 0.0f, 0.0f,

    15.0f, 7.0f, -15.0f, 6.0f, 3.0f,
   -15.0f, -0.5f, -15.0f, 0.0f, 0.0f,
    15.0f, -0.5f, -15.0f, 6.0f, 0.0f
};

const float LEFT_WALL_VERTICES[] = {
    -15.0f, 7.0f,  15.0f, 6.0f, 3.0f,
    -15.0f, 7.0f, -15.0f, 0.0f, 3.0f,
    -15.0f, -0.5f, -15.0f, 0.0f, 0.0f,

    -15.0f, 7.0f,  15.0f, 6.0f, 3.0f,
    -15.0f, -0.5f, -15.0f, 0.0f, 0.0f,
    -15.0f, -0.5f,  15.0f, 6.0f, 0.0f
};

const float RIGHT_WALL_VERTICES[] = {
    15.0f, 7.0f, -15.0f, 6.0f, 3.0f,
    15.0f, 7.0f,  15.0f, 0.0f, 3.0f,
    15.0f, -0.5f,  15.0f, 0.0f, 0.0f,

    15.0f, 7.0f, -15.0f, 6.0f, 3.0f,
    15.0f, -0.5f,  15.0f, 0.0f, 0.0f,
    15.0f, -0.5f, -15.0f, 6.0f, 0.0f
};

const float CUBE_VERTICES[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
};

void create_textured_vao(unsigned int &vao, unsigned int &vbo, const float *vertices, std::size_t size) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

} // namespace

void MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();

    create_floor();
    create_walls();
    create_podium();
    create_cube();
}

void MainController::create_floor() {
    create_textured_vao(m_floor_vao, m_floor_vbo, FLOOR_VERTICES, sizeof(FLOOR_VERTICES));
}

void MainController::create_walls() {
    create_textured_vao(m_back_wall_vao, m_back_wall_vbo, BACK_WALL_VERTICES, sizeof(BACK_WALL_VERTICES));
    create_textured_vao(m_left_wall_vao, m_left_wall_vbo, LEFT_WALL_VERTICES, sizeof(LEFT_WALL_VERTICES));
    create_textured_vao(m_right_wall_vao, m_right_wall_vbo, RIGHT_WALL_VERTICES, sizeof(RIGHT_WALL_VERTICES));
}

void MainController::create_cube() {
    create_textured_vao(m_cube_vao, m_cube_vbo, CUBE_VERTICES, sizeof(CUBE_VERTICES));
}

void MainController::create_podium() {
    constexpr int segments = 64;
    constexpr float radius = 2.8f;
    constexpr float base_y = -0.5f;
    constexpr float top_y = -0.25f;

    m_podium_vertices.clear();

    for (int i = 0; i < segments; ++i) {
        float angle1 = static_cast<float>(i) * 2.0f * PI / segments;
        float angle2 = static_cast<float>(i + 1) * 2.0f * PI / segments;

        float x1 = radius * std::cos(angle1);
        float z1 = radius * std::sin(angle1);
        float x2 = radius * std::cos(angle2);
        float z2 = radius * std::sin(angle2);

        float u1 = static_cast<float>(i) / segments;
        float u2 = static_cast<float>(i + 1) / segments;

        // Top
        m_podium_vertices.insert(m_podium_vertices.end(), {0.0f, top_y, 0.0f, 0.5f, 0.5f});
        m_podium_vertices.insert(m_podium_vertices.end(), {x1, top_y, z1, 0.5f + 0.5f * x1 / radius, 0.5f + 0.5f * z1 / radius});
        m_podium_vertices.insert(m_podium_vertices.end(), {x2, top_y, z2, 0.5f + 0.5f * x2 / radius, 0.5f + 0.5f * z2 / radius});

        // Side 1
        m_podium_vertices.insert(m_podium_vertices.end(), {x1, top_y, z1, u1, 1.0f});
        m_podium_vertices.insert(m_podium_vertices.end(), {x1, base_y, z1, u1, 0.0f});
        m_podium_vertices.insert(m_podium_vertices.end(), {x2, base_y, z2, u2, 0.0f});

        // Side 2
        m_podium_vertices.insert(m_podium_vertices.end(), {x1, top_y, z1, u1, 1.0f});
        m_podium_vertices.insert(m_podium_vertices.end(), {x2, base_y, z2, u2, 0.0f});
        m_podium_vertices.insert(m_podium_vertices.end(), {x2, top_y, z2, u2, 1.0f});
    }

    m_podium_vertex_count = static_cast<int>(m_podium_vertices.size() / 5);

    glGenVertexArrays(1, &m_podium_vao);
    glGenBuffers(1, &m_podium_vbo);

    glBindVertexArray(m_podium_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_podium_vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_podium_vertices.size() * sizeof(float)), m_podium_vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void MainController::update() {
    m_time += 0.01f;
}

void MainController::draw_textured_object(unsigned int vao, int vertex_count, unsigned int texture_id, const glm::mat4 &model) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto shader = resources->shader("showroom");

    shader->use();

    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 3.0f, 11.0f),
        glm::vec3(0.0f, 0.8f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        1280.0f / 720.0f,
        0.1f,
        100.0f
    );

    shader->set_mat4("model", model);
    shader->set_mat4("view", view);
    shader->set_mat4("projection", projection);
    shader->set_int("texture1", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    glBindVertexArray(0);
}

void MainController::draw_podium(const glm::vec3 &position) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto shader = resources->shader("showroom");
    auto podium_texture = resources->texture("podium1");

    shader->use();

    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 3.0f, 11.0f),
        glm::vec3(0.0f, 0.8f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        1280.0f / 720.0f,
        0.1f,
        100.0f
    );

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

    shader->set_mat4("model", model);
    shader->set_mat4("view", view);
    shader->set_mat4("projection", projection);
    shader->set_int("texture1", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, podium_texture->id());

    glBindVertexArray(m_podium_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_podium_vertex_count);
    glBindVertexArray(0);
}

void MainController::draw() {
    glm::mat4 view = glm::lookAt(
    glm::vec3(0.0f, 3.0f, 11.0f),
    glm::vec3(0.0f, 0.5f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f)
);

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        1280.0f / 720.0f,
        0.1f,
        100.0f
    );
    glViewport(0, 0, 1280, 720);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.15f, 0.16f, 0.21f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

    auto floor_texture = resources->texture("tiles");
    auto wall_texture = resources->texture("zidovi");

    // POD
    draw_textured_object(m_floor_vao, 6, floor_texture->id(), glm::mat4(1.0f));

    // ZIDOVI
    draw_textured_object(m_back_wall_vao, 6, wall_texture->id(), glm::mat4(1.0f));
    draw_textured_object(m_left_wall_vao, 6, wall_texture->id(), glm::mat4(1.0f));
    draw_textured_object(m_right_wall_vao, 6, wall_texture->id(), glm::mat4(1.0f));

    // PODIJUMI
    const glm::vec3 podium_positions[3] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(-6.5f, 0.0f, -4.0f),
        glm::vec3(6.5f, 0.0f, -4.0f)
    };

    for (const auto &position : podium_positions) {
        draw_podium(position);
    }




    // -------------------------
    // AUTOMOBILI
    // -------------------------

    auto porsche = resources->model("porsche");
    auto bugatti = resources->model("bugatti");
    auto lexus = resources->model("lexus");
    auto car_shader = resources->shader("porsche");

    car_shader->use();



    car_shader->set_mat4("view", view);
    car_shader->set_mat4("projection", projection);
    car_shader->set_vec3("lightPos", glm::vec3(4.0f, 6.0f, 4.0f));
    car_shader->set_vec3("viewPos", glm::vec3(0.0f, 3.0f, 10.0f));

    // PORSCHE
    {
        glm::mat4 model(1.0f);

        model = glm::translate(
            model,
            glm::vec3(0.0f, 0.2f, 0.0f)
        );

        model = glm::rotate(
            model,
            m_time,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        model = glm::scale(
            model,
            glm::vec3(1.0f)
        );

        car_shader->set_mat4("model", model);
        porsche->draw(car_shader);
    }

    // BUGATTI
    {
        glm::mat4 model(1.0f);

        // POZICIJA
        model = glm::translate(
            model,
            glm::vec3(-6.5f, 0.3f, -4.0f)
        );

        // ROTACIJA - samo okretanje oko Y ose
        model = glm::rotate(
            model,
            m_time,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        model = glm::scale(
            model,
            glm::vec3(1.0f)
        );

        car_shader->set_mat4("model", model);

        bugatti->draw(car_shader);
    }

    // 3. LEXUS - DESNI PODIJUM
    // LEXUS
    {
        glm::mat4 model(1.0f);

        model = glm::translate(
            model,
            glm::vec3(6.5f, 0.2f, -4.0f)
        );

        // Ako je potrebno promeni orijentaciju
        model = glm::rotate(
            model,
            glm::radians(0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        model = glm::rotate(
            model,
            m_time,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        model = glm::scale(
            model,
            glm::vec3(0.01f)
        );

        car_shader->set_mat4("model", model);
        lexus->draw(car_shader);
    }
}

void MainController::end_draw() {
    engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers();
}

void MainController::terminate() {
    glDeleteVertexArrays(1, &m_floor_vao);
    glDeleteBuffers(1, &m_floor_vbo);

    glDeleteVertexArrays(1, &m_back_wall_vao);
    glDeleteBuffers(1, &m_back_wall_vbo);

    glDeleteVertexArrays(1, &m_left_wall_vao);
    glDeleteBuffers(1, &m_left_wall_vbo);

    glDeleteVertexArrays(1, &m_right_wall_vao);
    glDeleteBuffers(1, &m_right_wall_vbo);

    glDeleteVertexArrays(1, &m_podium_vao); // ili m_podium_vao zavisno od zaglavlja
    glDeleteVertexArrays(1, &m_podium_vao);
    glDeleteBuffers(1, &m_podium_vbo);

    glDeleteVertexArrays(1, &m_cube_vao);
    glDeleteBuffers(1, &m_cube_vbo);
}