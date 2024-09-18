project('my_project', 'c',
  version: '0.1',
  default_options: ['warning_level=2', 'c_std=c11'])

src_dir = include_directories('src')

executable('my_project_exe', sources: [
  'src/src.c',
  'main.c'
], include_directories: src_dir)
