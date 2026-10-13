#include "game/game_system.hpp"

#include <core/engine.hpp>
#include <trampoline/app.hpp>

void sde::app_init(sde::Engine& engine)
{
    engine.register_system(new GameSystem());
}
