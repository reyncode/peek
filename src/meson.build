sources = [
  'main.c',
  'peek-application.c', 'peek-application.h',
  'peek-window.c', 'peek-window.h',
  'peek-preferences.c', 'peek-preferences.h',
  'peek-process-view.c', 'peek-process-view.h',
  'peek-tree-view.c', 'peek-tree-view.h',
  'peek-tree-model.c', 'peek-tree-model.h',
  'peek-process.c', 'peek-process.h',
]

resources = gnome.compile_resources(
  'resources',
  join_paths(data_dir, 'com.github.reyncode.peek.gresource.xml'),
  source_dir: [
    data_dir,
  ],
  c_name: 'resources'
)

executable(
  meson.project_name(),
  sources: sources + resources,
  dependencies: deps,
  install: true,
)
