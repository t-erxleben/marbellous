# Marbellous

Marbellous will be a WebApp to generate [paper marbling](https://en.wikipedia.org/wiki/Paper_marbling) patterns.
We aim for the best possible performance by using WebASM and WebGL.
The project is still in an early state so most of the functionality is still missing.

## How to use

You can play with the current version at [github pages](https://t-erxleben.github.io/marbellous).

![preview-status](https://github.com/t-erxleben/marbellous/actions/workflows/main.yml/badge.svg)

### Use it local

**required tooling**

* [cmake](https://cmake.org/)
* [emscripten](https://emscripten.org/index.html)

1. Clone the repository and sub-repositories with: `git clone --recurse-submodules https://github.com/t-erxleben/marbellous.git` 
2. Create and enter the build directory:
	```bash
	cd marbellous
	mkdir build
	cd build
	```
3. Make sure to load the emscripten environment.
4. Initialize cmake `emcmake cmake ..`.
5. Build WebASM code and collect needed files `emcmake cmake --build . `
6. Collect the needed files `emcmake cmake --install . `
7. The results are now in the `page` subdirectory: `cd page`
8. You may copy this files or serve it with `emrun index.html`

## Copyright notice

We use [PEG.js](https://pegjs.org/) to parse syntax. Thank you for this great tooling.
Also a shout out to [fast-png](https://github.com/image-js/fast-png) for that easy there easy to use png encoder.
