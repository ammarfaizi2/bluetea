#
# SPDX-License-Identifier: GPL-2.0
#
# @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
# @license GNU GPL-2.0
#
# Copyright (C) 2021  Ammar Faizi
#

DEP_DIRS += $(BASE_DEP_DIR)/src/template

OBJ_TMP_CC := \
	$(BASE_DIR)/src/template/main.o

OBJ_PRE_CC += $(OBJ_TMP_CC)


$(OBJ_TMP_CC):
	$(CC_PRINT)
	$(Q)$(CC) $(PIE_FLAGS) $(DEPFLAGS) $(CFLAGS) -c $(O_TO_C) -o $(@)
