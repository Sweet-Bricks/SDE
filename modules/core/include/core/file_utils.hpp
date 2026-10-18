#include "core/api.hpp"

#include <expected>
#include <filesystem>
#include <string>

namespace sde
{
auto SDE_CORE_API read_file(const std::filesystem::path& path) -> std::expected<std::string, std::string>;
}
