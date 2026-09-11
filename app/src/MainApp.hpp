#ifndef MAIN_APP_HPP
#define MAIN_APP_HPP

#include <engine/core/App.hpp>

class MainApp final : public engine::core::App {
protected:
    void app_setup() override;
};

#endif // MAIN_APP_HPP