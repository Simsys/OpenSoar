// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Dialogs/ComboPicker.hpp"
#include "Dialogs/ListPicker.hpp"
#include "ui/control/List.hpp"
#include "Form/DataField/Base.hpp"
#include "Form/DataField/ComboList.hpp"
#include "Renderer/TextRowRenderer.hpp"
#include "UIGlobals.hpp"
#include "Look/DialogLook.hpp"
#include "util/StaticString.hxx"

static const ComboList *ComboListPopup;

class ComboPickerSupport : public ListItemRenderer {
  const ComboList &combo_list;
  TextRowRenderer row_renderer;

public:
  ComboPickerSupport(const ComboList &_combo_list)
    :combo_list(_combo_list) {}

  unsigned CalculateLayout(const DialogLook &look) {
    return row_renderer.CalculateLayout(*look.list.font);
  }

  virtual void OnPaintItem(Canvas &canvas, const PixelRect rc,
                           unsigned i) noexcept override {
    row_renderer.DrawTextRow(canvas, rc, combo_list[i].display_string.c_str());
  }
};

static const char*
OnItemHelp(unsigned i)
{
  return (*ComboListPopup)[i].help_text.c_str();
}

int
ComboPicker(const char *caption,
            const ComboList &combo_list,
            const char *help_text,
            bool enable_item_help,
            const char *extra_caption)
{
  ComboListPopup = &combo_list;

  ComboPickerSupport support(combo_list);
  return ListPicker(caption,
                    combo_list.size(),
                    combo_list.current_index,
                    support.CalculateLayout(UIGlobals::GetDialogLook()),
                    support, false,
                    help_text,
                    enable_item_help ? OnItemHelp : nullptr,
                    extra_caption);
}

bool
ComboPicker(const char *caption, DataField &df,
            const char *help_text)
{
  StaticString<256> buffer;
  const char *reference = nullptr;

  while (true) {
    const ComboList combo_list = df.CreateComboList(reference);
    ComboListPopup = &combo_list;

    int idx = ComboPicker(caption, combo_list, help_text,
                          df.GetItemHelpEnabled());
    if (idx < 0)
      return false;

    const ComboList::Item &item = combo_list[idx];

    // OK/Select
    if (item.int_value == ComboList::Item::NEXT_PAGE) {
      // we're last in list and the want more past end of list so select last real list item and reopen
      // we'll reopen, so don't call xcsoar data changed routine yet
      reference = buffer = combo_list[idx - 1].string_value.c_str();
    } else if (item.int_value == ComboList::Item::PREVIOUS_PAGE) {
      // same as above but lower items needed
      reference = buffer = combo_list[idx + 1].string_value.c_str();
    } else {
      df.SetFromCombo(item.int_value, item.string_value.c_str());
      return true;
    }
  } // loop reopen combo if <<More>>  or <<Less>> picked
}
