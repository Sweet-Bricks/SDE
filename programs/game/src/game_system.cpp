#include "game/game_system.hpp"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

#include <cmath>
#include <format>
#include <stdexcept>

namespace
{
SDL_Window* window{};
SDL_Renderer* renderer{};
} // namespace

void GameSystem::setup()
{
    SDL_SetAppMetadata("Window", "1.0", "Window_1_0");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        throw std::runtime_error(std::format("Init failure : {}", SDL_GetError()));
    }

    if (!SDL_CreateWindowAndRenderer("Window", 1366, 768, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        throw std::runtime_error(std::format("Window creation failure : {}", SDL_GetError()));
    }

    SDL_SetRenderLogicalPresentation(renderer, 1366, 768, SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

void GameSystem::update()
{
    const auto now = static_cast<float>(SDL_GetTicks()) / 1000.0F;
    const auto r = 0.5F + (0.5F * std::sinf(now));
    const auto g = 0.5F + (0.5F * std::sinf(now + (3.14F * 2 / 3.F)));
    const auto b = 0.5F + (0.5F * std::sinf(now + (3.14F * 4 / 3.F)));

    SDL_SetRenderDrawColorFloat(renderer, r, g, b, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
