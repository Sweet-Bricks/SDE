#include "core/render/obj_loader.hpp"

#include "core/file_utils.hpp"
#include "core/render/obj_data.hpp"

#include <Eigen/Core>
#include <spdlog/spdlog.h>

#include <charconv>
#include <cmath>
#include <expected>
#include <filesystem>
#include <format>
#include <iterator>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

namespace sde
{
namespace
{
template <class T>
auto parse_token(std::string_view toc, int line_num) -> std::expected<T, std::string>
{
    T val = 0;
    auto [ptr, ec] = std::from_chars(std::to_address(toc.begin()), std::to_address(toc.end()), val);
    if (ec != std::errc{}) {
        auto str_err = std::generic_category().message(static_cast<int>(ec));
        return std::unexpected(std::format("Cannot parse token at {}: [{}] - {}", line_num, toc, str_err));
    }
    if (ptr != std::to_address(toc.end())) {
        return std::unexpected(std::format("Cannot parse token at {}: [{}]", line_num, toc));
    }
    return val;
}

auto parse_vertex(std::ranges::input_range auto&& tokens, int line_num, ObjData& result_data) -> std::expected<void, std::string>
{
    Eigen::Vector4f vertex{ 0.0f, 0.0f, 0.0f, 1.0f };
    int count = 0;

    for (std::string_view token : tokens) {
        if (count == 4) {
            return std::unexpected(std::format("Too many vertex coordinates at line {}", line_num));
        }

        auto value = parse_token<float>(token, line_num);
        if (!value) {
            return std::unexpected(std::move(value.error()));
        }

        if (!std::isfinite(*value)) {
            return std::unexpected(std::format("Non-finite coordinate at line {}", line_num));
        }

        vertex[count++] = *value;
    }

    if (count < 3) {
        return std::unexpected(std::format("Too few vertex coordinates at line {}", line_num));
    }

    result_data.vertices.push_back(vertex);
    return {};
}

auto parse_face(std::ranges::input_range auto&& tokens, int line_num, ObjData& result_data) -> std::expected<void, std::string>
{
    int counter = 0;
    int first_val = 0;
    int prev_val = 0;
    for (std::string_view token : tokens) {
        auto value = parse_token<int>(token, line_num);
        if (!value) {
            return std::unexpected(std::move(value.error()));
        }
        auto parsed_val = *value;
        if (parsed_val < 0) {
            parsed_val = static_cast<int>(result_data.vertices.size()) + parsed_val + 1;
        }
        if (parsed_val <= 0 || std::cmp_greater(parsed_val, result_data.vertices.size())) {
            return std::unexpected(std::format("Invalid index ({}) at line {}", token, line_num));
        }
        if (counter == 0) {
            first_val = parsed_val;
        }
        else if (counter > 1) {
            result_data.faces.emplace_back(first_val, prev_val, parsed_val);
        }
        prev_val = parsed_val;
        counter++;
    }
    if (counter < 3) {
        return std::unexpected(std::format("Too few indexes for face at line {}", line_num));
    }
    return {};
}
} // namespace

auto parse_obj(const std::string_view text) -> std::expected<ObjData, std::string>
{
    ObjData result_data;
    int line_num = 0;
    for (auto line : text | std::views::split('\n')) {
        line_num++;
        auto tokens = line
            | std::views::split(' ')
            | std::views::transform([](auto token) -> auto { return std::string_view(token); })
            | std::views::filter([](std::string_view token) -> bool { return !token.empty(); })
            | std::views::take_while([](std::string_view token) -> bool { return !token.starts_with('#'); });

        if (std::ranges::empty(tokens)) {
            // No token to parse after filter
            continue;
        }

        auto first_token = *tokens.begin();
        auto from_sec = tokens | std::ranges::views::drop(1);
        if (first_token == "v") {
            auto result = parse_vertex(from_sec, line_num, result_data);
            if (!result) {
                return std::unexpected(std::move(result.error()));
            }
        }
        else if (first_token == "f") {
            auto result = parse_face(from_sec, line_num, result_data);
            if (!result) {
                return std::unexpected(std::move(result.error()));
            }
        }
        else {
            spdlog::warn("Unrecognized format at {}: [{}]", line_num, line);
        }
    }
    return result_data;
}

auto parse_obj_file(const std::filesystem::path& path) -> std::expected<ObjData, std::string>
{
    return read_file(path).and_then([](const auto& contents) -> auto { return parse_obj(contents); });
}
} // namespace sde
