#pragma once

#include "core/api.hpp"

#include <Eigen/Core>

#include <format>
#include <iosfwd>
#include <vector>

namespace sde
{
struct SDE_CORE_API ObjData
{
    std::vector<Eigen::Vector4f> vertices;
    std::vector<Eigen::Vector3i> faces;

    auto operator==(const ObjData&) const -> bool = default;
};
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

    SDE_CORE_API auto format(const sde::ObjData& data, std::format_context& ctx) const -> std::format_context::iterator;
};
} // namespace std

namespace sde
{
SDE_CORE_API auto operator<<(std::ostream& out, const ObjData& value) -> std::ostream&;
} // namespace sde
