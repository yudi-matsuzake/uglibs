# Wraps the vendored GL 4.6 core glad source into a Nix package
# with a synthetic glad-config.cmake so find_package(glad) resolves.
{ pkgs }:

let
  gladSources = ../../libs/graphics/external/glad;

  gladConfigFile = pkgs.writeText "glad-config.cmake" ''
    set(PACKAGE_VERSION "0.1.36")
    set(PACKAGE_VERSION_COMPATIBLE TRUE)

    if(NOT TARGET glad::glad)
      add_library(glad::glad INTERFACE IMPORTED)
      # $out/lib/cmake/glad/glad-config.cmake -> ../../../include, ../../../src
      set_target_properties(glad::glad PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "''${CMAKE_CURRENT_LIST_DIR}/../../../include"
        INTERFACE_SOURCES             "''${CMAKE_CURRENT_LIST_DIR}/../../../src/glad.c")
    endif()
  '';
in
pkgs.stdenv.mkDerivation {
  pname = "glad";
  version = "0.1.36";

  src = gladSources;

  dontConfigure = true;
  dontBuild = true;

  installPhase = ''
    runHook preInstall
    mkdir -p "$out/include" "$out/src" "$out/lib/cmake/glad"
    cp -r include/. "$out/include/"
    cp src/glad.c "$out/src/glad.c"
    cp "${gladConfigFile}" "$out/lib/cmake/glad/glad-config.cmake"
    runHook postInstall
  '';

  meta = with pkgs.lib; {
    description = "glad GL 4.6 core C loader (vendored by uglibs)";
    license = licenses.mit;
    platforms = platforms.all;
  };
}
