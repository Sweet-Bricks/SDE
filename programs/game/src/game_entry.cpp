#include "game/game_system.hpp"
#include <core/app_entry.hpp>
#include <core/engine.hpp>

void application_init(sde::Engine* engine)
{
    engine->register_system(new GameSystem());
}
