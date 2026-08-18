{
  description = "uglibs - collection of C++ libraries (flake-managed)";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" ];
      forAllSystems = fn:
        nixpkgs.lib.genAttrs supportedSystems (system: fn system);

      paramsMod = system:
        import ./nix/modules/params.nix { lib = nixpkgs.lib; };

      overlays = [
        (import ./nix/modules/imgui-docking.nix { })
      ];

      mkDevShell = { pkgs, params, stdenv ? pkgs.stdenv }:
        let
          inherit (paramsMod pkgs.system) buildDirName;
          buildDir = buildDirName params;
          mkShell = pkgs.mkShell.override { inherit stdenv; };
          gladPkg = pkgs.callPackage ./nix/modules/glad-src.nix { inherit pkgs; };
          ugRun = pkgs.writeShellScriptBin "ug-run" ''
            if [ -d /run/opengl-driver ]; then
                exec "$@"
            fi
            _paths=""
            for _p in /usr/lib /usr/lib64 /usr/lib/x86_64-linux-gnu; do
                [ -d "$_p" ] && _paths="''${_paths:+$_paths:}$_p"
            done
            LD_LIBRARY_PATH="$_paths''${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" exec "$@"
          '';
        in
        mkShell {
          packages = with pkgs; [
            cmake
            ninja
            clang-tools
            pkg-config
            catch2_3
            range-v3
            spdlog
            fmt
            glm
            glfw3
            boost
            libGL
            gladPkg
            python3Packages.glad
            imgui-docking
            xvfb-run
            mesa-demos
            gnumake
            ugRun
          ];

          shellHook = ''
            export UGLIBS_BUILD_DIR="${buildDir}"
            export UGLIBS_CXX_STANDARD="${toString params.cxxStandard}"
            export UGLIBS_BUILD_TYPE="${params.buildType}"
            export UGLIBS_TOOLCHAIN="${params.toolchain}"
            export UGLIBS_SANITIZER="${params.sanitizer}"
            export IMGUI_DIR="${pkgs.imgui-docking-src}"

            echo ""
            echo "  uglibs development shell"
            echo "  -------------------------"
            echo "  toolchain  : $UGLIBS_TOOLCHAIN"
            echo "  cxx std    : $UGLIBS_CXX_STANDARD"
            echo "  build type : $UGLIBS_BUILD_TYPE"
            echo "  sanitizer  : $UGLIBS_SANITIZER"
            echo "  build dir  : $UGLIBS_BUILD_DIR"
            echo "  imgui src  : $IMGUI_DIR"
            echo ""
            echo "  configure: cmake -G Ninja -S . -B $UGLIBS_BUILD_DIR"
            echo "  build:     cmake --build $UGLIBS_BUILD_DIR"
            echo "  test:      ctest --test-dir $UGLIBS_BUILD_DIR --output-on-failure"
            echo "  run:       ug-run ./$UGLIBS_BUILD_DIR/libs/graphics/hello-cube"
            echo ""
          '';
        };

      mkBuild = { pkgs, params }:
        import ./nix/derivation.nix {
          inherit pkgs params;
          src = self;
        };

      perSystem = system:
        let
          pkgs = import nixpkgs {
            inherit system;
            inherit overlays;
          };
          inherit (paramsMod system) defaultParams;
        in
        {
          devShells = {
            default = mkDevShell { inherit pkgs; params = defaultParams; };
            clang = mkDevShell {
              inherit pkgs;
              stdenv = pkgs.clangStdenv;
              params = defaultParams // { toolchain = "clang"; };
            };
          };

          packages = {
            default = mkBuild {
              inherit pkgs;
              params = defaultParams // { enableTesting = false; };
            };
            uglibs-gcc = mkBuild {
              inherit pkgs;
              params = defaultParams // { enableTesting = false; };
            };
          };

          lib.params = paramsMod system;
        };

    in
    {
      overlays.default = nixpkgs.lib.composeManyExtensions overlays;
      devShells = forAllSystems (system: (perSystem system).devShells);
      packages = forAllSystems (system: (perSystem system).packages);
      lib = forAllSystems (system: (perSystem system).lib);
    };
}
