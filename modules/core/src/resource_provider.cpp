#include "core/resource_provider.hpp"

#include "core/resource_path.hpp"

#include <SDL3/SDL_filesystem.h>

#include <cassert>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>

namespace sde
{
ResourceProvider::ResourceProvider()
{
    auto exe_dir = std::filesystem::path(SDL_GetBasePath());
    auto mount_file = exe_dir / "mount_points.txt";
    assert(std::filesystem::exists(mount_file));

    auto stream = std::ifstream(mount_file);
    std::string line;
    while (std::getline(stream, line)) {
        auto sp_id = line.find(' ');
        auto point_path = std::filesystem::path(line.substr(sp_id + 1, line.size()));
        if (point_path.is_relative()) {
            point_path = std::filesystem::absolute(exe_dir / point_path);
        }
        mount_points[line.substr(0, sp_id)] = point_path;
    }
}

auto ResourceProvider::resolve_resource(const ResourcePath& res) -> std::expected<std::filesystem::path, std::string>
{
    if (!mount_points.contains(res.module_name())) {
        return std::unexpected(std::format("No module named {}", res.module_name()));
    }
    return mount_points[res.module_name()] / res.rel_path();
}
} // namespace sde
