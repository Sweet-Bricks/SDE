#include "core/render/obj_data.hpp"

#include <format>
#include <ostream>

auto std::formatter<sde::ObjData>::format(const sde::ObjData& data, std::format_context& ctx) const -> std::format_context::iterator
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

namespace sde
{
auto operator<<(std::ostream& out, const ObjData& value) -> std::ostream&
{
    return out << std::format("{}", value);
}
} // namespace sde
