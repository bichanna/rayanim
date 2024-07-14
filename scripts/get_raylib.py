import sys
import os
import platform
import logging
import urllib.request
import site
import tarfile
import argparse
from pathlib import Path
from typing import Tuple, Union

linux = {"x86_64": "_linux_amd64.tar.gz", "i386": "_linux_i386.tar.gz"}

macos = "_macos.tar.gz"


def get_raylib(library_path: Path) -> Union[Tuple[str, str], None]:
    download_url = "https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0"

    system = platform.system()
    logger.info(f"Using platform {system}")

    match system:
        case "Linux":
            archtecture = platform.machine()
            try:
                download_url += linux[archtecture]
            except KeyError:
                logger.error(f"Unsupported archtecture {archtecture}")
                return None
        case "Darwin":
            download_url += macos
        case _:
            logger.error(f"Sorry, I'm too lazy to handle Windows and other platforms")
            logger.error(f"Unsupported platform {platform}")
            return None

    compressed_raylib = library_path / "compressed-raylib"

    try:
        with (
            urllib.request.urlopen(download_url) as response,
            open(compressed_raylib, "wb") as f,
        ):
            logger.info("Downloaded compressed raylib")
            f.write(response.read())
    except URLError as e:
        logger.error(f"URLError: {e}")
        return None
    except HTTPError as e:
        logger.error(f"HTTPError: {e}")
        return None
    except Exception as e:
        logger.error(f"Exception: {e}")
        return None

    try:
        with tarfile.open(compressed_raylib, mode="r:gz") as tar:
            logger.info(f"Extracting {compressed_raylib} to {library_path}")
            tar.extractall(path=library_path)
            logger.info(f"Success extracting to {library_path}")
    except FileNotFoundError:
        logger.error(f"{compressed_raylib} file not found")
        return None
    except tarfile.ReadError:
        logger.error(f"Invalid archive format for {compressed_raylib}")
        return None

    try:
        os.remove(compressed_raylib)
    except FileNotFoundError:
        logger.error(f"{compressed_raylib} file not found")
        return None
    except PermissionError:
        logger.error(f"Don't have permission to delete {compressed_raylib}")
        return None
    except OSError as e:
        logger.error(f"OSError: {e}")
        return None

    for _, dirs, _ in os.walk(library_path):
        for dir in dirs:
            if "raylib" in dir:
                raylib_dir = library_path / dir

    raylib_lib_dir = raylib_dir / "lib"
    raylib_include_dir = raylib_dir / "include"

    return raylib_lib_dir, raylib_include_dir


def main(logger: logging.Logger) -> int:
    library_path = Path(site.getuserbase())

    for _, dirs, _ in os.walk(library_path):
        for dir in dirs:
            if "raylib" in dir:
                raylib_dir = library_path / dir
                lib_dir = raylib_dir / "lib"
                include_dir = raylib_dir / "include"

    if not lib_dir.exists() or not include_dir.exists():
        lib_dir, include_dir = get_raylib(library_path)

    print("RAYLIB_LIB_DIR:", lib_dir)
    print("RAYLIB_INCLUDE_DIR:", include_dir)

    return 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--verbose", action="store_true", help="Enable verbose mode")
    args = parser.parse_args()

    if args.verbose:
        logging.basicConfig(level=logging.INFO)
    else:
        logging.basicConfig(level=logging.ERROR)

    logger = logging.getLogger(__name__)

    sys.exit(main(logger))
