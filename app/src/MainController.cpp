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
#include <array>
#include <cmath>
#include <stdexcept>

namespace {

constexpr float PI = 3.14159265359f;

const float FLOOR_VERTICES[] = {
    15.0f, -0.5f,  15.0f, 15.0f,  0.0f,
   -15.0f, -0.5f,  15.0f,  0.0f,  0.0f,
   -15.0f, -0.5f, -15.0f,  0.0f, 15.0f,
    15.0f, -0.5f,  15.0f, 15.0f,  0.0f,
   -15.0f, -0.5f, -15.0f,  0.0f, 15.0f,
    15.0f, -0.5f, -15.0f, 15.0f, 15.0f
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
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
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

namespace {

 glm::vec3 camera_front(float yaw, float pitch) {
    glm::vec3 front;
    front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    front.y = std::sin(glm::radians(pitch));
    front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    return glm::normalize(front);
}

}

void MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    create_floor();
    create_walls();
    create_podium();
    create_cube();
    create_framebuffer();
    create_point_shadow_buffer();
    create_screen_quad();
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

        m_podium_vertices.insert(m_podium_vertices.end(), {0.0f, top_y, 0.0f, 0.5f, 0.5f});
        m_podium_vertices.insert(m_podium_vertices.end(), {x1, top_y, z1, 0.5f + 0.5f * x1 / radius, 0.5f + 0.5f * z1 / radius});
        m_podium_vertices.insert(m_podium_vertices.end(), {x2, top_y, z2, 0.5f + 0.5f * x2 / radius, 0.5f + 0.5f * z2 / radius});
        m_podium_vertices.insert(m_podium_vertices.end(), {x1, top_y, z1, u1, 1.0f});
        m_podium_vertices.insert(m_podium_vertices.end(), {x1, base_y, z1, u1, 0.0f});
        m_podium_vertices.insert(m_podium_vertices.end(), {x2, base_y, z2, u2, 0.0f});
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

void MainController::create_framebuffer() {
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    glGenTextures(1, &m_colorTexture);
    glBindTexture(GL_TEXTURE_2D, m_colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
                 m_framebufferWidth, m_framebufferHeight,
                 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_colorTexture, 0);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glGenRenderbuffers(1, &m_depthStencilRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                           m_framebufferWidth, m_framebufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, m_depthStencilRbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Framebuffer nije kompletan.");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MainController::create_point_shadow_buffer() {
    glGenFramebuffers(1, &m_shadowFbo);
    glGenTextures(1, &m_depthCubemap);

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthCubemap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32F,
                     m_shadowSize, m_shadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Point shadow framebuffer nije kompletan.");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MainController::create_screen_quad() {
    const float quad[] = {
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &m_screen_vao);
    glGenBuffers(1, &m_screen_vbo);
    glBindVertexArray(m_screen_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_screen_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void MainController::draw_screen_quad() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto shader = resources->shader("postprocess");
    shader->use();
    shader->set_int("screenTexture", 0);
    shader->set_int("enabled", m_postProcessEnabled ? 1 : 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_colorTexture);

    glBindVertexArray(m_screen_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void MainController::draw_textured_object(unsigned int vao, int vertex_count, unsigned int texture_id, const glm::mat4 &model) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto shader = resources->shader("showroom");
    shader->use();

    const glm::vec3 front = camera_front(m_cameraYaw, m_cameraPitch);
    const glm::mat4 view = glm::lookAt(m_cameraPos, m_cameraPos + front, glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                   static_cast<float>(m_framebufferWidth) / static_cast<float>(m_framebufferHeight),
                                                   0.1f, 100.0f);

    shader->set_mat4("model", model);
    shader->set_mat4("view", view);
    shader->set_mat4("projection", projection);
    shader->set_vec3("lightPos", m_pointLightPos);
    shader->set_float("farPlane", m_shadowFarPlane);
    shader->set_int("shadowsEnabled", (m_pointShadowsEnabled && m_pointLightEnabled) ? 1 : 0);
    shader->set_int("shadowMap", 5);
    shader->set_int("texture1", 0);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthCubemap);
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

    const glm::vec3 front = camera_front(m_cameraYaw, m_cameraPitch);
    const glm::mat4 view = glm::lookAt(m_cameraPos, m_cameraPos + front, glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                   static_cast<float>(m_framebufferWidth) / static_cast<float>(m_framebufferHeight),
                                                   0.1f, 100.0f);
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

void MainController::draw_depth_object(unsigned int vao, int vertex_count, const glm::mat4 &model) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto shader = resources->shader("point_shadow");
    shader->set_mat4("model", model);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    glBindVertexArray(0);
}

void MainController::draw_depth_scene(const glm::mat4 shadowMatrices[6]) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto shader = resources->shader("point_shadow");
    auto porsche = resources->model("porsche");
    auto bugatti = resources->model("bugatti");
    auto lexus = resources->model("lexus");

    shader->use();
    for (int i = 0; i < 6; ++i)
        shader->set_mat4("shadowMatrices[" + std::to_string(i) + "]", shadowMatrices[i]);
    shader->set_vec3("lightPos", m_pointLightPos);
    shader->set_float("farPlane", m_shadowFarPlane);

    draw_depth_object(m_floor_vao, 6, glm::mat4(1.0f));
    draw_depth_object(m_back_wall_vao, 6, glm::mat4(1.0f));
    draw_depth_object(m_left_wall_vao, 6, glm::mat4(1.0f));
    draw_depth_object(m_right_wall_vao, 6, glm::mat4(1.0f));

    const glm::vec3 podium_positions[3] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(-6.5f, 0.0f, -4.0f),
        glm::vec3(6.5f, 0.0f, -4.0f)
    };
    for (const auto &position : podium_positions)
        draw_depth_object(m_podium_vao, m_podium_vertex_count, glm::translate(glm::mat4(1.0f), position));

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.45f, 0.0f));
    model = glm::rotate(model, m_time, glm::vec3(0.0f, 1.0f, 0.0f));
    shader->set_mat4("model", model);
    porsche->draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.5f, 0.35f, -4.0f));
    model = glm::rotate(model, m_time, glm::vec3(0.0f, 1.0f, 0.0f));
    shader->set_mat4("model", model);
    bugatti->draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(6.5f, -0.45f, -4.0f));
    model = glm::rotate(model, m_time, glm::vec3(0.0f, 1.0f, 0.0f));
    shader->set_mat4("model", model);
    lexus->draw(shader);
}

void MainController::update() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    const float dt = platform->dt();

    // Kamera: W/S napred-nazad, A/D levo-desno, PageUp/PageDown gore-dole, strelice za pogled.
    glm::vec3 front = camera_front(m_cameraYaw, m_cameraPitch);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) m_cameraPos += front * m_cameraMoveSpeed * dt;
    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) m_cameraPos -= front * m_cameraMoveSpeed * dt;
    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) m_cameraPos -= right * m_cameraMoveSpeed * dt;
    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) m_cameraPos += right * m_cameraMoveSpeed * dt;
    if (platform->key(engine::platform::KeyId::KEY_PAGE_UP).is_down()) m_cameraPos.y += m_cameraMoveSpeed * dt;
    if (platform->key(engine::platform::KeyId::KEY_PAGE_DOWN).is_down()) m_cameraPos.y -= m_cameraMoveSpeed * dt;

    if (platform->key(engine::platform::KeyId::KEY_LEFT).is_down()) m_cameraYaw -= m_cameraLookSpeed * dt;
    if (platform->key(engine::platform::KeyId::KEY_RIGHT).is_down()) m_cameraYaw += m_cameraLookSpeed * dt;
    if (platform->key(engine::platform::KeyId::KEY_UP).is_down()) m_cameraPitch += m_cameraLookSpeed * dt;
    if (platform->key(engine::platform::KeyId::KEY_DOWN).is_down()) m_cameraPitch -= m_cameraLookSpeed * dt;
    m_cameraPitch = glm::clamp(m_cameraPitch, -85.0f, 85.0f);

    m_time += dt * m_porscheSpeed;

    if (platform->key(engine::platform::KeyId::KEY_1).state() == engine::platform::Key::State::JustPressed)
        m_pointLightEnabled = !m_pointLightEnabled;
    if (platform->key(engine::platform::KeyId::KEY_2).state() == engine::platform::Key::State::JustPressed)
        m_directionalLightEnabled = !m_directionalLightEnabled;
    if (platform->key(engine::platform::KeyId::KEY_J).is_down()) m_pointLightPos.x -= 3.0f * dt;
    if (platform->key(engine::platform::KeyId::KEY_L).is_down()) m_pointLightPos.x += 3.0f * dt;
    if (platform->key(engine::platform::KeyId::KEY_I).is_down()) m_pointLightPos.z -= 3.0f * dt;
    if (platform->key(engine::platform::KeyId::KEY_K).is_down()) m_pointLightPos.z += 3.0f * dt;
    if (platform->key(engine::platform::KeyId::KEY_U).is_down()) m_pointLightIntensity = glm::min(m_pointLightIntensity + 0.8f * dt, 4.0f);
    if (platform->key(engine::platform::KeyId::KEY_O).is_down()) m_pointLightIntensity = glm::max(m_pointLightIntensity - 0.8f * dt, 0.0f);

    if (platform->key(engine::platform::KeyId::KEY_F).state() == engine::platform::Key::State::JustPressed)
        m_postProcessEnabled = !m_postProcessEnabled;
    if (platform->key(engine::platform::KeyId::KEY_H).state() == engine::platform::Key::State::JustPressed)
        m_pointShadowsEnabled = !m_pointShadowsEnabled;

    if (platform->key(engine::platform::KeyId::KEY_E).state() == engine::platform::Key::State::JustPressed) {
        m_eventStage = 1;
        m_eventTimer = 0.0f;
        m_pointLightColor = glm::vec3(1.0f, 0.85f, 0.65f);
        m_porscheSpeed = 1.0f;
    }
    if (m_eventStage == 1 || m_eventStage == 2) m_eventTimer += dt;
    if (m_eventStage == 1 && m_eventTimer >= 2.0f) {
        m_pointLightColor = glm::vec3(1.0f, 0.08f, 0.03f);
        m_eventStage = 2;
        m_eventTimer = 0.0f;
    }
    if (m_eventStage == 2 && m_eventTimer >= 2.0f) {
        m_porscheSpeed = 2.8f;
        m_eventStage = 3;
    }
}

void MainController::draw() {
    const glm::vec3 front = camera_front(m_cameraYaw, m_cameraPitch);
    const glm::mat4 view = glm::lookAt(m_cameraPos, m_cameraPos + front, glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                   static_cast<float>(m_framebufferWidth) / static_cast<float>(m_framebufferHeight),
                                                   0.1f, 100.0f);
    glViewport(0, 0, 1280, 720);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // ------------------------------------------------------------
    // 1) POINT SHADOW CUBEMAP
    // ------------------------------------------------------------
    if (m_pointShadowsEnabled && m_pointLightEnabled) {
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, m_shadowNearPlane, m_shadowFarPlane);
        const glm::vec3 p = m_pointLightPos;
        glm::mat4 shadowMatrices[6] = {
            shadowProj * glm::lookAt(p, p + glm::vec3( 1, 0, 0), glm::vec3(0,-1,0)),
            shadowProj * glm::lookAt(p, p + glm::vec3(-1, 0, 0), glm::vec3(0,-1,0)),
            shadowProj * glm::lookAt(p, p + glm::vec3( 0, 1, 0), glm::vec3(0, 0,1)),
            shadowProj * glm::lookAt(p, p + glm::vec3( 0,-1, 0), glm::vec3(0, 0,-1)),
            shadowProj * glm::lookAt(p, p + glm::vec3( 0, 0, 1), glm::vec3(0,-1,0)),
            shadowProj * glm::lookAt(p, p + glm::vec3( 0, 0,-1), glm::vec3(0,-1,0))
        };

        glViewport(0, 0, m_shadowSize, m_shadowSize);
        glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        draw_depth_scene(shadowMatrices);
        glCullFace(GL_BACK);
        glDisable(GL_CULL_FACE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // ------------------------------------------------------------
    // 2) MAIN HDR SCENE + BRIGHT PASS
    // ------------------------------------------------------------
    glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glClearColor(0.15f, 0.16f, 0.21f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto floor_texture = resources->texture("tiles");
    auto wall_texture = resources->texture("zidovi");

    draw_textured_object(m_floor_vao, 6, floor_texture->id(), glm::mat4(1.0f));
    draw_textured_object(m_back_wall_vao, 6, wall_texture->id(), glm::mat4(1.0f));
    draw_textured_object(m_left_wall_vao, 6, wall_texture->id(), glm::mat4(1.0f));
    draw_textured_object(m_right_wall_vao, 6, wall_texture->id(), glm::mat4(1.0f));

    const glm::vec3 podium_positions[3] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(-6.5f, 0.0f, -4.0f),
        glm::vec3(6.5f, 0.0f, -4.0f)
    };
    for (const auto &position : podium_positions) draw_podium(position);

    auto porsche = resources->model("porsche");
    auto bugatti = resources->model("bugatti");
    auto lexus = resources->model("lexus");
    auto car_shader = resources->shader("porsche");
    car_shader->use();
    car_shader->set_mat4("view", view);
    car_shader->set_mat4("projection", projection);
    car_shader->set_vec3("lightPos", m_pointLightPos);
    car_shader->set_vec3("viewPos", glm::vec3(0.0f, 3.0f, 11.0f));
    car_shader->set_vec3("pointLightColor", m_pointLightColor);
    car_shader->set_float("pointLightIntensity", m_pointLightIntensity);
    car_shader->set_int("pointLightEnabled", m_pointLightEnabled ? 1 : 0);
    car_shader->set_vec3("directionalLightDirection", m_directionalLightDir);
    car_shader->set_vec3("directionalLightColor", m_directionalLightColor);
    car_shader->set_float("directionalLightIntensity", m_directionalLightIntensity);
    car_shader->set_int("directionalLightEnabled", m_directionalLightEnabled ? 1 : 0);
    car_shader->set_int("shadowMap", 5);
    car_shader->set_float("farPlane", m_shadowFarPlane);
    car_shader->set_int("shadowsEnabled", (m_pointShadowsEnabled && m_pointLightEnabled) ? 1 : 0);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthCubemap);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.45f, 0.0f));
    model = glm::rotate(model, m_time, glm::vec3(0.0f, 1.0f, 0.0f));
    car_shader->set_mat4("model", model);
    porsche->draw(car_shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.5f, 0.35f, -4.0f));
    model = glm::rotate(model, m_time, glm::vec3(0.0f, 1.0f, 0.0f));
    car_shader->set_mat4("model", model);
    bugatti->draw(car_shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(6.5f, -0.45f, -4.0f));
    model = glm::rotate(model, m_time, glm::vec3(0.0f, 1.0f, 0.0f));
    car_shader->set_mat4("model", model);
    lexus->draw(car_shader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ------------------------------------------------------------
    // 3) FINAL POST-PROCESSING
    // ------------------------------------------------------------
    glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);
    glDisable(GL_DEPTH_TEST);
    draw_screen_quad();
    glEnable(GL_DEPTH_TEST);
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
    glDeleteVertexArrays(1, &m_podium_vao);
    glDeleteBuffers(1, &m_podium_vbo);
    glDeleteVertexArrays(1, &m_cube_vao);
    glDeleteBuffers(1, &m_cube_vbo);

    glDeleteFramebuffers(1, &m_framebuffer);
    glDeleteTextures(1, &m_colorTexture);
    glDeleteRenderbuffers(1, &m_depthStencilRbo);

    glDeleteFramebuffers(1, &m_shadowFbo);
    glDeleteTextures(1, &m_depthCubemap);

    glDeleteVertexArrays(1, &m_screen_vao);
    glDeleteBuffers(1, &m_screen_vbo);
}
