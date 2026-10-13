#pragma once

#include "core/api.hpp"
#include "core/system.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

#include <memory>
#include <span>
#include <vector>

namespace sde
{
class SDE_CORE_API Engine
{
public:
    explicit Engine(std::span<char*>);
    Engine(const Engine&) = delete;
    auto operator=(const Engine&) -> Engine& = delete;

    auto initialize() -> SDL_AppResult;
    auto handle_event(SDL_Event*) -> SDL_AppResult;
    auto update() -> SDL_AppResult;
    void quit(SDL_AppResult);

    void register_system(ISystem*);

private:
    std::vector<std::unique_ptr<ISystem>> systems;
};
} // namespace sde
