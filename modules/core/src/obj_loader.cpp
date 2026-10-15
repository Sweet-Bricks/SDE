#include "core/obj_loader.hpp"

#include <Eigen/Core>

#include <expected>
#include <filesystem>
#include <string>

namespace sde
{
auto read_obj_file(const std::filesystem::path& file_path) -> std::expected<ObjData, std::string>
{
    if (!std::filesystem::exists(file_path))
    {
        return std::unexpected("Cannot parse not existed file: " + file_path.string());
    }
    if (file_path.extension() != ".obj")
    {
        return std::unexpected("File shuld be with .obj extesion: " + file_path.filename().string());
    }

    return ObjData{};
}
} // namespace sde
