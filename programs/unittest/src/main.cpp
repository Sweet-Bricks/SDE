#define DOCTEST_CONFIG_IMPLEMENT

#include "unittest/test_utils.hpp"

#include <core/engine.hpp>
#include <doctest.h>

sde::Engine* test_engine = nullptr;

auto main(int argc, char** argv) -> int
{
    doctest::Context context;
    context.setOption("no-intro", true);
    context.setOption("no-path-filenames", true);
    context.applyCommandLine(argc, argv);

    if (context.shouldExit()) {
        return context.run();
    }

    test_engine = new sde::Engine{};
    test_engine->initialize();
    return context.run();
}
