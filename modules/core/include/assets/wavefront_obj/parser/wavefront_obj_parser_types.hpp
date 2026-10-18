#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>
#include <variant>

namespace sde
{
struct WavefrontOBJToken
{
    enum class TokenKind : std::uint8_t
    {
        GEOMETRIC_VERTEX, // v

        TEXTURE_VERTEX, // vt

        VERTEX_NORMAL, // vn

        FACE, // f

        /**
            Entire face-vertex reference is treated as one token, for example
            Input: f 1/2/3 4//5 6/7/8
            Tokens:
                FACE
                FACE_REFERENCE("1/2/3")
                FACE_REFERENCE("4//5")
                FACE_REFERENCE("6/7/8")
                END_OF_LINE
        */
        FACE_REFERENCE,

        INTEGER, // 1, -2, +3
        FLOAT,   // 1.5, -0.5, 1E3

        /**
         * Other OBJ keywords, that is not yet supported by this glorious parser (vp, bmat, curv, surf ...)
         */
        UNSUPPORTED_IDENTIFIER,
        END_OF_LINE,
        END_OF_FILE,

        COUNT,
    };

    struct Face
    {
        std::int64_t vertex_index{};
        std::optional<std::int64_t> texture_vertex_index{};
        std::optional<std::int64_t> vertex_normal_index{};
    };

    using TokenValue = std::variant<std::monostate, std::int64_t, double, Face>;

    TokenKind kind{};
    TokenValue value{};

    std::string_view lexeme{};
    std::size_t line{};
    std::size_t column{};
};
} // namespace sde
