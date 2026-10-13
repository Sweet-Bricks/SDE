sdl3_dep = dependency(
    'sdl3',
    static: true,
    include_type: 'system',
    default_options: ['warning_level=0', 'werror=false'],
)

cmake = import('cmake')

jolt_dev = get_option('debug')

jolt_opts = cmake.subproject_options()
jolt_opts.add_cmake_defines({
    'JPH_BUILD_SHARED_LIBS': false,
    'ENABLE_INSTALL': false,
    'ENABLE_ALL_WARNINGS': false,
    # Let Meson's buildtype drive optimization/LTO instead of Jolt's own flag overrides
    'OVERRIDE_CXX_FLAGS': false,
    'INTERPROCEDURAL_OPTIMIZATION': false,
    'USE_ASSERTS': jolt_dev,
    'DEBUG_RENDERER_IN_DEBUG_AND_RELEASE': false,
    'DEBUG_RENDERER_IN_DISTRIBUTION': jolt_dev,
    'PROFILER_IN_DEBUG_AND_RELEASE': false,
    'PROFILER_IN_DISTRIBUTION': jolt_dev,
    # GPU compute backends: need Metal/Vulkan/DX12 SDKs and shader compilers, not used yet
    'JPH_USE_DX12': false,
    'JPH_USE_VK': false,
    'JPH_USE_MTL': false,
    'JPH_USE_CPU_COMPUTE': false,
})
jolt_opts.set_override_option('warning_level', '0')
jolt_opts.set_override_option('werror', 'false')

jolt_proj = cmake.subproject('jolt', options: jolt_opts)
jolt_dep = jolt_proj.dependency('Jolt', include_type: 'system')
