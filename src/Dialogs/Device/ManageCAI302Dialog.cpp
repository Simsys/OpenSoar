// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "ManageCAI302Dialog.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Dialogs/Message.hpp"
#include "Dialogs/FilePicker.hpp"
#include "Dialogs/JobDialog.hpp"
#include "Dialogs/Error.hpp"
#include "UIGlobals.hpp"
#include "CAI302/UnitsEditor.hpp"
#include "CAI302/WaypointUploader.hpp"
#include "Widget/RowFormWidget.hpp"
#include "Language/Language.hpp"
#include "Operation/Cancelled.hpp"
#include "Operation/MessageOperationEnvironment.hpp"
#include "Device/Driver/CAI302/Internal.hpp"
#include "Device/Driver/CAI302/Protocol.hpp"
#include "Waypoint/Patterns.hpp"

using namespace UI;

class ManageCAI302Widget final
  : public RowFormWidget {
  CAI302Device &device;

public:
  ManageCAI302Widget(const DialogLook &look, CAI302Device &_device)
    :RowFormWidget(look), device(_device) {}

  /* virtual methods from Widget */
  void Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept override;
};

static void
EditUnits(const DialogLook &look, CAI302Device &device)
{
  CAI302::Pilot data;

  MessageOperationEnvironment env;
  if (!device.ReadActivePilot(data, env))
    return;

  CAI302UnitsEditor widget(look, data);
  if (!DefaultWidgetDialog(UIGlobals::GetMainWindow(),
                           UIGlobals::GetDialogLook(),
                           _("Units"), widget))
    return;

  device.WriteActivePilot(widget.GetData(), env);
}

static void
UploadWaypoints(const DialogLook &look, CAI302Device &device)
{
  const auto path = FilePicker(_("Waypoints"), WAYPOINT_FILE_PATTERNS);
  if (path == nullptr)
    return;

  CAI302WaypointUploader job(path, device);
  JobDialog(UIGlobals::GetMainWindow(), look, _("Waypoints"), job, true);
}

void
ManageCAI302Widget::Prepare([[maybe_unused]] ContainerWindow &parent,
                            [[maybe_unused]] const PixelRect &rc) noexcept
{
  AddButton(_("Units"), [this](){
    try {
      EditUnits(GetLook(), device);
    } catch (OperationCancelled) {
    } catch (...) {
      ShowError(std::current_exception(), _("Units"));
    }
  });

  AddButton(_("Waypoints"), [this](){
    try {
      UploadWaypoints(GetLook(), device);
    } catch (OperationCancelled) {
    } catch (...) {
      ShowError(std::current_exception(), _("Waypoints"));
    }
  });

  AddButton(_("Start Logger"), [this](){
    MessageOperationEnvironment env;
    try {
      device.StartLogging(env);
    } catch (OperationCancelled) {
    } catch (...) {
      env.SetError(std::current_exception());
    }
  });

  AddButton(_("Stop Logger"), [this](){
    MessageOperationEnvironment env;
    try {
      device.StopLogging(env);
    } catch (OperationCancelled) {
    } catch (...) {
      env.SetError(std::current_exception());
    }
  });

  AddButton(_("Delete all flights"), [this](){
    if (ShowMessageBox(_("Do you really want to delete all flights from the device?"),
                       "CAI 302", MB_YESNO) != IDYES)
      return;

    MessageOperationEnvironment env;
    try {
      device.ClearLog(env);
    } catch (OperationCancelled) {
    } catch (...) {
      env.SetError(std::current_exception());
    }
  });

  AddButton(_("Reboot"), [this](){
    MessageOperationEnvironment env;
    try {
      device.Reboot(env);
    } catch (OperationCancelled) {
    } catch (...) {
      env.SetError(std::current_exception());
    }
  });
}

void
ManageCAI302Dialog([[maybe_unused]] SingleWindow &parent, const DialogLook &look,
                   Device &device)
{
  WidgetDialog dialog(WidgetDialog::Auto{}, UIGlobals::GetMainWindow(),
                      UIGlobals::GetDialogLook(),
                      "CAI 302",
                      new ManageCAI302Widget(look, (CAI302Device &)device));
  dialog.AddButton(_("Close"), mrCancel);
  dialog.ShowModal();
}
