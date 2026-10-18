#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <variant>

namespace sde
{
struct WavefrontOBJParseError
{
    enum class Errc : std::uint8_t
    {
        INVALID_ACTION_TOKEN,
        NON_FINITE_COORDINATE,
        COORDINATE_OUT_OF_RANGE,
        INVALID_VERTEX_INDEX,
        INVALID_TEXTURE_INDEX,
        INVALID_NORMAL_INDEX,
    };

    Errc code{};
    std::size_t line{};
    std::size_t column{};
};

struct WavefrontOBJTokenizeError
{
    enum class Errc : std::uint8_t
    {
        INVALID_NUMBER,
        NUMBER_OUT_OF_RANGE,
        INVALID_FACE_REFERENCE,
    };

    Errc code{};
    std::string_view lexeme{};
    std::size_t line{};
    std::size_t column{};
};

struct WavefrontObjSyntaxError
{
    enum class Errc : std::uint8_t
    {
        UNEXPECTED_TOKEN,
        UNEXPECTED_EOF,
    };

    Errc code{};
    std::size_t line{};
    std::size_t column{};
};

using WavefrontOBJError = std::variant<WavefrontOBJTokenizeError, WavefrontOBJParseError, WavefrontObjSyntaxError>;
} // namespace sde
