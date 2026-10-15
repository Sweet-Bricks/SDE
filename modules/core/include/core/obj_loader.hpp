#pragma once

#include "core/api.hpp"

#include <expected>
#include <filesystem>
#include <string>

namespace sde
{
class SDE_CORE_API ObjData
{
};

auto SDE_CORE_API read_obj_file(const std::filesystem::path& file_path) -> std::expected<ObjData, std::string>;
} // namespace sde
