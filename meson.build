project('rayanim', 'c',
  version: '0.1.0',
  license: 'MIT',
  default_options: [
    'warning_level=3',
    'werror=true',
    'c_std=c17'
  ]
)

python = import('python').find_installation('python3')
result = run_command(python, files('scripts/get_raylib.py'), check: true)
if result.returncode() == 0
  outputs = result.stdout().strip().split('\n')
  foreach output : outputs
    if output.contains('RAYLIB_LIB_DIR')
      raylib_lib_dir = output.strip().split(':')[1].strip()
    elif output.contains('RAYLIB_INCLUDE_DIR')
      raylib_include_dir = output.strip().split(':')[1].strip()
    endif
  endforeach
else
  error('Failed to get raylib')
endif

result = run_command(python, files('scripts/generate_clangd_config.py'), raylib_include_dir, check: true)
if result.returncode() != 0
  error('Failed to set RAYLIB_INCLUDE_DIR')
endif

cc = meson.get_compiler('c')

raylib_dep = declare_dependency(
  dependencies: cc.find_library('raylib', dirs: raylib_lib_dir),
  include_directories: include_directories(raylib_include_dir)
)

src = [
  'src/main.c'
]

executable('rayanim',
  src,
  dependencies: [raylib_dep]
)
