#pragma once

#include "core/api.hpp"

namespace sde
{
class SDE_CORE_API ISystem
{
public:
    ISystem() = default;
    ISystem(const ISystem&) = delete;
    auto operator=(const ISystem&) -> ISystem& = delete;

    virtual void setup() = 0;
    virtual void update() = 0;
    virtual ~ISystem();
};
} // namespace sde
