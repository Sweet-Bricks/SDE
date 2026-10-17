#pragma once

#include "core/api.hpp"

#include <string>

namespace sde
{
class SDE_CORE_API ResourcePath
{
public:
    explicit ResourcePath(const std::string&);
    [[nodiscard]] auto module_name() const [[clang::lifetimebound]] -> const std::string&;
    [[nodiscard]] auto rel_path() const [[clang::lifetimebound]] -> const std::string&;

private:
    std::string module;
    std::string path;
};
} // namespace sde
