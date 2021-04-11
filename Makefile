#
# SPDX-License-Identifier: GPL-2.0
#
# @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
# @license GNU GPL-2.0
#
# <Project Name> - <Short Description>
#
# Copyright (C) 2021  Ammar Faizi
#

VERSION	= 0
PATCHLEVEL = 0
SUBLEVEL = 1
EXTRAVERSION = -rc1
NAME = Blue Tea
TARGET_BIN = template
PACKAGE_NAME = $(TARGET_BIN)-$(VERSION).$(PATCHLEVEL).$(SUBLEVEL)$(EXTRAVERSION)


#
# Bin
#
AS	:= as
CC 	:= cc
CXX	:= c++
LD	:= $(CXX)
VG	:= valgrind
RM	:= rm
MKDIR	:= mkdir
STRIP	:= strip
OBJCOPY	:= objcopy
OBJDUMP	:= objdump
READELF	:= readelf


LIB_LDFLAGS	:= -lpthread
LDFLAGS		:= -fPIC -fpic -ggdb3
CFLAGS		:= -std=c11
CXXFLAGS	:= -std=c++2a

# `C_CXX_FLAGS` will be appended to `CFLAGS` and `CXXFLAGS`.
C_CXX_FLAGS := \
	-fPIC \
	-fpic \
	-ggdb3 \
	-fstrict-aliasing \
	-fstack-protector-strong \
	-fno-omit-frame-pointer \
	-fdata-sections \
	-ffunction-sections \
	-pedantic-errors \
	-D_GNU_SOURCE \
	-DVERSION=$(VERSION) \
	-DPATCHLEVEL=$(PATCHLEVEL) \
	-DSUBLEVEL=$(SUBLEVEL) \
	-DEXTRAVERSION="\"$(EXTRAVERSION)\"" \
	-DNAME="\"$(NAME)\""

ifndef DEFAULT_OPTIMIZATION
	DEFAULT_OPTIMIZATION = -O0
endif

C_CXX_FLAGS_DEBUG := $(DEFAULT_OPTIMIZATION)
C_CXX_FLAGS_RELEASE := -O3 -DNDEBUG

STACK_USAGE_SIZE := 2097152

GCC_WARN_FLAGS := \
	-Wall \
	-Wextra \
	-Wformat \
	-Wformat-security \
	-Wformat-signedness \
	-Wsequence-point \
	-Wstrict-aliasing=3 \
	-Wstack-usage=$(STACK_USAGE_SIZE) \
	-Wunsafe-loop-optimizations

CLANG_WARN_FLAGS := \
	-Wall \
	-Wextra \
	-Weverything \
	-Wno-padded \
	-Wno-unused-macros \
	-Wno-covered-switch-default \
	-Wno-disabled-macro-expansion \
	-Wno-language-extension-token \
	-Wno-used-but-marked-unused


BASE_DIR	:= $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
BASE_DIR	:= $(strip $(patsubst %/, %, $(BASE_DIR)))
BASE_DEP_DIR	:= $(BASE_DIR)/.deps
MAKEFILE_FILE	:= $(lastword $(MAKEFILE_LIST))

ifneq ($(words $(subst :, ,$(BASE_DIR))), 1)
$(error Source directory cannot contain spaces or colons)
endif

include $(BASE_DIR)/src/build/flags.make
include $(BASE_DIR)/src/build/print.make

#######################################
# Force these to be a simple variable
TESTS		:=
TEST_TVAR	:=
TEST_EXEC	:=
OBJ_CC		:=
OBJ_PRE_CC	:=
OBJ_TMP_CC	:=
CFLAGS_TMP	:=
SHARED_LIB	:=
obj-y		:=
#######################################

include $(BASE_DIR)/src/Makefile
include $(BASE_DIR)/tests/Makefile



all: $(TARGET_BIN)



$(TARGET_BIN): $(OBJ_CC) $(OBJ_PRE_CC) $(obj-y)
	$(LD_PRINT)
	$(Q)$(LD) $(LDFLAGS) $(^) -o "$(@)" $(LIB_LDFLAGS)



#
# Create dependendy directory
#
$(DEP_DIRS):
	$(MKDIR_PRINT)
	$(Q)$(MKDIR) -p $(@)



#
# Compile object from main Makefile (this Makefile).
#
$(OBJ_CC):
	$(CC_PRINT)
	$(Q)$(CC) $(DEPFLAGS) $(CFLAGS) -c $(O_TO_C) -o $(@)



#
# Add more dependency chain to object that is not
# compiled from the main Makefile
#
$(OBJ_CC): $(MAKEFILE_FILE) | $(DEP_DIRS)
$(OBJ_PRE_CC): $(MAKEFILE_FILE) | $(DEP_DIRS)

#
# Include generated dependencies
#
-include $(obj-y:$(BASE_DIR)/%.o=$(BASE_DEP_DIR)/%.d)
-include $(OBJ_CC:$(BASE_DIR)/%.o=$(BASE_DEP_DIR)/%.d)
-include $(OBJ_PRE_CC:$(BASE_DIR)/%.o=$(BASE_DEP_DIR)/%.d)


clean:
	$(Q)$(RM) -vrf $(TARGET_BIN) $(DEP_DIRS) $(OBJ_CC) $(OBJ_PRE_CC)




.PHONY: all clean
