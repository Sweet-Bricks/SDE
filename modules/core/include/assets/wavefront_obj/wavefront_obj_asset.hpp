#pragma once

#include <Eigen/Core>

#include <cstddef>
#include <optional>
#include <vector>

namespace sde
{
struct WavefrontOBJAsset
{
    struct Face
    {
        struct Vertex
        {
            std::size_t position_index{};
            std::optional<std::size_t> texcoord_index{};
            std::optional<std::size_t> normal_index{};

            auto operator==(const Vertex&) const -> bool = default;
        };

        std::vector<Vertex> vertices;

        auto operator==(const Face&) const -> bool = default;
    };

    std::vector<Eigen::Vector4f> positions;
    std::vector<Eigen::Vector3f> texcoords;
    std::vector<Eigen::Vector3f> normals;
    std::vector<Face> faces;

    auto operator==(const WavefrontOBJAsset&) const -> bool = default;
};
} // namespace sde
