#pragma once

#if defined(_WIN32)
#    if defined(SDE_CORE_BUILD)
#        define SDE_CORE_API __declspec(dllexport)
#    else
#        define SDE_CORE_API __declspec(dllimport)
#    endif
#else
#    define SDE_CORE_API __attribute__((visibility("default")))
#endif
