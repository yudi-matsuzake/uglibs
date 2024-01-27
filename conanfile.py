import conan
import conan.tools.cmake


class UgLibsRecipte(conan.ConanFile):
    name = 'uglibs'
    version = '0.0.1'

    # binary configuration
    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {'shared': [True, False], 'fPIC': [True, False]}
    default_options = {'shared': False, 'fPIC': True}
    generators = 'CMakeToolchain', 'CMakeDeps'

    # sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = 'CMakeLists.txt'

    def config_options(self):
        if self.settings.os == 'Windows':
            del self.options.fPIC

    def layout(self):
        conan.tools.cmake.cmake_layout(self)

    def requirements(self):
        self.requires('catch2/3.4.0')
        self.requires('range-v3/0.12.0')
        self.requires('spdlog/1.12.0')

    def build(self):
        cmake = conan.tools.cmake.CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = conan.tools.cmake.CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.components['utils'].libs = ['utils']
        self.cpp_info.components['utils'].requires = [
            'catch2/3.4.0',
            'range-v3/0.12.0',
            'spdlog/1.12.0'
        ]
        self.cpp_info.components['utils'].set_property(
            'cmake_target_name',
            'utils')

        self.cpp_info.components['gmt'].libs = ['gmt']
        self.cpp_info.components['gmt'].requires = ['catch2/3.4.0']
        self.cpp_info.components['gmt'].set_property(
            'cmake_target_name',
            'gmt')

        self.cpp_info.components['ia'].libs = ['ai']
        self.cpp_info.components['ia'].requires = ['catch2/3.4.0']
        self.cpp_info.components['ia'].set_property(
            'cmake_target_name',
            'ia')

        self.cpp_info.components['containers'].libs = ['containers']
        self.cpp_info.components['containers'].requires = ['catch2/3.4.0']
        self.cpp_info.components['containers'].set_property(
            'cmake_target_name',
            'containers')
