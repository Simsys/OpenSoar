// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "CloudEnableDialog.hpp"
#include "Tracking/SkyLines/Features.hpp"
#include "Tracking/SkyLines/Key.hpp"
#include "Dialogs/Message.hpp"
#include "net/State.hpp"
#include "Asset.hpp"
#include "Interface.hpp"
#include "Simulator.hpp"
#include "Language/Language.hpp"
#include "Profile/Profile.hpp"
#include "UIGlobals.hpp"
#include "ui/window/SingleWindow.hpp"

void
CloudEnableDialog() noexcept
{
#ifdef HAVE_SKYLINES_TRACKING
  if (!IsAndroid() && !IsDebug())
    /* currently only enabled on Android */
    return;

  auto &settings =
    CommonInterface::SetComputerSettings().tracking.skylines.cloud;
  if (settings.enabled != TriState::UNKNOWN)
    /* explicitly enabled or disabled - don't ask further questions */
    return;

  if (UIGlobals::GetMainWindow().HasDialog())
    /* don't ask stupid questions while the user operates in another
       modal dialog */
    return;

  if (is_simulator())
    /* only check in "Fly" mode */
    return;

  if (CommonInterface::MovementDetected())
    /* don't ask stupid questions if we're flying or about to */
    return;

#ifdef HAVE_NET_STATE
  if (GetNetState() != NetState::CONNECTED)
    /* this feature requires an internet connection; don't bother
       asking if we're not connected */
    return;
#endif

  const char *msg = _("The XCSoar project is currently developing a revolutionary service which allows sharing thermal/wave locations and more with other pilots.\n"
                       "Do you wish to participate in the field test? This means that your position, thermal/wave locations and other weather data will be transmitted to our test server. You can disable it at any time in the \"Tracking\" settings.\n"
                       "Please help us improve XCSoar!");

  int result = ShowMessageBox(msg, "XCSoar Cloud",
                              MB_YESNOCANCEL|MB_ICONQUESTION);

  switch (result) {
  case IDYES:
    settings.enabled = TriState::TRUE;
    Profile::Set(ProfileKeys::CloudEnabled, true);

    if (settings.key == 0) {
      settings.key = SkyLinesTracking::GenerateKey();

      char s[64];
      snprintf(s, sizeof(s), "%llx",
               (unsigned long long)settings.key);
      Profile::Set(ProfileKeys::CloudKey, s);
    }

    Profile::Save();
    break;

  case IDNO:
    settings.enabled = TriState::FALSE;
    Profile::Set(ProfileKeys::CloudEnabled, false);
    Profile::Save();
    break;

  default:
    /* prevent further dialogs during this XCSoar run, but ask again
       later (don't save to profile) */
    settings.enabled = TriState::FALSE;
  }
#endif
}
