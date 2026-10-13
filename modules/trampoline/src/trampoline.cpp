#define SDL_MAIN_USE_CALLBACKS 1
#include "trampoline/app.hpp"

#include <core/engine.hpp>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#include <cstddef>
#include <span>

auto SDL_AppInit(void** appstate, int argc, char** argv) -> SDL_AppResult
{
    auto* engine = new sde::Engine(std::span<char*>(argv, static_cast<size_t>(argc)));
    *appstate = engine;
    sde::app_init(*engine);
    return engine->initialize();
}

auto SDL_AppEvent(void* appstate, SDL_Event* event) -> SDL_AppResult
{
    return static_cast<sde::Engine*>(appstate)->handle_event(event);
}

auto SDL_AppIterate(void* appstate) -> SDL_AppResult
{
    return static_cast<sde::Engine*>(appstate)->update();
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    auto* engine = static_cast<sde::Engine*>(appstate);
    engine->quit(result);
    delete engine;
}
