// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Dialogs/dlgInfoBoxAccess.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "UIGlobals.hpp"
#include "UIState.hpp"
#include "Screen/Layout.hpp"
#include "Look/DialogLook.hpp"
#include "InfoBoxes/InfoBoxManager.hpp"
#include "InfoBoxes/InfoBoxLayout.hpp"
#include "InfoBoxes/Content/Factory.hpp"
#include "InfoBoxes/Panel/Panel.hpp"
#include "Widget/TabWidget.hpp"
#include "Widget/ActionWidget.hpp"
#include "Widget/ButtonWidget.hpp"
#include "Widget/TwoWidgets.hpp"
#include "Interface.hpp"
#include "Language/Language.hpp"

#include <cassert>
#include <stdio.h>

/**
 * This modal result will trigger the "Switch InfoBox" dialog.
 */
static constexpr int SWITCH_INFO_BOX = 100;

void
dlgInfoBoxAccessShowModeless(const int id, const InfoBoxPanel *panels)
{
  assert (id > -1);

  const InfoBoxSettings &settings = CommonInterface::SetUISettings().info_boxes;
  const unsigned panel_index = CommonInterface::GetUIState().panel_index;
  const InfoBoxSettings::Panel &panel = settings.panels[panel_index];
  const InfoBoxFactory::Type old_type = panel.contents[id];

  const DialogLook &look = UIGlobals::GetDialogLook();

  PixelRect form_rc = InfoBoxManager::layout.remaining;

  TWidgetDialog<TabWidget>
    dialog(UIGlobals::GetMainWindow(), look, form_rc,
           gettext(InfoBoxFactory::GetName(old_type)),
           nullptr);
  dialog.SetWidget(TabWidget::Orientation::HORIZONTAL);
  dialog.PrepareWidget();
  auto &tab_widget = dialog.GetWidget();

  bool found_setup = false;

  if (panels != nullptr) {
    for (; panels->load != nullptr; ++panels) {
      assert(panels->name != nullptr);

      auto widget = panels->load(id);

      if (widget == NULL)
        continue;

      if (!found_setup && StringIsEqual(panels->name, "Setup")) {
        /* add a "Switch InfoBox" button to the "Setup" tab -
           kludge! */
        found_setup = true;

        PixelRect button_rc;
        button_rc.left = 0;
        button_rc.top = 0;
        button_rc.right = Layout::Scale(60);
        button_rc.bottom = std::max(2u * Layout::GetMinimumControlHeight(),
                                    Layout::GetMaximumControlHeight());

        auto button = std::make_unique<ButtonWidget>(look.button,
                                                     _("Switch InfoBox"),
                                                     dialog.MakeModalResultCallback(SWITCH_INFO_BOX));

        widget = std::make_unique<TwoWidgets>(std::move(widget),
                                              std::move(button),
                                              false);
      }

      tab_widget.AddTab(std::move(widget), gettext(panels->name));
    }
  }

  if (!found_setup) {
    /* the InfoBox did not provide a "Setup" tab - create a default
       one that allows switching the contents */
    tab_widget.AddTab(std::make_unique<ActionWidget>(dialog.MakeModalResultCallback(SWITCH_INFO_BOX)),
                      _("Switch InfoBox"));
  }

  tab_widget.AddTab(std::make_unique<ActionWidget>(dialog.MakeModalResultCallback(mrOK)),
                    _("Close"));

  const PixelRect client_rc = dialog.GetClientAreaWindow().GetClientRect();
  const PixelSize max_size = tab_widget.GetMaximumSize();
  if (max_size.height < client_rc.GetHeight()) {
    form_rc.top += client_rc.GetHeight() - max_size.height;
    dialog.Move(form_rc);
  }

  dialog.SetModeless();
  int result = dialog.ShowModal();

  if (result == SWITCH_INFO_BOX)
    InfoBoxManager::ShowInfoBoxPicker(id);
}
