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

    15.0f, -0.5f,  15.0f,      15.0f,  0.0f,
   -15.0f, -0.5f,  15.0f,       0.0f,  0.0f,
   -15.0f, -0.5f, -15.0f,       0.0f, 15.0f,

    15.0f, -0.5f,  15.0f,      15.0f,  0.0f,
   -15.0f, -0.5f, -15.0f,       0.0f, 15.0f,
    15.0f, -0.5f, -15.0f,      15.0f, 15.0f
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

void create_textured_vao(
    unsigned int &vao,
    unsigned int &vbo,
    const float *vertices,
    std::size_t size
) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(size),
        vertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        nullptr
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        reinterpret_cast<void *>(3 * sizeof(float))
    );
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
    create_textured_vao(
        m_floor_vao,
        m_floor_vbo,
        FLOOR_VERTICES,
        sizeof(FLOOR_VERTICES)
    );
}

void MainController::create_walls() {
    create_textured_vao(
        m_back_wall_vao,
        m_back_wall_vbo,
        BACK_WALL_VERTICES,
        sizeof(BACK_WALL_VERTICES)
    );

    create_textured_vao(
        m_left_wall_vao,
        m_left_wall_vbo,
        LEFT_WALL_VERTICES,
        sizeof(LEFT_WALL_VERTICES)
    );

    create_textured_vao(
        m_right_wall_vao,
        m_right_wall_vbo,
        RIGHT_WALL_VERTICES,
        sizeof(RIGHT_WALL_VERTICES)
    );
}

void MainController::create_cube() {
    create_textured_vao(
        m_cube_vao,
        m_cube_vbo,
        CUBE_VERTICES,
        sizeof(CUBE_VERTICES)
    );
}

void MainController::create_podium() {
    constexpr int segments = 64;

    constexpr float radius = 2.8f;
    constexpr float base_y = -0.5f;
    constexpr float top_y = -0.25f;

    m_podium_vertices.clear();

    for (int i = 0; i < segments; ++i) {
        float angle1 =
            static_cast<float>(i) * 2.0f * PI / segments;

        float angle2 =
            static_cast<float>(i + 1) * 2.0f * PI / segments;

        float x1 = radius * std::cos(angle1);
        float z1 = radius * std::sin(angle1);

        float x2 = radius * std::cos(angle2);
        float z2 = radius * std::sin(angle2);

        float u1 = static_cast<float>(i) / segments;
        float u2 = static_cast<float>(i + 1) / segments;

        // Top
        m_podium_vertices.push_back(0.0f);
        m_podium_vertices.push_back(top_y);
        m_podium_vertices.push_back(0.0f);
        m_podium_vertices.push_back(0.5f);
        m_podium_vertices.push_back(0.5f);

        m_podium_vertices.push_back(x1);
        m_podium_vertices.push_back(top_y);
        m_podium_vertices.push_back(z1);
        m_podium_vertices.push_back(0.5f + 0.5f * x1 / radius);
        m_podium_vertices.push_back(0.5f + 0.5f * z1 / radius);

        m_podium_vertices.push_back(x2);
        m_podium_vertices.push_back(top_y);
        m_podium_vertices.push_back(z2);
        m_podium_vertices.push_back(0.5f + 0.5f * x2 / radius);
        m_podium_vertices.push_back(0.5f + 0.5f * z2 / radius);

        // Side 1
        m_podium_vertices.push_back(x1);
        m_podium_vertices.push_back(top_y);
        m_podium_vertices.push_back(z1);
        m_podium_vertices.push_back(u1);
        m_podium_vertices.push_back(1.0f);

        m_podium_vertices.push_back(x1);
        m_podium_vertices.push_back(base_y);
        m_podium_vertices.push_back(z1);
        m_podium_vertices.push_back(u1);
        m_podium_vertices.push_back(0.0f);

        m_podium_vertices.push_back(x2);
        m_podium_vertices.push_back(base_y);
        m_podium_vertices.push_back(z2);
        m_podium_vertices.push_back(u2);
        m_podium_vertices.push_back(0.0f);

        // Side 2
        m_podium_vertices.push_back(x1);
        m_podium_vertices.push_back(top_y);
        m_podium_vertices.push_back(z1);
        m_podium_vertices.push_back(u1);
        m_podium_vertices.push_back(1.0f);

        m_podium_vertices.push_back(x2);
        m_podium_vertices.push_back(base_y);
        m_podium_vertices.push_back(z2);
        m_podium_vertices.push_back(u2);
        m_podium_vertices.push_back(0.0f);

        m_podium_vertices.push_back(x2);
        m_podium_vertices.push_back(top_y);
        m_podium_vertices.push_back(z2);
        m_podium_vertices.push_back(u2);
        m_podium_vertices.push_back(1.0f);
    }

    m_podium_vertex_count =
        static_cast<int>(m_podium_vertices.size() / 5);

    glGenVertexArrays(1, &m_podium_vao);
    glGenBuffers(1, &m_podium_vbo);

    glBindVertexArray(m_podium_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_podium_vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(
            m_podium_vertices.size() * sizeof(float)
        ),
        m_podium_vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        nullptr
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        reinterpret_cast<void *>(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void MainController::update() {
    m_time += 0.01f;
}

void MainController::draw_textured_object(
    unsigned int vao,
    int vertex_count,
    unsigned int texture_id,
    const glm::mat4 &model
) {
    auto resources =
        engine::core::Controller::get<
            engine::resources::ResourcesController
        >();

    auto shader = resources->shader("showroom");

    shader->use();

    glm::mat4 view =
        glm::lookAt(
            glm::vec3(0.0f, 3.0f, 11.0f),
            glm::vec3(0.0f, 0.8f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

    glm::mat4 projection =
        glm::perspective(
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
    auto resources =
        engine::core::Controller::get<
            engine::resources::ResourcesController
        >();

    auto shader = resources->shader("showroom");
    auto podium_texture = resources->texture("podium1");

    shader->use();

    glm::mat4 view =
        glm::lookAt(
            glm::vec3(0.0f, 3.0f, 11.0f),
            glm::vec3(0.0f, 0.8f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

    glm::mat4 projection =
        glm::perspective(
            glm::radians(45.0f),
            1280.0f / 720.0f,
            0.1f,
            100.0f
        );

    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), position);

    shader->set_mat4("model", model);
    shader->set_mat4("view", view);
    shader->set_mat4("projection", projection);
    shader->set_int("texture1", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(
        GL_TEXTURE_2D,
        podium_texture->id()
    );

    glBindVertexArray(m_podium_vao);

    glDrawArrays(
        GL_TRIANGLES,
        0,
        m_podium_vertex_count
    );

    glBindVertexArray(0);
}

void MainController::draw() {
    // Osiguravamo da se crta preko celog prozora
    glViewport(0, 0, 1280, 720);

    // Isključujemo culling da zidovi/pod ne bi bili odbačeni
    glDisable(GL_CULL_FACE);

    // Uključujemo depth test
    glEnable(GL_DEPTH_TEST);

    glClearColor(
    0.15f,
    0.16f,
    0.21f,
    1.0f
    );
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );

    auto resources =
        engine::core::Controller::get<
            engine::resources::ResourcesController
        >();

    auto floor_texture =
        resources->texture("tiles");

    auto wall_texture =
        resources->texture("zidovi");

    // -------------------------
    // POD
    // -------------------------

    draw_textured_object(
        m_floor_vao,
        6,
        floor_texture->id(),
        glm::mat4(1.0f)
    );

    // -------------------------
    // ZIDOVI
    // -------------------------

    draw_textured_object(
        m_back_wall_vao,
        6,
        wall_texture->id(),
        glm::mat4(1.0f)
    );

    draw_textured_object(
        m_left_wall_vao,
        6,
        wall_texture->id(),
        glm::mat4(1.0f)
    );

    draw_textured_object(
        m_right_wall_vao,
        6,
        wall_texture->id(),
        glm::mat4(1.0f)
    );

    // -------------------------
    // 5 PODIJUMA
    // -------------------------

    const glm::vec3 podium_positions[3] = {
        // GLAVNI AUTO - centar
        glm::vec3(
            0.0f,
            0.0f,
            0.0f
        ),

        // AUTO LEVO POZADI
        glm::vec3(
            -9.5f,
            0.0f,
            -8.0f
        ),

        // AUTO DESNO POZADI
        glm::vec3(
            9.5f,
            0.0f,
            -8.0f
        )
    };

    for (const auto &position : podium_positions) {
        draw_podium(position);
    }

    // -------------------------
    // PORSCHE
    // -------------------------

    auto porsche =
        resources->model("porsche");

    auto porsche_shader =
        resources->shader("porsche");

    porsche_shader->use();

    glm::mat4 view =
        glm::lookAt(
    glm::vec3(
        0.0f,
        3.0f,
        11.0f
    ),
    glm::vec3(
        0.0f,
        0.8f,
        0.0f
    ),
    glm::vec3(
        0.0f,
        1.0f,
        0.0f
    )
);

    glm::mat4 projection =
        glm::perspective(
            glm::radians(50.0f),
            1280.0f / 720.0f,
            0.1f,
            100.0f
        );

    glm::mat4 porsche_model =
        glm::mat4(1.0f);

    porsche_model =
        glm::translate(
            porsche_model,
            glm::vec3(0.0f, -0.25f, 0.0f)
        );

    porsche_model =
        glm::rotate(
            porsche_model,
            m_time,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

    porsche_model =
        glm::scale(
            porsche_model,
            glm::vec3(1.25f)
        );

    porsche_shader->set_mat4(
        "model",
        porsche_model
    );

    porsche_shader->set_mat4(
        "view",
        view
    );

    porsche_shader->set_mat4(
        "projection",
        projection
    );

    porsche_shader->set_vec3(
        "lightPos",
        glm::vec3(4.0f, 6.0f, 4.0f)
    );

    porsche_shader->set_vec3(
        "viewPos",
        glm::vec3(0.0f, 3.0f, 11.0f)
    );

    porsche->draw(porsche_shader);
}

void MainController::end_draw() {
    engine::core::Controller::get<
        engine::platform::PlatformController
    >()->swap_buffers();
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

    glDeleteVertexArrays(1, &m_podium_vao);
    glDeleteBuffers(1, &m_podium_vbo);

    glDeleteVertexArrays(1, &m_cube_vao);
    glDeleteBuffers(1, &m_cube_vbo);
}