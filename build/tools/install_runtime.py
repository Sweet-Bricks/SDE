"""Install the runtime files selected by Meson for one program."""

import argparse
import filecmp
import json
import os
from pathlib import Path
import shutil

MOUNT_POINT = "mount_points.txt"
RESOURCE_CACHE = ".resource-files.json"
RUNTIME_CACHE = ".runtime-files.json"

def sync_resources(source: Path, destination: Path) -> None:
    manifest_file = destination / RESOURCE_CACHE
    files = {
        path.relative_to(source).as_posix(): path
        for path in sorted(source.rglob("*"))
        if path.is_file()
    }

    previous_files: list[str] = []
    if manifest_file.exists():
        with manifest_file.open(encoding="utf-8") as file:
            previous_files = json.load(file)

    for rel_path in set(previous_files) - files.keys():
        installed = destination / rel_path
        installed.unlink(missing_ok=True)

    destination.mkdir(parents=True, exist_ok=True)
    for name, path in files.items():
        installed = destination / name
        installed.parent.mkdir(parents=True, exist_ok=True)
        if not installed.exists() or not filecmp.cmp(path, installed, shallow=False):
            shutil.copy2(path, installed)

    manifest_data = json.dumps(list(files), indent=4)
    manifest_file.write_text(manifest_data, encoding="utf-8")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("program")
    parser.add_argument("--mount-file", type=Path, required=True)
    parser.add_argument("--mode", choices=["mount", "copy"], default="mount")
    parser.add_argument("--resource", nargs=2, action="append", default=[])
    parser.add_argument("--runtime", type=Path, action="append", default=[])
    args = parser.parse_args()

    program = str(args.program)
    mount_file = Path(args.mount_file)
    mode = str(args.mode)
    resources = {str(name): Path(path) for name, path in args.resource}
    runtime = {
        str(path.name): Path(path)
        for path in args.runtime
        if path.is_file()
    }

    destination = Path(os.environ["MESON_INSTALL_DESTDIR_PREFIX"]) / program
    resource_dir = destination / "res"
    installed_mount_file = destination / MOUNT_POINT
    manifest_file = destination / RUNTIME_CACHE
    previous_runtime: list[str] = []

    if manifest_file.exists():
        with manifest_file.open(encoding="utf-8") as file:
            previous_runtime = json.load(file)

    destination.mkdir(parents=True, exist_ok=True)
    # Copy runtime
    for name, path in runtime.items():
        installed = destination / name
        if not installed.exists() or not filecmp.cmp(path, installed, shallow=False):
            shutil.copy2(path, installed)
    # Delete diff
    for name in set(previous_runtime) - runtime.keys():
        (destination / name).unlink(missing_ok=True)

    manifest_data = json.dumps(list(runtime), indent=4)
    manifest_file.write_text(manifest_data, encoding="utf-8")

    match mode:
        case "mount":
            shutil.copy2(mount_file, installed_mount_file)
        case "copy":
            installed_mount_file.unlink(missing_ok=True)
            mount_file_data = "\n".join(f"{name} ./res/{name}" for name in resources.keys())
            installed_mount_file.write_text(mount_file_data, encoding="utf-8")
            for name, source in resources.items():
                sync_resources(source, resource_dir / name)


if __name__ == "__main__":
    main()
