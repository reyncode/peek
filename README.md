# peek

Peek is process monitoring application that allows linux users to inspect active processes on their system. It uses GNOME's [glib](https://github.com/GNOME/glib) and [glibtop](https://github.com/GNOME/libgtop) libraries to interface with the system and [gtk](https://github.com/GNOME/gtk) to construct a user interface.

This project was created to showcase my knowledge of the GTK library and explore an interest of the inner working of the linux operating system. All major operating systems provide some visual of the active processes within the system and I thought it would be cool to figure out how this data is obtained and design my own interface for it. Acquiring process data was made easy thorough the _libgtop_ API's but efficiently handling the data was another story.

## Building the Project
To build peek and try it for yourself, there are a couple dependencies!
### Dependencies
- [Meson](https://mesonbuild.com/Getting-meson.html) build system
- [libgtk-3-dev](https://www.gtk.org/docs/installations/linux/)
- glib-2.0
- libgtop2-dev
### Building & Running
Navigate to the root of the project on your command line and run `meson compile -C build` to build the application (see [commands](https://mesonbuild.com/Commands.html) for more options). The compiled binary can be found at `build/src`.
