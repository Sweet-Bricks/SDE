# Run after all modules and programs have registered their components.
#
# components -> list of dictionaries describing modules:
#   name: str
#   requires: list[str] (direct depth)
#   runtime: list[str] (runtime files)
#   source_dir: str
#   program <optional, false>: bool (treat as program if true)
#   build_dir <required for programs>: str

install_script = files('tools/install_runtime.py')
mount_script = files('tools/generate_mounts.py')
traversed_components = {}
# traversed_components -> map with (name: dict) for each processed components

# We traverse all components as ordered direct graph
foreach component : components
    resources = {}
    runtime = []

    # Collect resource dirs and runtime artifacrs from each dependency
    foreach dep_name : component['requires']
        # Resurses
        foreach name, path : traversed_components[dep_name]['resources']
            resources += {name: path}
        endforeach
        # Runtime
        foreach path : traversed_components[dep_name]['runtime']
            if not runtime.contains(path)
                runtime += path
            endif
        endforeach
    endforeach

    # Own resource dir
    name = component['name']
    if component.has_key('source_dir')
        resources += {name: component['source_dir'] / 'res'}
    endif

    # Own runtime paths
    foreach path : component['runtime']
        if not runtime.contains(path)
            runtime += path
        endif
    endforeach

    traversed_components += {
        name: {
            'resources': resources,
            'runtime': runtime
        }
    }

    # If program pack needed resources
    if component.get('program', false)
        resource_args = []
        runtime_args = []

        # Fill with [mod_name: res_dir]
        foreach resource_name, path : resources
            resource_args += ['--resource', resource_name, path]
        endforeach
        # Fill with runtime paths
        foreach path : runtime
            runtime_args += ['--runtime', path]
        endforeach

        mount_file = component['build_dir'] / 'mount_points.txt'
        meson.add_install_script(python, install_script, name,
            '--mount-file', mount_file,
            '--mode', get_option('res_mode'),
            resource_args,
            runtime_args,
            install_tag: 'runtime',
        )
        # Running from the build tree always uses source resources.
        meson.add_postconf_script(python, mount_script,
            '--output', mount_file,
            resources.values(),
        )
    endif
endforeach
