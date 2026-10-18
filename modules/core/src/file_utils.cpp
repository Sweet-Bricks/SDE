#include "core/file_utils.hpp"

#include <cstddef>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <string>
#include <system_error>

namespace sde
{
auto read_file(const std::filesystem::path& path) -> std::expected<std::string, std::string>
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return std::unexpected("Cannot open file: " + path.string());
    }

    std::error_code ec;
    const auto size = std::filesystem::file_size(path, ec);
    if (ec) {
        return std::unexpected(std::format("Cannot get size of {}: {}", path.string(), ec.message()));
    }

    std::string data;
    data.resize_and_overwrite(size, [&](char* buf, std::size_t n) -> std::size_t {
        file.read(buf, static_cast<std::streamsize>(n));
        return static_cast<std::size_t>(file.gcount());
    });
    if (data.size() != size) {
        return std::unexpected(std::format("Short read of {}: {} of {} bytes", path.string(), data.size(), size));
    }
    std::erase(data, '\r');
    return data;
}
} // namespace sde
