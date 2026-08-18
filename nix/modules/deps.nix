# Assembles nativeBuildInputs, buildInputs, propagatedBuildInputs.
{ pkgs }:

{
  nativeBuildInputs = with pkgs; [
    cmake
    ninja
    pkg-config
  ];

  buildInputs = with pkgs; [
    range-v3
    spdlog
    fmt
    glm
    glfw3
    boost
    libGL
    mesa
    imgui-docking
    (pkgs.callPackage ./glad-src.nix { inherit pkgs; })
    catch2_3
  ];

  propagatedBuildInputs = [ ];
}
