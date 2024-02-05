import os.path
import conan
import conan.tools.cmake
import conan.tools.files as files


class UgLibsRecipe(conan.ConanFile):
    name = 'uglibs'
    version = '0.0.1'

    # optional metadata
    license = "MIT license"
    author = "Yudi Matsuzake yudi.matsuzake@gmail.com"
    url = "https://gitlab.com/yudi-matsuzake/uglibs"
    description = "uglibs is a collection of c++ libraries."
    topics = (
        "opengl", "containers", "graphics", "linear algebra", "modern cpp",
        "metaprogramming", "render", "run-length encoding",
        "interval arithmetics", "numeric computing", "ranges", "cpp20",
        "tight integers container"
    )

    # binary configuration
    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {'shared': [True, False], 'fPIC': [True, False]}
    default_options = {'shared': False, 'fPIC': True}
    generators = 'CMakeToolchain', 'CMakeDeps'

    # sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = 'CMakeLists.txt', 'libs/*'

    def layout(self):
        conan.tools.cmake.cmake_layout(self)

    def requirements(self):
        # all
        self.requires('catch2/3.4.0', test=True)

        # util
        self.requires(
            'range-v3/0.12.0',
            transitive_headers = True,
            transitive_libs = True)
        self.requires(
            'spdlog/1.12.0',
            transitive_headers = True,
            transitive_libs = True)

        # graphics
        self.requires(
            'glm/0.9.9.8',
            transitive_headers = True,
            transitive_libs = True)
        self.requires('glad/0.1.36',
            transitive_headers = True,
            transitive_libs = True)
        self.requires(
            'imgui/cci.20220621+1.88.docking',
            transitive_headers = True,
            transitive_libs = True)
        self.requires(
            'glfw/3.3.8',
            transitive_headers = True,
            transitive_libs = True)
        self.requires(
            'boost/1.83.0',
            transitive_headers = True,
            transitive_libs = True)

    def configure(self):
        self.options['glad'].spec = 'gl'
        self.options['glad'].gl_version = '4.6'
        self.options['glad'].gl_profile = 'core'

    def generate(self):
        files.copy(
            self,
            "*glfw*",
            os.path.join(
                self.dependencies["imgui"].package_folder,
                "res",
                "bindings"),
            os.path.join(self.build_folder, "imgui-bindings"))

        files.copy(
            self,
            "*opengl3*",
            os.path.join(
                self.dependencies["imgui"].package_folder,
                "res", "bindings"),
            os.path.join(self.build_folder, "imgui-bindings"))

    def build(self):
        cmake = conan.tools.cmake.CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = conan.tools.cmake.CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.components['util'].libs = ['util']
        self.cpp_info.components['util'].requires = [
            'range-v3::range-v3',
            'spdlog::spdlog',
        ]
        self.cpp_info.components['util'].set_property(
            'cmake_target_name',
            'util')

        self.cpp_info.components['gmt'].requires = ['util']
        self.cpp_info.components['gmt'].set_property(
            'cmake_target_name',
            'gmt')

        self.cpp_info.components['ia'].libs = ['ia']
        self.cpp_info.components['ia'].requires = ['util']
        self.cpp_info.components['ia'].set_property(
            'cmake_target_name',
            'ia')

        self.cpp_info.components['containers'].requires = ['util']
        self.cpp_info.components['containers'].set_property(
            'cmake_target_name',
            'containers')

        self.cpp_info.components['graphics'].libs = [
            'graphics', 'ugimgui'
        ]

        self.cpp_info.components['graphics'].requires = [
            'util',
            'glm::glm',
            'glad::glad',
            'imgui::imgui',
            'glfw::glfw',
            'boost::boost'
        ]

        self.cpp_info.components['graphics'].set_property(
            'cmake_target_name',
            'graphics')
