INCLUDES += -I$(OUT)/include

PERL = perl

$(OUT)/include/MathTables.h: $(HOST_OUTPUT_DIR)/tools/GenerateSineTables$(HOST_EXEEXT) | $(OUT)/include/dirstamp
	@$(NQ)echo "  GEN     $@"
	$(Q)$(HOST_OUTPUT_DIR)/tools/GenerateSineTables$(HOST_EXEEXT) >$@

$(call SRC_TO_OBJ,$(SRC)/Math/FastMath.cpp): $(OUT)/include/MathTables.h
$(call SRC_TO_OBJ,$(SRC)/Math/FastTrig.cpp): $(OUT)/include/MathTables.h
$(call SRC_TO_OBJ,$(SRC)/Computer/ThermalRecency.cpp): $(OUT)/include/MathTables.h

$(OUT)/include/InputEvents_Text2Event.cpp: $(SRC)/Input/InputEvents.hpp \
	$(topdir)/tools/Text2Event.pl | $(OUT)/include/dirstamp
	@$(NQ)echo "  GEN     $@"
	$(Q)$(PERL) $(topdir)/tools/Text2Event.pl $< >$@.tmp
	@mv $@.tmp $@

$(OUT)/include/InputEvents_Text2GCE.cpp: $(SRC)/Input/InputQueue.hpp \
	$(topdir)/tools/Text2GCE.pl | $(OUT)/include/dirstamp
	@$(NQ)echo "  GEN     $@"
	$(Q)$(PERL) $(topdir)/tools/Text2GCE.pl $< >$@.tmp
	@mv $@.tmp $@

$(OUT)/include/InputEvents_Text2NE.cpp: $(SRC)/Input/InputQueue.hpp \
	$(topdir)/tools/Text2NE.pl | $(OUT)/include/dirstamp
	@$(NQ)echo "  GEN     $@"
	$(Q)$(PERL) $(topdir)/tools/Text2NE.pl $< >$@.tmp
	@mv $@.tmp $@

$(OUT)/include/InputEvents_Char2GCE.cpp: $(SRC)/Input/InputQueue.hpp \
	$(topdir)/tools/Char2GCE.pl | $(OUT)/include/dirstamp
	@$(NQ)echo "  GEN     $@"
	$(Q)$(PERL) $(topdir)/tools/Char2GCE.pl $< >$@.tmp
	@mv $@.tmp $@

$(OUT)/include/InputEvents_Char2NE.cpp: $(SRC)/Input/InputQueue.hpp \
	$(topdir)/tools/Char2NE.pl | $(OUT)/include/dirstamp
	@$(NQ)echo "  GEN     $@"
	$(Q)$(PERL) $(topdir)/tools/Char2NE.pl $< >$@.tmp
	@mv $@.tmp $@

XCI_LIST = default
XCI_HEADERS = $(patsubst %,$(OUT)/include/InputEvents_%.cpp,$(XCI_LIST))

ifeq ($(TARGET_IS_OPENVARIO),y)
  GETTEXT_EVENTS = Data/Input/defaultOV.xci
else
  GETTEXT_EVENTS = Data/Input/default.xci
endif

$(OUT)/include/InputEvents_default.cpp: $(topdir)/$(GETTEXT_EVENTS) \
	$(topdir)/tools/xci2cpp.pl \
	| $(OUT)/include/dirstamp
	@$(NQ)echo "  GEN     $@"
	$(Q)$(PERL) $(topdir)/tools/xci2cpp.pl $< >$@.tmp
	@mv $@.tmp $@

$(call SRC_TO_OBJ,$(SRC)/Input/InputDefaults.cpp): $(XCI_HEADERS)
$(call SRC_TO_OBJ,$(SRC)/Input/InputLookup.cpp): $(OUT)/include/InputEvents_Text2Event.cpp $(OUT)/include/InputEvents_Text2GCE.cpp $(OUT)/include/InputEvents_Text2NE.cpp

$(call SRC_TO_OBJ,$(SRC)/lua/InputEvent.cpp): $(OUT)/include/InputEvents_Char2GCE.cpp $(OUT)/include/InputEvents_Char2NE.cpp

$(OUT)/include/Status_defaults.cpp: Data/Status/default.xcs \
	tools/xcs2cpp.pl | $(OUT)/include/dirstamp
	@$(NQ)echo "  GEN     $@"
	$(Q)$(PERL) tools/xcs2cpp.pl $< >$@.tmp
	@mv $@.tmp $@

SM_OBJ = $(call SRC_TO_OBJ,$(SRC)/StatusMessage.cpp)
$(SM_OBJ): $(OUT)/include/Status_defaults.cpp

generate:: $(OUT)/include/MathTables.h $(XCI_HEADERS) \
	$(OUT)/include/Status_defaults.cpp \
	$(OUT)/include/InputEvents_Text2Event.cpp $(OUT)/include/InputEvents_Text2GCE.cpp $(OUT)/include/InputEvents_Text2NE.cpp \
	$(OUT)/include/InputEvents_Char2GCE.cpp $(OUT)/include/InputEvents_Char2NE.cpp

# UNIX resources

ifeq ($(USE_WIN32_RESOURCES),n)

ifeq ($(TARGET_IS_ANDROID),n)

$(TARGET_OUTPUT_DIR)/include/resource_data.h: $(TARGET_OUTPUT_DIR)/resources.txt \
	$(RESOURCE_FILES) \
	tools/GenerateResources.pl | $(TARGET_OUTPUT_DIR)/include/dirstamp
	@$(NQ)echo "  GEN     $@"
	$(Q)$(PERL) tools/GenerateResources.pl $< >$@.tmp
	@mv $@.tmp $@

$(call SRC_TO_OBJ,$(SRC)/ResourceLoader.cpp): $(TARGET_OUTPUT_DIR)/include/resource_data.h

generate:: $(TARGET_OUTPUT_DIR)/include/resource_data.h

endif # !TARGET_IS_ANDROID

endif
