#include <assets/wavefront_obj/parser/wavefront_obj_parser.hpp>
#include <assets/wavefront_obj/wavefront_obj_utils.hpp>
#include <core/resource_path.hpp>
#include <core/resource_provider.hpp>

#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

TEST_CASE("OBJ: geometric vertices")
{
    auto result = sde::WavefrontOBJParser::parse_wavefront_obj("v 1 2 3\nv -1.5 0 2 0.5");
    REQUIRE(result.has_value());

    const auto& data = result.value();
    REQUIRE_EQ(data.positions.size(), 2);

    CHECK(data.positions[0].isApprox(Eigen::Vector4f(1, 2, 3, 1)));
    CHECK(data.positions[1].isApprox(Eigen::Vector4f(-1.5f, 0, 2, 0.5f)));
}

TEST_CASE("OBJ: polygon faces")
{
    auto result = sde::WavefrontOBJParser::parse_wavefront_obj("v 0 0 0\n"
                                                               "v 1 0 0\n"
                                                               "v 1 1 0\n"
                                                               "v 0 1 0\n"
                                                               "f 1 2 3 4");
    REQUIRE(result.has_value());

    const auto& data = result.value();
    REQUIRE_EQ(data.faces.size(), 1);
    REQUIRE_EQ(data.faces[0].vertices.size(), 4);

    for (std::size_t i = 0; i < 4; ++i) {
        CHECK_EQ(data.faces[0].vertices[i].position_index, i);
    }
}

TEST_CASE("OBJ: texture coordinates, normals and face references")
{
    auto result = sde::WavefrontOBJParser::parse_wavefront_obj("v 0 0 0\n"
                                                               "v 1 0 0\n"
                                                               "v 0 1 0\n"
                                                               "vt 0 0\n"
                                                               "vt 1 0\n"
                                                               "vt 0 1\n"
                                                               "vn 0 0 1\n"
                                                               "f 1/1/1 2/2/1 3/3/1");
    REQUIRE(result.has_value());

    const auto& data = result.value();

    REQUIRE_EQ(data.positions.size(), 3);
    REQUIRE_EQ(data.texcoords.size(), 3);
    REQUIRE_EQ(data.normals.size(), 1);
    REQUIRE_EQ(data.faces.size(), 1);
    REQUIRE_EQ(data.faces[0].vertices.size(), 3);

    for (std::size_t i = 0; i < 3; ++i) {
        const auto& vertex = data.faces[0].vertices[i];

        CHECK_EQ(vertex.position_index, i);
        REQUIRE(vertex.texcoord_index.has_value());
        REQUIRE(vertex.normal_index.has_value());

        CHECK_EQ(*vertex.texcoord_index, i);
        CHECK_EQ(*vertex.normal_index, 0);
    }
}

TEST_CASE("OBJ: negative face indices")
{
    auto result = sde::WavefrontOBJParser::parse_wavefront_obj("v 0 0 0\n"
                                                               "v 1 0 0\n"
                                                               "v 0 1 0\n"
                                                               "f -3 -2 -1");
    REQUIRE(result.has_value());

    const auto& data = result.value();
    REQUIRE_EQ(data.faces.size(), 1);
    REQUIRE_EQ(data.faces[0].vertices.size(), 3);

    CHECK_EQ(data.faces[0].vertices[0].position_index, 0);
    CHECK_EQ(data.faces[0].vertices[1].position_index, 1);
    CHECK_EQ(data.faces[0].vertices[2].position_index, 2);
}

TEST_CASE("OBJ: invalid input")
{
    SUBCASE("Missing vertex coordinate")
    {
        CHECK_FALSE(sde::WavefrontOBJParser::parse_wavefront_obj("v 1 2").has_value());
    }

    SUBCASE("Too many vertex coordinates")
    {
        CHECK_FALSE(sde::WavefrontOBJParser::parse_wavefront_obj("v 1 2 3 4 5").has_value());
    }

    SUBCASE("Face with fewer than three vertices")
    {
        CHECK_FALSE(sde::WavefrontOBJParser::parse_wavefront_obj("v 0 0 0\nf 1 1").has_value());
    }

    SUBCASE("Invalid positive index")
    {
        CHECK_FALSE(sde::WavefrontOBJParser::parse_wavefront_obj("v 0 0 0\nf 1 2 3").has_value());
    }

    SUBCASE("Invalid negative index")
    {
        CHECK_FALSE(sde::WavefrontOBJParser::parse_wavefront_obj("v 0 0 0\nf -2 -1 -1").has_value());
    }

    SUBCASE("Zero index")
    {
        CHECK_FALSE(sde::WavefrontOBJParser::parse_wavefront_obj("v 0 0 0\nf 0 1 1").has_value());
    }

    SUBCASE("Malformed face reference")
    {
        CHECK_FALSE(sde::WavefrontOBJParser::parse_wavefront_obj("f 1/2/ 2/3/4 3/4/5").has_value());
    }
}

TEST_CASE("OBJ: comments and unsupported statements")
{
    auto result = sde::WavefrontOBJParser::parse_wavefront_obj("# Comment\n"
                                                               "o Cube\n"
                                                               "\n"
                                                               "v 1 2 3 # Inline comment\n"
                                                               "g Group01\n");
    REQUIRE(result.has_value());

    CHECK_EQ(result->positions.size(), 1);
    CHECK(result->positions[0].isApprox(Eigen::Vector4f(1, 2, 3, 1)));
}
