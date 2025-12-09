CONAN_PC_DIR := $(CURDIR)/build/conan
export PKG_CONFIG_PATH := $(CONAN_PC_DIR):$(PKG_CONFIG_PATH)

CXXFLAGS += $(shell pkg-config --cflags absl_strings)
LDFLAGS  += $(shell pkg-config --libs absl_strings)
LDFLAGS  += -Wl,-rpath,'$$ORIGIN/build/conan/runtime'

all:
	$(CXX) main.cpp -o app $(CXXFLAGS) $(LDFLAGS)
