#include "core/engine.hpp"

#include "core/resource_provider.hpp"
#include "core/system.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

#include <algorithm>
#include <memory>
#include <span>
#include <vector>

namespace sde
{
Engine::Engine(std::span<char*>)
{
}

auto Engine::initialize() -> SDL_AppResult
{
    res_provider = std::make_unique<ResourceProvider>();
    std::ranges::for_each(systems, [](auto& sys) -> auto {
        sys->setup();
    });
    return SDL_APP_CONTINUE;
}

auto Engine::handle_event(SDL_Event* event) -> SDL_AppResult
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

auto Engine::update() -> SDL_AppResult
{
    std::ranges::for_each(systems, [](auto& sys) -> auto {
        sys->update();
    });
    return SDL_APP_CONTINUE;
}

void Engine::quit(SDL_AppResult) {}

void Engine::register_system(ISystem* sys)
{
    systems.emplace_back(sys);
}
} // namespace sde
