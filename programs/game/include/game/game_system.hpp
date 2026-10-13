#pragma once

#include <core/system.hpp>

class GameSystem : public sde::ISystem
{
public:
    void setup() override;
    void update() override;
};
