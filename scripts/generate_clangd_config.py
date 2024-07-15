import sys
from pathlib import Path
from string import Template


dot_clangd = Template("""
If:
  PathMatch: $include

CompileFlags:
  Add:
    - "-I$include"
""")

compile_flags_txt = Template("-I$include")

def main() -> int:
    if len(sys.argv) <= 1:
        print("Excepted raylib include path")
        return 1

    include_dir = Path(sys.argv[-1])
    
    if include_dir.name != "include" or not include_dir.exists():
        return 1

    if not Path(".clangd").exists():
        with open(".clangd", "w") as f:
            f.write(dot_clangd.safe_substitute(include=include_dir))
    
    if not Path("compile_flags.txt").exists():
        with open("compile_flags.txt", "w") as f:
            f.write(compile_flags_txt.safe_substitute(include=include_dir))

    return 0


if __name__ == "__main__":
    sys.exit(main())
