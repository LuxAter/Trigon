
SHELL=/bin/bash
ifndef .VERBOSE
    .SILENT:
endif
ROOT=$(shell pwd)
ROOT=/home/arden/Programming/cpp/trigon
CXX=clang++
CC=clang
CCIGNORE=
CXXIGNORE=
CCFLAGS=-fPIC -Wall -Wpedantic --static
CXXFLAGS=-std=c++17 -fPIC -Wall -Wpedantic --static -O3
LINK=-lz
SOURCE=src
INCLUDE_DIR=include
INCLUDE=-I$(ROOT)/$(BUILD)/libpng/include
BUILD=build
COMMON_INCLUDE=-I$(ROOT)/$(INCLUDE_DIR) $(INCLUDE)

SCAN_COLOR=\033[1;35m
BUILD_COLOR=\033[32m
CLEAN_COLOR=\033[1;33m
LINK_COLOR=\033[1;32m
INSTALL_COLOR=\033[1;36m
CMD_COLOR=\033[1;34m
HELP_COLOR=\033[1;34m

define scan_target
printf "%b%s%b\n" "$(SCAN_COLOR)" "Scaning dependencies for target $(1)" "\033[0m"
endef
define complete_target
printf "%s\n" "Built target $(1)"
endef
define clean_target
printf "%b%s%b\n" "$(CLEAN_COLOR)" "Cleaning target $(1)" "\033[0m"
endef
define install_target
printf "%b%s%b\n" "$(INSTALL_COLOR)" "Installing target $(1)" "\033[0m"
endef
define uninstall_target
printf "%b%s%b\n" "$(INSTALL_COLOR)" "Unnstalling target $(1)" "\033[0m"
endef
define print_build_c
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(BUILD_COLOR)" "Building C object $$str" "\033[0m"
endef
define print_build_cpp
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(BUILD_COLOR)" "Building C++ object $$str" "\033[0m"
endef
define print_link_lib
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(LINK_COLOR)" "Linking static library $$str" "\033[0m"
endef
define print_link_exe
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(LINK_COLOR)" "Linking executable $$str" "\033[0m"
endef
define print_run_cmd
printf "%b%s%b\n" "$(CMD_COLOR)" "Running '$(1)'" "\033[0m"
endef
define help
printf "%b%*s%b: %s\n" "$(HELP_COLOR)" 20 "$(1)" "\033[0m" "$(2)"
endef

all: build-trigon
clean: clean-trigon
install: install-trigon
uninstall: uninstall-trigon

# TRIGON {{{

TRIGON=$(ROOT)/trigon
TRIGON_FILES=$(shell find "src/" -name "*.cpp")
TRIGON_OBJS=$(TRIGON_FILES:%=$(ROOT)/$(BUILD)/%.o)
-include $(TRIGON_OBJS:.o=.d)

build-trigon: build-libpng.a pre-trigon $(TRIGON)
	$(call complete_target,$(shell basename $(TRIGON)))

clean-trigon:
	$(call clean_target,$(shell basename $(TRIGON)))
	if [ -e "$(TRIGON)" ]; then rm $(TRIGON); fi

pre-trigon:
	$(call scan_target,$(shell basename $(TRIGON)))

$(TRIGON): $(TRIGON_OBJS) FORCE
	$(call print_link_exe,$(shell basename $(TRIGON)))
	$(CXX) $(TRIGON_OBJS) $(BUILD)/libpng.a/lib/libpng.a $(LINK) $(COMMON_INCLUDE) -o $(TRIGON)

install-trigon: build-trigon
	$(call install_target,$(shell basename $(TRIGON)))
	mkdir -p $(INSTALL_PATH)/bin/
	cp $(TRIGON) $(INSTALL_PATH)/bin

uninstall-trigon:
	$(call uninstall_target,$(shell basename $(TRIGON)))
	if [ -e "$(INSTALL_PATH)/bin/$(shell basename $(TRIGON))" ]; then rm $(INSTALL_PATH)/bin/$(shell basename $(TRIGON)); fi

# }}}
# LIBPNG.A {{{

build-libpng.a: pre-libpng.a
	if [ ! -f "libpng/configure" ]; then $(call print_run_cmd,autogen.sh) && cd libpng && ./autogen.sh; fi
	if [ ! -f "libpng/Makefile" ]; then $(call print_run_cmd,configure) && cd libpng && ./configure --prefix=$(ROOT)/build/libpng.a; fi
	if [ ! -d "$(ROOT)/build/libpng.a" ]; then $(call print_run_cmd,make) && cd libpng && make install; fi
	$(call complete_target,libpng.a)

clean-libpng.a:
	$(call clean_target,libpng.a)
	if [ -e "libpng/Makefile" ]; then cd libpng && make clean && rm Makefile; fi
	if [ -d "$(ROOT)/build/libpng.a" ]; then rm $(ROOT)/build/libpng.a -r; fi

pre-libpng.a:
	$(call scan_target,libpng.a)

# }}}

$(ROOT)/$(BUILD)/%.cpp.o: %.cpp
	$(call print_build_cpp,$@)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -c $(COMMON_INCLUDE) $< -o $@

$(ROOT)/$(BUILD)/%.c.o: %.c
	$(call print_build_c,$@)
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) -MMD -c $(COMMON_INCLUDE) $< -o $@

FORCE:
