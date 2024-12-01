BUILD_DIR := ./build
SRC_DIR := ./src
LIB_DIR := ./lib
ASSETS_DIR := ./assets
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

.PHONY: clean all

all:
	mkdir -p $(BUILD_DIR)
	$(EMCC) -o $(BUILD_DIR)/$(OUT_WEB) $(EMCCFLAGS) $(INCFLAGS) $(SRCS)

%.c:
	mkdir -p $(BUILD_DIR)
	$(EMCC) -o $(BUILD_DIR)/$(OUT_WEB) $(EMCCFLAGS) $(INCFLAGS) $(SRC_DIR)/$@

clean:
	rm -r $(BUILD_DIR)

