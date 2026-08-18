# uglibs build derivation - Nix chooses every build parameter,
# feeds them to cmake as -D flags and env vars.
{ pkgs, params, src }:

let
  paramsMod = import ./modules/params.nix { inherit (pkgs) lib; };
  toolchainsMod = import ./modules/toolchains.nix { inherit pkgs; lib = pkgs.lib; };
  deps = import ./modules/deps.nix { inherit pkgs; };

  stdenv = toolchainsMod.stdenvFor params;
  buildDir = paramsMod.buildDirName params;
  san = paramsMod.sanitizerFlags params.sanitizer;

  cxxFlags = pkgs.lib.concatStringsSep " " san.cxx;
  ldFlags = pkgs.lib.concatStringsSep " " san.ld;
in
stdenv.mkDerivation {
  pname = "uglibs";
  version = "0.0.1";

  inherit src;

  inherit (deps) nativeBuildInputs buildInputs propagatedBuildInputs;

  IMGUI_DIR = pkgs.imgui-docking-src;
  env = san.runtimeEnv;

  cmakeFlags = [
    "-G" params.generator
    "-S" "."
    "-B" buildDir
    "-DCMAKE_BUILD_TYPE=${params.buildType}"
    "-DCMAKE_CXX_STANDARD=${toString params.cxxStandard}"
    "-DBUILD_SHARED_LIBS=${if params.shared then "ON" else "OFF"}"
    "-DCMAKE_POSITION_INDEPENDENT_CODE=${if params.pic then "ON" else "OFF"}"
    "-DENABLE_TESTING=${if params.enableTesting then "ON" else "OFF"}"
    "-DENABLE_PCH=${if params.enablePCH then "ON" else "OFF"}"
    "-DIMGUI_DIR=${pkgs.imgui-docking-src}"
    "-DCMAKE_CXX_FLAGS=${cxxFlags}"
    "-DCMAKE_EXE_LINKER_FLAGS=${ldFlags}"
  ];

  preConfigure = ''
    test -d "$IMGUI_DIR" || {
      echo "uglibs: IMGUI_DIR=$IMGUI_DIR is missing" >&2
      exit 1
    }
  '';

  # stdenv's cmake hook appends ".." which breaks our -S . -B <buildDir>.
  dontUseCmakeConfigure = true;

  configurePhase = ''
    runHook preConfigure
    cmake $cmakeFlags
    runHook postConfigure
  '';

  buildPhase = ''
    runHook preBuild
    cmake --build "${buildDir}" --parallel "$NIX_BUILD_CORES"
    runHook postBuild
  '';

  installPhase = ''
    runHook preInstall
    cmake --install "${buildDir}" --prefix "$out"
    runHook postInstall
  '';

  doCheck = false;

  meta = with pkgs.lib; {
    description = "uglibs - collection of C++ libraries";
    license = licenses.boost;
    platforms = platforms.linux;
  };
}
