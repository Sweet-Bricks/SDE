project(
    'sde',
    'cpp',
    version: '0.1.0',
    meson_version: '>=1.12',
    subproject_dir: 'third-party',
    default_options: [
        'buildtype=debug',
        # C++26 is set in build-logic: Meson 1.12 has no c++26 value for clang-cl
        'cpp_std=none',
        # Max warnings as errors for our code; third-party subprojects override these below.
        'warning_level=everything',
        'werror=true',
    ],
)

subdir('build-logic')

# include_type system: SDL3 headers don't produce warnings in our code.
sdl3_dep = dependency(
    'sdl3',
    include_type: 'system',
    default_options: ['warning_level=0', 'werror=false', 'default_library=static'],
)

vulkan_dep = dependency('vulkan-headers', include_type: 'system')

subdir('modules/core')
subdir('programs/game')
