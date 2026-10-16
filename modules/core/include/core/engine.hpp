#pragma once

#include "core/api.hpp"
#include "core/resource_provider.hpp"
#include "core/system.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

#include <memory>
#include <span>
#include <vector>

namespace sde
{
class SDE_CORE_API Engine final
{
public:
    explicit Engine(std::span<char*> = {});
    Engine(const Engine&) = delete;
    auto operator=(const Engine&) -> Engine& = delete;

    auto initialize() -> SDL_AppResult;
    auto handle_event(SDL_Event*) -> SDL_AppResult;
    auto update() -> SDL_AppResult;
    void quit(SDL_AppResult);

    void register_system(ISystem*);

    template <class T>
    auto get() [[clang::lifetimebound]] -> T*;

private:
    std::unique_ptr<ResourceProvider> res_provider;
    std::vector<std::unique_ptr<ISystem>> systems;
};

template <class T>
inline auto Engine::get() -> T*
{
    static_assert(false);
    return nullptr;
}

template <>
inline auto Engine::get() -> ResourceProvider*
{
    return res_provider.get();
}
} // namespace sde
