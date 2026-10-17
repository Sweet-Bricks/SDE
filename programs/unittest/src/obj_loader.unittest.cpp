#include "core/resource_path.hpp"
#include "core/resource_provider.hpp"
#include "unittest/test_utils.hpp"

#include <core/obj_loader.hpp>
#include <doctest.h>

#include <filesystem>

TEST_CASE("testing obj file reading")
{
    auto* resource_provider = test_engine->get<sde::ResourceProvider>();
    auto test_obj = sde::ResourcePath("/unittest/box.obj");
    CHECK_EQ(test_obj.module_name(), "unittest");
    CHECK_EQ(test_obj.rel_path(), "box.obj");

    auto obj_result = resource_provider->resolve_resource(test_obj);
    CHECK(obj_result.has_value());
    CHECK(std::filesystem::exists(obj_result.value()));

    auto obj = sde::read_obj_file(obj_result.value());
    CHECK(obj.has_value());
}
