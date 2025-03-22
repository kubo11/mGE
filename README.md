# mGE
Minimal Graphics Engine.

## Requirements

- gcc/g++
- make
- cmake
- git

## Building

In order to build this project you have to download required submodules. This can be achieved by either adding `--recursive` flag to `git clone` command or by running the following command in an already cloned repository directory:
```
git submodule update --init --recursive
```

To build the project run inside the root repository directory:
```
mkdir build && cd build
cmake ..
cmake --build .
```

If you want to speed up the buliding process add `-jN` flag to the last command, where `N` is the number of paralell processes to run (for optimal performance do not run more processes than the number of threads in your system).

## Libraries

This project has been created with use of some other open-source dependencies:
- [glad](https://github.com/Dav1dde/glad)
- [glfw](https://github.com/glfw/glfw)
- [spdlog](https://github.com/gabime/spdlog)
- [glm](https://github.com/g-truc/glm)
- [imgui](https://github.com/ocornut/imgui)
- [entt](https://github.com/skypjack/entt.git)