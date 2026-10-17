#include "core/obj_loader.hpp"

#include <Eigen/Core>
#include <spdlog/spdlog.h>

#include <charconv>
#include <expected>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace sde
{
auto read_obj_file(const std::filesystem::path& file_path) -> std::expected<ObjData, std::string>
{
    if (!std::filesystem::exists(file_path)) {
        return std::unexpected("Cannot parse not existed file: " + file_path.string());
    }
    if (file_path.extension() != ".obj") {
        return std::unexpected("File shuld be with .obj extesion: " + file_path.filename().string());
    }

    std::ifstream file_stream(file_path);
    ObjData result_data;

    std::string line;
    int line_num = 0;
    while (std::getline(file_stream, line)) {
        line_num++;
        auto tokens = line | std::views::split(' ');
        auto first_token_view = tokens | std::ranges::views::take(1);
        if (std::ranges::empty(first_token_view)) {
            spdlog::debug("Skipping empty line at {}: [{}]", line_num, line);
            continue;
        }

        auto first_token = std::string_view(*first_token_view.begin());
        if (first_token.empty()) {
            spdlog::debug("Skipping empty line at {}: [{}]", line_num, line);
            continue;
        }

        if (first_token == "#") {
            spdlog::debug("Skipping comment at {}: [{}]", line_num, line);
            continue;
        }

        auto from_sec = tokens | std::ranges::views::drop(1) | std::views::transform([](auto r) -> auto {
            return std::string_view(r);
        });
        if (first_token == "v") {
            // Vertices
            auto parsed_floats = from_sec | std::views::take(4) | std::views::transform([line_num](std::string_view toc) -> std::optional<float> {
                float val = 0;
                auto [_, ec] = std::from_chars(std::to_address(toc.begin()), std::to_address(toc.end()), val);
                if (ec == std::errc{}) [[likely]] {
                    return val;
                }
                spdlog::error("Cannot parse token at {}: [{}] - {}", line_num, toc, std::generic_category().message(static_cast<int>(ec)));
                return std::nullopt;
            });

            Eigen::Vector4f vec4 = Eigen::Vector4f::Zero();
            vec4 << 0.0f, 0.0f, 0.0f, 1.0f;
            int count = 0;
            for (auto opt_val : parsed_floats) {
                if (opt_val.has_value()) {
                    vec4[count] = *opt_val;
                }
                count++;
            }

            if (count >= 3) [[likely]] {
                result_data.vertices.push_back(vec4);
            }
            else {
                spdlog::error("Error at {}: Vertex must have at least 3 coordinates! Got: {}", line_num, count);
            }

            if (std::ranges::distance(from_sec) > 4) {
                spdlog::error("More tokens at {}: [{}]", line_num, line);
            }
        }
        else if (first_token == "f") {
            // Faces
            auto parsed_ints = from_sec | std::views::transform([line_num](std::string_view toc) -> std::optional<int> {
                int val = 0;
                auto [_, ec] = std::from_chars(std::to_address(toc.begin()), std::to_address(toc.end()), val);
                if (ec == std::errc{}) [[likely]] {
                    return val;
                }
                spdlog::error("Cannot parse token at {}: [{}] - {}", line_num, toc, std::generic_category().message(static_cast<int>(ec)));
                return std::nullopt;
            });
            int counter = 0;
            int first_val = 0;
            int prev_val = 0;
            for (const auto opt_val : parsed_ints) {
                if (opt_val.has_value()) {
                    auto parsed_val = *opt_val;
                    if (counter == 0) {
                        first_val = parsed_val;
                    }
                    else if (counter > 1) {
                        result_data.faces.emplace_back(first_val,
                                                       prev_val,
                                                       parsed_val);
                    }
                    prev_val = parsed_val;
                }
                counter++;
            }
        }
        else {
            spdlog::warn("Unrecognized at {}: [{}]", line_num, line);
            continue;
        }
    }

    spdlog::info("Parsed data: {}", result_data);
    return result_data;
}
} // namespace sde
