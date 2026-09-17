#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <string_view>

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
    void draw() override;
    void end_draw() override;
};

}

#endif // MAINCONTROLLER_HPP