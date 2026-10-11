# Compiler flags for our own targets (third-party subprojects keep their defaults).
# warning_level=everything + werror=true (root meson.build) give clang-cl /Wall (= clang -Weverything) and /WX.
add_project_arguments(
    # clang-cl: /std:c++latest is the post-C++26 draft; this is C++26 (__cplusplus 202400L)
    '/std:c++26preview',

    # Compat groups flag modern C++ itself
    '-Wno-c++98-compat',
    '-Wno-c++98-compat-pedantic',
    '-Wno-pre-c++14-compat',
    '-Wno-pre-c++17-compat',
    '-Wno-pre-c++20-compat',
    '-Wno-pre-c++20-compat-pedantic',
    '-Wno-pre-c++23-compat',
    '-Wno-pre-c++26-compat',

    # Flags every printf-style call with a C-API string (e.g. SDL_Log("%s", SDL_GetError()))
    '-Wno-unsafe-buffer-usage-in-format-attr-call',

    language: 'cpp',
)
