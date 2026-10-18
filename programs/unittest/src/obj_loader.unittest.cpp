#include "unittest/test_utils.hpp"

#include <core/render/obj_data.hpp>
#include <core/render/obj_loader.hpp>
#include <core/resource_path.hpp>
#include <core/resource_provider.hpp>
#include <doctest.h>

#include <filesystem>

TEST_CASE("testing obj file parsing")
{
    auto one_vert_result = sde::parse_obj("v 1 1 1 1");
    CHECK(one_vert_result.has_value());
    CHECK_EQ(one_vert_result.value(), sde::ObjData{ { { 1, 1, 1, 1 } }, {} });

    auto one_face_result = sde::parse_obj("v 1 1 1 1\nf 1 1 1 1");
    CHECK(one_face_result.has_value());
    CHECK_EQ(one_face_result.value(), sde::ObjData{ { { 1, 1, 1, 1 } }, { { 1, 1, 1 }, { 1, 1, 1 } } });

    auto complex_result = sde::parse_obj("v 1 1 1 1\nv 2 2 2 2\nf 1 2 1 1\nf 1 -1 2 2");
    CHECK(complex_result.has_value());
    CHECK_EQ(complex_result.value(), sde::ObjData{ { { 1, 1, 1, 1 }, { 2, 2, 2, 2 } }, { { 1, 2, 1 }, { 1, 1, 1 }, { 1, 2, 2 }, { 1, 2, 2 } } });
}

TEST_CASE("testing obj file reading")
{
    auto* resource_provider = test_engine->get<sde::ResourceProvider>();
    auto test_obj = sde::ResourcePath("/unittest/box.obj");
    CHECK_EQ(test_obj.module_name(), "unittest");
    CHECK_EQ(test_obj.rel_path(), "box.obj");

    auto obj_result = resource_provider->resolve_resource(test_obj);
    CHECK(obj_result.has_value());
    CHECK(std::filesystem::exists(obj_result.value()));

    auto obj = sde::parse_obj_file(obj_result.value());
    CHECK(obj.has_value());
}
