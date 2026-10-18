#pragma once

#include <cstdint>

namespace sde
{
enum class FileErrc : std::uint8_t
{
    OPEN_FAILED,
    READ_FAILED,
};
} // namespace sde
