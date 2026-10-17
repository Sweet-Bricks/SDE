#pragma once

#include "core/api.hpp"
#include "core/resource_path.hpp"

#include <expected>
#include <filesystem>
#include <string>
#include <unordered_map>

namespace sde
{
class SDE_CORE_API ResourceProvider final
{
public:
    ResourceProvider();
    auto resolve_resource(const ResourcePath&) -> std::expected<std::filesystem::path, std::string>;

private:
    std::unordered_map<std::string, std::filesystem::path> mount_points;
};
} // namespace sde
