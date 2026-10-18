#pragma once

#include "core/api.hpp"
#include "core/render/obj_data.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

#include <expected>
#include <filesystem>
#include <string>
#include <string_view>

namespace sde
{
auto SDE_CORE_API parse_obj(const std::string_view text) -> std::expected<ObjData, std::string>;
auto SDE_CORE_API parse_obj_file(const std::filesystem::path& file_path) -> std::expected<ObjData, std::string>;
} // namespace sde
