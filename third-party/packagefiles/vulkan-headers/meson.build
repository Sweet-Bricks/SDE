project('vulkan-headers', version: '1.4.365', meson_version: '>=1.12')

vulkan_headers_dep = declare_dependency(
    include_directories: include_directories('include'),
    # No vulkan-1.lib link: functions are loaded at runtime via SDL_Vulkan_GetVkGetInstanceProcAddr
    compile_args: '-DVK_NO_PROTOTYPES',
)
meson.override_dependency('vulkan-headers', vulkan_headers_dep)
