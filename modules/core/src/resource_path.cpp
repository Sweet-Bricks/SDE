#include "core/resource_path.hpp"

#include <cassert>
#include <string>

namespace sde
{
ResourcePath::ResourcePath(const std::string& abstract_resource_path)
{
    assert(abstract_resource_path.starts_with('/'));
    auto sec_slash = abstract_resource_path.find('/', 1);
    module = abstract_resource_path.substr(1, sec_slash - 1);
    path = abstract_resource_path.substr(sec_slash + 1, abstract_resource_path.size());
}

auto ResourcePath::module_name() const -> const std::string&
{
    return module;
}

auto ResourcePath::rel_path() const -> const std::string&
{
    return path;
}
} // namespace sde
