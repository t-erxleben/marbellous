# Marbellous

Marbellous will be an WebApp to generate [paper marbling](https://en.wikipedia.org/wiki/Paper_marbling) pattern.
We aim for the best possible performance though usage of WebASM and WebGL.
The project is still in an early state, so there for most of the functionality is still missing.

## How to use

You can play with the current version at: [](https://t-erxleben.github.io/marbellous).
![preview-status](https://github.com/t-erxleben/marbellous/actions/workflows/main.yml/badge.svg).

### Use it local

**required tooling**

* cmake
* emscripten

1. clone the repository and sub-repositories with: `git clone --recurse-submodules https://github.com/t-erxleben/marbellous.git` 
2. create an enter the build directory:
	```bash
	cd marbellous
	mkdir build
	cd build
	```
3. Initialize cmake `emcmake cmake ..`.
4. Build WebASM code and collect needed files `emcmake cmake --build . `
5. Collect the needed files `emcmake cmake --install . `
6. the results are now in the `page` subdirectory: `cd page`
7. you may copy this files or serve it with `emrun index.html`

## Copyright notice

We use for the triangulation of our meshes [earcut](https://github.com/mapbox/earcut.hpp).
Thank you mapbox team for providing it.
