#include <MainController.hpp>

#include "../../engine/libs/spdlog/include/spdlog/spdlog.h"

#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <imgui_impl_opengl3_loader.h>
#include <spdlog/spdlog.h>

namespace app {

    class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
    public:
        void on_mouse_move(engine::platform::MousePosition position) override;
};

    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx,position.dy);
    }

    void MainController::initialize() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();;
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
        engine::graphics::OpenGL::enable_depth_testing();
        spdlog::info(("MainController::initialize"));
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }



    void MainController::draw_sun() {
        //Model
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        //Shader
        engine::resources::Shader * shader = resources->shader("planeta");
        shader->use();

        shader->set_mat4("projection",graphics->projection_matrix());
        shader->set_mat4("view",graphics->camera()->view_matrix());

        //sunce
        engine::resources::Model* sun = resources->model("sun");
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.2f, 0.0f, -5.0f));
        model = glm::scale(model, glm::vec3(0.08f));
        shader->set_mat4("model", model);
        sun->draw(shader);

        //merkur
        engine::resources::Model* merkur = resources->model("merkur");
        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model1, glm::vec3(-1.7f, 0.0f, -5.0f));
        model1 = glm::scale(model1, glm::vec3(0.0006f));
        shader->set_mat4("model", model1);
        merkur->draw(shader);

        //venera
        engine::resources::Model* venera = resources->model("venera");
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(-1.0f, 0.0f, -5.0f));
        model2 = glm::scale(model2, glm::vec3(0.085f));
        shader->set_mat4("model", model2);
        venera->draw(shader);

        //zemlja
        engine::resources::Model* zemlja = resources->model("zemlja");
        glm::mat4 model3 = glm::mat4(1.0f);
        model3 = glm::translate(model3, glm::vec3(-0.3f, 0.0f, -5.0f));
        model3 = glm::scale(model3, glm::vec3(0.08f));
        shader->set_mat4("model", model3);
        zemlja->draw(shader);

        //mars
        engine::resources::Model* mars = resources->model("mars");
        glm::mat4 model4 = glm::mat4(1.0f);
        model4 = glm::translate(model4, glm::vec3(0.4f, 0.0f, -5.0f));
        model4 = glm::scale(model4, glm::vec3(0.05f));
        shader->set_mat4("model", model4);
        mars->draw(shader);

        //jupiter
        engine::resources::Model* jupiter = resources->model("jupiter");
        glm::mat4 model5 = glm::mat4(1.0f);
        model5 = glm::translate(model5, glm::vec3(1.1f, 0.0f, -5.0f));
        model5 = glm::scale(model5, glm::vec3(0.2f));
        shader->set_mat4("model", model5);
        jupiter->draw(shader);

        //saturn
        engine::resources::Model* saturn = resources->model("saturn");
        glm::mat4 model6 = glm::mat4(1.0f);
        model6 = glm::translate(model6, glm::vec3(1.8f, 0.0f, -5.0f));
        model6 = glm::scale(model6, glm::vec3(0.1f));
        shader->set_mat4("model", model6);
        saturn->draw(shader);

        //uran
        engine::resources::Model* uran = resources->model("uran");
        glm::mat4 model7 = glm::mat4(1.0f);
        model7 = glm::translate(model7, glm::vec3(2.5f, 0.0f, -5.0f));
        model7 = glm::scale(model7, glm::vec3(0.018f));
        shader->set_mat4("model", model7);
        uran->draw(shader);

        //neptun
        engine::resources::Model* neptun = resources->model("neptun");
        glm::mat4 model8 = glm::mat4(1.0f);
        model8 = glm::translate(model8, glm::vec3(3.2f, 0.0f, -5.0f));
        model8 = glm::scale(model8, glm::vec3(0.1f));
        shader->set_mat4("model", model8);
        neptun->draw(shader);



    }

    void MainController::update_camera() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();
        float dt = platform->dt();
        if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD,dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD,dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT,dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT,dt);
        }


    }

    void MainController::update() {
        update_camera();
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::draw() {
        draw_sun();
    }

    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }


}// namespace app