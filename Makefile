BUILD_DIR := ./build
SRC_DIR := ./src
LIB_DIR := ./lib
ASSETS_DIR := ./assets
SHADER_DIR := $(ASSETS_DIR)/shaders
SHADER_SRC := $(shell find $(SHADER_DIR) -name *.glsl)

OUT_WEB := index.html
OUT_SHADER := $(SHADER_SRC:%.glsl=%.glsl.h)

EMCC := emcc
SHDC := sokol-shdc

EMCCFALGS :=
EMCCFLAGS += -std=gnu2x
EMCCFLAGS += -g1
EMCCFLAGS += -O2
EMCCFLAGS += -Wall
EMCCFLAGS += -Wextra
EMCCFLAGS += -Wno-unused-parameter
EMCCFLAGS += -Wno-missing-field-initializers
EMCCFLAGS += -Wno-gnu
EMCCFLAGS += --preload-file $(ASSETS_DIR)
EMCCFLAGS += -s USE_SDL=2
EMCCFLAGS += -s FULL_ES3=1
EMCCFLAGS += -s ABORT_ON_WASM_EXCEPTIONS=1
EMCCFLAGS += -s ALLOW_MEMORY_GROWTH=1

SRCS := $(shell find $(SRC_DIR) -name "*.c")
INCDIRS := $(shell find $(SRC_DIR) -type d)

INCFLAGS := $(addprefix -iquote,$(INCDIRS))
INCFLAGS += -I$(LIB_DIR)
INCFLAGS += $(shell sdl2-config --cflags)

.PHONY: clean all shaders

all:
	mkdir -p $(BUILD_DIR)
	$(EMCC) -o $(BUILD_DIR)/$(OUT_WEB) $(EMCCFLAGS) $(INCFLAGS) $(SRCS)

shaders: $(OUT_SHADER)

$(OUT_SHADER): %.glsl.h : %.glsl
	$(SHDC) --input $^ --output $@ --slang glsl300es --format sokol_impl

%.c:
	mkdir -p $(BUILD_DIR)
	$(EMCC) -o $(BUILD_DIR)/$(OUT_WEB) $(EMCCFLAGS) $(INCFLAGS) $(SRC_DIR)/$@

clean:
	find $(SHADER_DIR) -name "*.glsl.h" -type f -delete
	rm -r $(BUILD_DIR)

