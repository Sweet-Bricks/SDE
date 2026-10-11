#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <memory>

namespace {

struct App {
    SDL_Window* window = nullptr;
};

} // namespace

SDL_AppResult SDL_AppInit(void** appstate, int /*argc*/, char** /*argv*/) {
    auto app = std::make_unique<App>();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_APP_FAILURE;
    }

    app->window = SDL_CreateWindow("Game", 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);

    if (app->window == nullptr) {
        SDL_Log("SDL_CreateWindow: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    *appstate = app.release(); // owned by SDL until SDL_AppQuit
    return SDL_APP_CONTINUE;
}

// NOLINTNEXTLINE(misc-const-correctness): signature is fixed by SDL
SDL_AppResult SDL_AppEvent(void* /*appstate*/, SDL_Event* event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;

        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_ESCAPE) {
                return SDL_APP_SUCCESS;
            }
            break;

        default:
            break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* /*appstate*/) {
    SDL_Delay(16);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult /*result*/) {
    const std::unique_ptr<App> app{static_cast<App*>(appstate)};

    if (app != nullptr) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}
