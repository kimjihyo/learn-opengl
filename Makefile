BUILD_DIR := ./build
SRC_DIR := ./src
LIB_DIR := ./lib
OUT_WEB := index.html

EMCC := emcc

EMCCFALGS :=
EMCCFLAGS += -std=gnu2x
EMCCFLAGS += -g1
EMCCFLAGS += -O2
EMCCFLAGS += -Wall
EMCCFLAGS += -Wextra
EMCCFLAGS += -Wno-unused-parameter
EMCCFLAGS += -Wno-missing-field-initializers
EMCCFLAGS += -Wno-gnu
EMCCFLAGS += -s USE_SDL=2
EMCCFLAGS += -s FULL_ES3=1
EMCCFLAGS += -s ABORT_ON_WASM_EXCEPTIONS=1
EMCCFLAGS += -s ALLOW_MEMORY_GROWTH=1

# Find all the C files to compile
SRCS := $(shell find $(SRC_DIR) -name "*.c")

# Prepend BUILD_DIR and append .o to every src file
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INCDIRS := $(shell find $(SRC_DIRS) -type d)

# Add a prefix to INCDIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INCFLAGS := $(addprefix -iquote,$(INCDIRS))
INCFLAGS += -I$(LIB_DIR)

# Add SDL3 includes and libraries
INCFLAGS += $(shell sdl2-config --cflags)

.PHONY: clean all

all:
	mkdir -p $(BUILD_DIR)
	$(EMCC) -o $(BUILD_DIR)/$(OUT_WEB) $(EMCCFLAGS) $(INCFLAGS) $(SRCS)

%.c:
	mkdir -p $(BUILD_DIR)
	$(EMCC) -o $(BUILD_DIR)/$(OUT_WEB) $(EMCCFLAGS) $(INCFLAGS) $(SRC_DIR)/$@

clean:
	rm -r $(BUILD_DIR)

