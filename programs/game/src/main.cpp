#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

#include <cmath>
#include <print>

namespace
{
SDL_Window* window{};
SDL_Renderer* renderer{};
} // namespace

SDL_AppResult SDL_AppInit(void**, int, char**)
{
    SDL_SetAppMetadata("Window", "1.0", "Window_1_0");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::print("Init failure : {}", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Window", 1366, 768, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        std::print("Window creation failure : {}", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, 1366, 768, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void*, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void*)
{
    const auto now = static_cast<float>(SDL_GetTicks()) / 1000.0F;
    const auto r = 0.5F + (0.5F * std::sinf(now));
    const auto g = 0.5F + (0.5F * std::sinf(now + (3.14F * 2 / 3.F)));
    const auto b = 0.5F + (0.5F * std::sinf(now + (3.14F * 4 / 3.F)));

    SDL_SetRenderDrawColorFloat(renderer, r, g, b, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void*, SDL_AppResult) {}
