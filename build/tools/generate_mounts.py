"""Generate a mount file from resource directory paths."""

import argparse
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("resources", type=Path, nargs="*")
    args = parser.parse_args()

    out_file = Path(args.output)
    resources = [Path(r) for r in args.resources]
    mount_file_data = "\n".join(f"{path.parent.name} {path.as_posix()}" for path in resources)

    out_file.parent.mkdir(parents=True, exist_ok=True)
    out_file.write_text(mount_file_data, encoding="utf-8")


if __name__ == "__main__":
    main()
