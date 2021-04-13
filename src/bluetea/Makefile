#
# SPDX-License-Identifier: GPL-2.0
#
# @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
# @license GNU GPL-2.0
#
# Makefile for BlueTea C framework
#
# Copyright (C) 2021  Ammar Faizi
#

DEP_DIRS += $(BASE_DEP_DIR)/src/bluetea
INCLUDE_DIR += $(BASE_DIR)/src/bluetea/include

#######################################
# Force this to be a simple variable
FBT_CC_OBJ :=
#######################################

include $(BASE_DIR)/src/bluetea/lib/Makefile
include $(BASE_DIR)/src/bluetea/teatest/Makefile
include $(BASE_DIR)/src/bluetea/tests/Makefile


bluetea: libbluetea.so



libbluetea.so: $(FBT_CC_OBJ)
	$(LD_PRINT)
	$(Q)$(LD) $(PIC_FLAGS) $(LDFLAGS) $(^) -o "$(@)" $(LIB_LDFLAGS) -shared



#
# Compile all $(FBT_CC_OBJ)
#
$(FBT_CC_OBJ):
	$(CC_PRINT)
	$(Q)$(CC) $(PIC_FLAGS) $(DEPFLAGS) $(CFLAGS) -c $(O_TO_C) -o $(@)



$(FBT_CC_OBJ): | $(DEP_DIRS)


.PHONY: bluetea
