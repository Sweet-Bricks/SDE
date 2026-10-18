#include "assets/wavefront_obj/wavefront_obj_utils.hpp"

#include "assets/wavefront_obj/parser/wavefront_obj_parser.hpp"
#include "assets/wavefront_obj/wavefront_obj_asset.hpp"
#include "file_system/file_system_errors.hpp"

#include <expected>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iterator>
#include <string>

namespace sde::wavefront_obj_utils
{
auto load_wavefront_obj_from_file(const std::filesystem::path& file_path) -> std::expected<WavefrontOBJAsset, WavefrontOBJLoadError>
{
    std::ifstream file(file_path, std::ios::binary);

    if (!file) {
        return std::unexpected(WavefrontOBJLoadError{ FileErrc::OPEN_FAILED });
    }

    std::string source{ std::istreambuf_iterator<char>{ file }, std::istreambuf_iterator<char>{} };

    if (file.bad()) {
        return std::unexpected(WavefrontOBJLoadError{ FileErrc::READ_FAILED });
    }

    return WavefrontOBJParser::parse_wavefront_obj(source);
}
} // namespace sde::wavefront_obj_utils
