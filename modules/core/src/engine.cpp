#include "core/engine.hpp"

#include "core/app_entry.hpp"

#include <SDL3/SDL_init.h>

namespace sde
{
Engine::Engine(int, char**)
{
}

auto Engine::initialize() -> SDL_AppResult
{
    application_init(this);
    for (auto& sys : systems)
    {
        sys->setup();
    }
    return SDL_APP_CONTINUE;
}

auto Engine::handle_event(SDL_Event* event) -> SDL_AppResult
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

auto Engine::update() -> SDL_AppResult
{
    for (auto& sys : systems)
    {
        sys->update();
    }
    return SDL_APP_CONTINUE;
}

void Engine::quit(SDL_AppResult) {}

void Engine::register_system(ISystem* sys)
{
    systems.emplace_back(sys);
}
} // namespace sde
