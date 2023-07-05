# peek

Peek is process monitoring application that allows linux users to inspect active processes on their system. It uses GNOME's [glib](https://github.com/GNOME/glib) and [glibtop](https://github.com/GNOME/libgtop) libraries to interface with the system and [gtk](https://github.com/GNOME/gtk) to construct a user interface.

This project was created to showcase my knowledge of the GTK library and explore an interest of the inner working of the linux operating system. All major operating systems provide some visual of the active processes within the system and I thought it would be cool to figure out how this data is obtained and design my own interface for it. I discovered that reading process information is not as straight forward as you would think due to the way it is exposed by the kernel. On linux, the kernel exposes a virtual filesystem of process information that can be found at /proc (a window of sorts to the process activity inside the kernel). Previous experience suggested that I make the application event driven for updates to the active processes, but the API for monitoring directories and files for changes, [inotify(7)](https://www.man7.org/linux/man-pages/man7/inotify.7.html), does not allow for monitoring of virtual filesystems. This made the design polling based at a set interval. 

Obtaining the process data was made easy with the _libgtop_ API's after several hours spent of trial and error with the _libprocps_ library, another process monitoring library (see [readproc(3)](https://manpages.debian.org/testing/procps/readproc.3.en.html)).

## Building the Project
To build peek and try it for yourself, there are a couple dependencies!
### Dependencies
- [Meson](https://mesonbuild.com/Getting-meson.html) build system
- [libgtk-3-dev](https://www.gtk.org/docs/installations/linux/)
- glib-2.0
- libgtop2-dev
### Building & Running
Navigate to the root of the project on your command line and run `meson compile -C build` to build the application (see [commands](https://mesonbuild.com/Commands.html) for more options). The compiled binary can be found at `build/src`.
