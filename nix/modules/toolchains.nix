# Maps params.toolchain to a Nix stdenv.
# msan requires clang's instrumented libc++; eval-time error if gcc is selected.
{ pkgs, lib }:

let
  inherit (lib) throwIf;

  msanNeedsClang = params:
    params.sanitizer == "msan" && params.toolchain != "clang";
in
{
  stdenvFor = params:
    let
      _checked = throwIf (msanNeedsClang params)
        "uglibs: sanitizer 'msan' requires toolchain='clang'"
        params;
    in
    if params.toolchain == "gcc" then
      pkgs.stdenv
    else if params.toolchain == "clang" then
      pkgs.clangStdenv
    else
      throw "uglibs: unknown toolchain '${params.toolchain}'";
}
