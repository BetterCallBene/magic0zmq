from conans import ConanFile, CMake, tools
import os


class ArProject(ConanFile):
    name = "server"
    version = "0.0.1"  # Handled by bump2version
    license = "Proprietary"
    author = 'Agile Robots AG'
    description = "zmq server"
    # TODO set keyword topics which best summarize your project
    topics = "c++", "example"
    url = 'http://git.ar.int/sd/peac/mv/server'
    homepage = 'http://git.ar.int/sd/peac/mv/server'
    generators = "cmake", "cmake_find_package"
    default_user = 'ar'
    default_channel = 'stable'
    scm = {"revision": "auto", "type": "git", "url": "auto"}
    no_copy_source = True

    # Depending on your package, you should include or remove different settings from here
    # E.g., if 'build_type' is listed, this tells conan that the binaries of your package
    # are different for different build types. If 'build_type' would not be included here,
    # conan will generate the same package ID for different build types.
    settings = "os", "arch", "compiler", "build_type"

    
    def build_requirements(self):
        self.tool_requires("cmake/3.28.1")
        # ar-dev should be a tool_requires since it is only tooling to build the package, but it
        # is not required to run the executables
        self.tool_requires("ar-dev/[^3.x]@ar/stable")

    def requirements(self):
        self.requires("cppzmq/4.8.1")
        self.requires("nlohmann_json/3.11.2")
        pass

    def build(self):
        cmake = CMake(self)
        if tools.get_env("CONAN_RUN_TESTS", True):
            cmake.definitions['AR_BUILD_TESTS'] = True
        cmake.configure()
        cmake.build()
        # if tools.get_env("CONAN_RUN_TESTS", True):
        #     cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

        # Remove pkgconfig files
        tools.rmdir(os.path.join(self.package_folder, "lib", "pkgconfig"))
