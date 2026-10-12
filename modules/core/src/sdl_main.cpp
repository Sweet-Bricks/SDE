#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL_main.h>

#include "core/engine.hpp"

auto SDL_AppInit(void** appstate, int argc, char** argv) -> SDL_AppResult
{
    auto* engine = new sde::Engine(argc, argv);
    *appstate = engine;
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
