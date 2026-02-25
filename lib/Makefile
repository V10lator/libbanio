TARGET   ?= libbanio
CFLAGS   ?= -march=x86-64-v2 -O3 -ffast-math -ftree-vectorize -ftree-slp-vectorize -pipe -mfpmath=sse -flto=8 -funsigned-char -I ./include
LDFLAGS  ?= -shared
BUILD_DIR = build

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

C_SRCS := $(call rwildcard,src,*.c)

# ---- Linux build ----
LIN_OBJS := $(addprefix $(BUILD_DIR)/linux/, $(C_SRCS:.c=.o))
LIN_DEPS := $(addprefix $(BUILD_DIR)/linux/, $(C_SRCS:.c=.d))
LIN_BUILD_DIRS := $(sort $(dir $(LIN_OBJS)))

# ---- Windows build ----
WIN_OBJS := $(addprefix $(BUILD_DIR)/win/, $(C_SRCS:.c=.o))
WIN_DEPS := $(addprefix $(BUILD_DIR)/win/, $(C_SRCS:.c=.d))
WIN_BUILD_DIRS := $(sort $(dir $(WIN_OBJS)))

# ---- All targets ----
all: linux windows

linux: $(BUILD_DIR)/$(TARGET).so

$(BUILD_DIR)/$(TARGET).so: $(LIN_OBJS) | $(BUILD_DIR)
	gcc $(LIN_OBJS) $(CFLAGS) $(LDFLAGS) -o $@

$(BUILD_DIR)/linux/%.o: %.c | $(LIN_BUILD_DIRS)
	gcc $(CFLAGS) $< -MMD -MF $(@:.o=.d) -c -o $@

windows: $(BUILD_DIR)/$(TARGET).dll

$(BUILD_DIR)/$(TARGET).dll: $(WIN_OBJS) | $(BUILD_DIR)
	x86_64-w64-mingw32-gcc $(WIN_OBJS) $(CFLAGS) $(LDFLAGS) -o $@

$(BUILD_DIR)/win/%.o: %.c | $(WIN_BUILD_DIRS)
	x86_64-w64-mingw32-gcc $(CFLAGS) $< -MMD -MF $(@:.o=.d) -c -o $@

$(BUILD_DIR) $(LIN_BUILD_DIRS) $(WIN_BUILD_DIRS):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

-include $(LIN_DEPS)
-include $(WIN_DEPS)

.PHONY: all linux windows clean
