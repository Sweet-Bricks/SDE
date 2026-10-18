#pragma once

#include "assets/wavefront_obj/parser/wavefront_obj_parser_errors.hpp"
#include "assets/wavefront_obj/parser/wavefront_obj_parser_types.hpp"
#include "assets/wavefront_obj/wavefront_obj_asset.hpp"
#include "core/api.hpp"

#include <cstddef>
#include <expected>
#include <string_view>

namespace sde
{
class SDE_CORE_API WavefrontOBJParser
{
public:
    static auto parse_wavefront_obj(std::string_view source) -> std::expected<WavefrontOBJAsset, WavefrontOBJError>;

    explicit WavefrontOBJParser([[clang::lifetimebound]] std::string_view);

    auto next() -> std::expected<WavefrontOBJToken, WavefrontOBJTokenizeError>;

private:
    std::string_view source;

    std::size_t position = 0z;
    std::size_t line = 1z;
    std::size_t column = 1z;

    bool unterminated_line = false;
};
} // namespace sde
