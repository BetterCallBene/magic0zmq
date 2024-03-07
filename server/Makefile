
# Formatting variables #################################
BOLD := $(shell tput -T linux bold)
PURPLE := $(shell tput -T linux setaf 5)
GREEN := $(shell tput -T linux setaf 2)
CYAN := $(shell tput -T linux setaf 6)
RED := $(shell tput -T linux setaf 1)
RESET := $(shell tput -T linux sgr0)
TITLE := $(BOLD)$(PURPLE)
SUCCESS := $(BOLD)$(GREEN)

# Default target (i.e., goal) should be first in the makefile
.PHONY: all
all: build/compile_commands.json  ## Build package with cmake
	cmake --build build --config Debug

# We call python here, since this works on linux and windows
AR_DEV_ARPM_BASE := $(shell python3 -c "import os; import re; print(re.findall(r'(?:\s+|\:)AR_DEV_ARPM_BASE\s*=\s*\"(.*)\"', os.popen('arpm_env --no-update \"ar-dev/[^3.x]@ar/stable\"').read(), re.MULTILINE)[0])")
include $(AR_DEV_ARPM_BASE)/make/ar-dev.mk


ifeq ($(OS),Windows_NT)
	SCRIPT_EXT := bat
	# to activate in cmd, do not prepend with '.'
	ACTIVATE_SCRIPT := build\activate.bat
	ACTIVATE_RUN_SCRIPT := build\activate_run.bat
	RM_CMD := rmdir /S /Q
	MKDIR_CMD := mkdir
	CMAKE_GENERATOR := Visual Studio 17 2022
else
	SCRIPT_EXT := sh
	ACTIVATE_SCRIPT := . build/activate.sh
	ACTIVATE_RUN_SCRIPT := . build/activate_run.sh
	RM_CMD := rm -rf
	MKDIR_CMD := mkdir -p
	CMAKE_GENERATOR := Unix Makefiles
endif

# Set default build type to Debug if env var is not set
SERVER_BUILD_TYPE := $(or $(SERVER_BUILD_TYPE),Debug)
SERVER_BUILD_TYPE_LOWER  = $(shell echo $(SERVER_BUILD_TYPE) | tr A-Z a-z)

# The Coverage build is built using CMake Coverage configuration, but
# using the Release Conan package. The reason is that the finders for
# CMake in the Coverage build target Release libraries. This doesn't
# affect the coverage, as the RTI libraries are not built from source.
ifeq ($(SERVER_BUILD_TYPE),Coverage)
	ifeq ($(OS),Windows_NT)
		SERVER_CONAN_SETTINGS := -s build_type=Release
	else
		SERVER_CONAN_SETTINGS := -s build_type=Debug
	endif
else
	SERVER_CONAN_SETTINGS := -s build_type=$(SERVER_BUILD_TYPE)
endif

.PHONY: package
package:  ## Build conan package with ar/stable
	$(call title,Running conan create 'ar/stable')
	conan create . ar/stable $(SERVER_CONAN_SETTINGS) $(CONAN_OPTIONS)

.PHONY: package_testing
package_testing:  ## Build conan package with ${USER}/testing
	$(call title,Running conan create '${USER}/testing')
	conan create . ${USER}/testing $(SERVER_CONAN_SETTINGS) $(CONAN_OPTIONS)

.PHONY: clean
clean:  ## Clean build directory
	$(RM_CMD) build

.PHONY: test
test: all  ## Run tests
	$(call title,Running unit tests)
	$(ACTIVATE_SCRIPT) &&\
	$(ACTIVATE_RUN_SCRIPT) &&\
	$(MAKE) -C build test

.PHONY: test_coverage
test_coverage: SERVER_BUILD_TYPE=Coverage
test_coverage: build/compile_commands.json  ## Run tests with coverage
	$(call title,Running unit tests with coverage)
	$(MAKE) -C build all && \
	$(MAKE) -C build test && \
	$(ACTIVATE_SCRIPT) &&\
	$(ACTIVATE_RUN_SCRIPT) &&\
	gcovr --xml-pretty --exclude-unreachable-branches --print-summary -o build/coverage.xml -e 'build/*' --object-directory "build" --root "." && \
	mkdir -p build/coverage_html && \
	gcovr --html --html-details --exclude-unreachable-branches --print-summary -o build/coverage_html/index.html -e 'build/*' --object-directory "build" --root "."


# Assumed that the target is running from the root directory
.PHONY: test_with_conan
test_with_conan:
	conan create ./tests/conan/ $(CONAN_OPTIONS) $(SERVER_CONAN_SETTINGS)


build:
	$(MKDIR_CMD) build

build_clang:
	$(MKDIR_CMD) build_clang

build/conanbuildinfo.cmake: build
	conan install $(CONAN_OPTIONS) $(SERVER_CONAN_SETTINGS) -g virtualrunenv -g virtualenv -g cmake -g cmake_find_package -if build .

build/compile_commands.json: build/conanbuildinfo.cmake
	$(ACTIVATE_SCRIPT) &&\
	$(ACTIVATE_RUN_SCRIPT) &&\
	cd build &&\
	cmake\
		-DCMAKE_BUILD_TYPE=$(SERVER_BUILD_TYPE)\
		-G "$(CMAKE_GENERATOR)"\
		-DAR_BUILD_TESTS=ON\
		-DCMAKE_MODULE_PATH=${CURDIR}/build\
		-DCMAKE_PREFIX_PATH=${CURDIR}/build\
		..

build_clang/conanbuildinfo.cmake: build_clang
	conan install $(SERVER_CONAN_SETTINGS) $(CONAN_OPTIONS)\
		--profile conan/clang_13\
		-g cmake\
		-g cmake_find_package\
		-g virtualenv\
		-g virtualrunenv \
		-if build_clang\
		.


build_clang/compile_commands.json: build_clang/conanbuildinfo.cmake
	cd build_clang &&\
	. ./activate.sh &&\
	. ./activate_run.sh &&\
	cmake\
		-DCMAKE_BUILD_TYPE=$(SERVER_BUILD_TYPE)\
		-DAR_BUILD_TESTS=ON\
		-DCMAKE_MODULE_PATH=${CURDIR}/build_clang\
		-DCMAKE_PREFIX_PATH=${CURDIR}/build_clang\
		..

.PHONY: docs
docs: build/compile_commands.json  ## Generate documentation
	$(call title,Running docs generation)
	cmake --build build --target docs

.PHONY: icons
icons:  ## Generate icons from svg
	inkscape -f docs/server.svg -e docs/server.png
	convert docs/server.png -scale 48 docs/server.ico

.PHONY: version_test
version_test:  ## Check if it is possible to bump version
	$(call title,Running version bump test)
	@echo "Checking if bimp2version is installed"
	@command -v bump2version >/dev/null 2>&1 || { echo >&2 "bump2version is not installed. Install it."; pip install bump2version -q ; }
	# Check if it is possible to bump version
    # e.g. all versions in files are the same and eq
    # to version in .bumpversion.cfg
	bump2version --dry-run --allow-dirty --verbose major

## Halp! #################################

.PHONY: help
help:  ## Display this help
	@grep -Eh '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "$(BOLD)$(CYAN)%-25s$(RESET)%s\n", $$1, $$2}'
