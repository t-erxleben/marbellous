# Build

- First install bazel. (See [Installing Bazel](https://docs.bazel.build/versions/4.0.0/install.html))
- There is no need to install `emsdk` if you just want to build the project. Bazel will fetch it for you.
- To build the example project go to `hello-world` directory and run:
    - `bazel build hello-world-wasm`
- To load the website use `emrun` or a similar tool for a local webserver.
- For further reference see [bazel docs](https://docs.bazel.build/versions/4.0.0/bazel-overview.html)
- Also see [emscripten website](https://emscripten.org/)