#pragma once

namespace sde
{
class ISystem
{
public:
    virtual void setup() = 0;
    virtual void update() = 0;
    virtual ~ISystem();
};
} // namespace sde
