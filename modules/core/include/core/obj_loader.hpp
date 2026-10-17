#pragma once

#include "core/api.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

#include <expected>
#include <filesystem>
#include <format>
#include <string>
#include <vector>

namespace sde
{
struct SDE_CORE_API ObjData
{
    std::vector<Eigen::Vector4f> vertices;
    std::vector<Eigen::Vector3i> faces;
};

auto SDE_CORE_API read_obj_file(const std::filesystem::path& file_path) -> std::expected<ObjData, std::string>;
} // namespace sde

namespace std
{
template <>
struct formatter<sde::ObjData>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const sde::ObjData& data, std::format_context& ctx) const
    {
        auto out = ctx.out();
        out = std::format_to(out, "Vertices: [");
        for (const auto& v : data.vertices) {
            out = std::format_to(out, "\n    ({}, {}, {}, {})", v[0], v[1], v[2], v[3]);
        }
        out = std::format_to(out, "]\nFaces: [");
        for (const auto& v : data.faces) {
            out = std::format_to(out, "\n    ({}, {}, {})", v[0], v[1], v[2]);
        }
        out = std::format_to(out, "\n]");
        return out;
    }
};
} // namespace std
