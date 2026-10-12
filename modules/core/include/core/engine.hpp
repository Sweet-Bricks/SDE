#pragma once

#include "core/system.hpp"
#include "SDL3/SDL_init.h"

#include <memory>
#include <vector>

namespace sde
{
class Engine
{
public:
    Engine(int, char**);

    auto initialize() -> SDL_AppResult;
    auto handle_event(SDL_Event*) -> SDL_AppResult;
    auto update() -> SDL_AppResult;
    void quit(SDL_AppResult);

    void register_system(ISystem*);

private:
    std::vector<std::unique_ptr<ISystem>> systems;
};
} // namespace sde
