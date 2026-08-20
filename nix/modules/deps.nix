# Assembles nativeBuildInputs, buildInputs, propagatedBuildInputs.
# Propagated deps are visible to downstream consumers via CMake
# find_package and include paths.
{ pkgs }:

let
  gladPkg = pkgs.callPackage ./glad-src.nix { inherit pkgs; };
in
{
  nativeBuildInputs = with pkgs; [
    cmake
    ninja
    pkg-config
    xvfb-run
  ];

  buildInputs = with pkgs; [
    mesa
    catch2_3
  ];

  propagatedBuildInputs = with pkgs; [
    range-v3
    spdlog
    fmt
    glm
    glfw3
    boost
    libGL
    imgui-docking
    gladPkg
  ];
}
