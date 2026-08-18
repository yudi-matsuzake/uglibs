# Build parameters - single source of truth for Nix-driven CMake knobs.
{ lib }:

let
  defaultParams = {
    toolchain = "gcc";      # "gcc" | "clang"
    buildType = "Release";  # "Release" | "Debug" | "RelWithDebInfo"
    generator = "Ninja";    # "Ninja" | "Unix Makefiles"
    sanitizer = "none";     # "none" | "asan" | "tsan" | "ubsan" | "msan"
    cxxStandard = 23;
    enableTesting = true;
    enablePCH = false;
    shared = false;         # -> BUILD_SHARED_LIBS
    pic = true;             # -> CMAKE_POSITION_INDEPENDENT_CODE
    glad = {
      spec = "gl";
      glVersion = "4.6";
      profile = "core";
    };
    imguiTag = "v1.91.4-docking";
  };
in
{
  inherit defaultParams;

  buildDirName = p:
    "build-${p.toolchain}-${p.buildType}"
    + lib.optionalString (p.sanitizer != "none") "-${p.sanitizer}";

  sanitizerFlags = sanitizer:
    let
      base = [ "-fno-omit-frame-pointer" "-g" ];
    in
    {
      "none" = { cxx = [ ]; ld = [ ]; runtimeEnv = { }; };
      "asan" = {
        cxx = base ++ [ "-fsanitize=address" ];
        ld = [ "-fsanitize=address" ];
        runtimeEnv = { ASAN_OPTIONS = "detect_leaks=1:abort_on_error=1"; };
      };
      "tsan" = {
        cxx = base ++ [ "-fsanitize=thread" ];
        ld = [ "-fsanitize=thread" ];
        runtimeEnv = { TSAN_OPTIONS = "halt_on_error=1"; };
      };
      "ubsan" = {
        cxx = base ++ [ "-fsanitize=undefined" ];
        ld = [ "-fsanitize=undefined" ];
        runtimeEnv = { UBSAN_OPTIONS = "halt_on_error=1"; };
      };
      "msan" = {
        cxx = base ++ [ "-fsanitize=memory" ];
        ld = [ "-fsanitize=memory" ];
        runtimeEnv = { MSAN_OPTIONS = "halt_on_error=1"; };
      };
    }.${sanitizer};
}
