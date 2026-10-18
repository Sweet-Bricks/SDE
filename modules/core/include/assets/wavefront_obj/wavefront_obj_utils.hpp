#pragma once

#include "assets/wavefront_obj/parser/wavefront_obj_parser_errors.hpp"
#include "assets/wavefront_obj/wavefront_obj_asset.hpp"
#include "file_system/file_system_errors.hpp"

#include <expected>
#include <filesystem>
#include <variant>

namespace sde::wavefront_obj_utils
{
using WavefrontOBJLoadError = std::variant<FileErrc, WavefrontOBJError>;

auto load_wavefront_obj_from_file(const std::filesystem::path& file_path) -> std::expected<WavefrontOBJAsset, WavefrontOBJLoadError>;
} // namespace sde::wavefront_obj_utils
