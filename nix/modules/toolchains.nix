# Maps params.toolchain to a Nix stdenv.
{ pkgs, lib }:

{
  stdenvFor = params:
    if params.toolchain == "gcc" then
      pkgs.stdenv
    else if params.toolchain == "clang" then
      pkgs.clangStdenv
    else
      throw "uglibs: unknown toolchain '${params.toolchain}'";
}
