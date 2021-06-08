#
# SPDX-License-Identifier: GPL-2.0
#
# @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
# @license GNU GPL-2.0
# @link https://github.com/ammarfaizi2/bluetea
#
# Copyright (C) 2021  Ammar Faizi
#

DEP_DIRS += $(BASE_DEP_DIR)/src/bluetea/tests/001_string
THIS_TEST := $(BASE_DIR)/src/bluetea/tests/001_string/001_string.test
THIS_TEST_OBJ := $(THIS_TEST:%.test=%.o)
BLUETEST_LIST += $(THIS_TEST)_do_test
BLUETEST_UNIT_OBJ += $(THIS_TEST_OBJ) $(THIS_TEST)


$(THIS_TEST_OBJ): | $(DEP_DIRS)


$(THIS_TEST_OBJ):
	$(CC_PRINT)
	$(Q)$(CC) $(PIC_FLAGS) $(DEPFLAGS) $(CFLAGS) -c $(O_TO_C) -o $(@)


$(THIS_TEST): $(THIS_TEST_OBJ) $(BLUETEST_SHARED_OBJ)
	$(LD_PRINT)
	$(Q)$(CC) $(PIC_FLAGS) $(DEPFLAGS) $(^) -o $(@) $(LIB_LDFLAGS)


-include $(THIS_TEST_OBJ:$(BASE_DIR)/%.o=$(BASE_DEP_DIR)/%.d)


$(THIS_TEST)_do_test: $(THIS_TEST)
	$(THIS_TEST) do_test $(shell which $(VG))


.PHONY: $(THIS_TEST)_do_test
