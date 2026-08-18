# Overlay: fetch imgui docking branch and disable bundled backends.
# uglibs rebuilds the backends itself via the ugimgui CMake target.
{ }:

final: prev: {
  imgui-docking-src = prev.fetchFromGitHub {
    owner = "ocornut";
    repo = "imgui";
    rev = "v1.91.4-docking";
    sha256 = "sha256-b0ZXuSXV8U8eBU6WE6blxUvS6xaIgEt9Svob+Kow0g8=";
  };

  imgui-docking = (prev.imgui.override {
    IMGUI_BUILD_GLFW_BINDING = false;
    IMGUI_BUILD_OPENGL3_BINDING = false;
  }).overrideAttrs (_: {
    src = final.imgui-docking-src;
  });
}
