load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@emsdk//emscripten_toolchain:wasm_rules.bzl", "wasm_cc_binary")

cc_library(
    name = "color",
    srcs = ["src/Color.cpp"],
    hdrs = ["include/Color.hpp"],
    includes = ["include/"],
)

cc_library(
    name = "palette",
    srcs = ["src/Palette.cpp"],
    hdrs = ["include/Palette.hpp"],
    includes = ["include/"],
    deps = [":color"],
)

cc_library(
    name = "options",
    srcs = ["src/Options.cpp"],
    hdrs = ["include/Options.hpp"],
    includes = ["include/"],
    deps = [":color", ":palette],
)