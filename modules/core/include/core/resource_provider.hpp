#pragma once

#include "core/api.hpp"

#include <expected>
#include <filesystem>
#include <string>
#include <unordered_map>

namespace sde
{
class SDE_CORE_API ResPath
{
public:
    explicit ResPath(const std::string&);
    [[nodiscard]] auto module_name() const -> std::string;
    [[nodiscard]] auto rel_path() const -> std::string;

private:
    std::string module;
    std::string path;
};

class SDE_CORE_API ResourceProvider final
{
public:
    ResourceProvider();
    auto resolve_res(const ResPath&) -> std::expected<std::filesystem::path, std::string>;

private:
    std::unordered_map<std::string, std::filesystem::path> mount_points;
};
} // namespace sde
