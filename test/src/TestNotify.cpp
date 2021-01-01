/* Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2021 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "thread/Thread.hpp"
#include "event/Notify.hpp"
#include "event/Globals.hpp"
#include "Screen/Init.hpp"
#include "TestUtil.hpp"

#ifdef ANDROID
#include "event/android/Loop.hpp"
#include "event/shared/Event.hpp"
#elif defined(USE_POLL_EVENT)
#include "event/shared/Event.hpp"
#include "event/poll/Loop.hpp"
#include "Screen/TopWindow.hpp"
#elif defined(ENABLE_SDL)
#include "event/sdl/Event.hpp"
#include "event/sdl/Loop.hpp"
#else
#include "event/windows/Event.hpp"
#include "event/windows/Loop.hpp"
#endif

#ifdef USE_FB
#include "Hardware/RotateDisplay.hpp"
bool
Display::Rotate(DisplayOrientation orientation)
{
  return false;
}
#endif

#ifndef KOBO

#if defined(USE_EGL) || defined(USE_GLX)
/* avoid TopWindow.cpp from being linked, as it brings some heavy
   dependencies */
void TopWindow::Refresh() noexcept {}
#endif

#ifdef USE_POLL_EVENT
bool TopWindow::OnEvent(const Event &event) { return false; }
#endif

#endif

static bool quit;

class TestThread final : public Thread {
  Notify &notify;

public:
  TestThread(Notify &_notify):notify(_notify) {}

protected:
  void Run() noexcept override {
    notify.SendNotification();
  }
};

int main(int argc, char **argv)
{
  plan_tests(1);

  ScreenGlobalInit screen;

  EventLoop loop(*event_queue);
  Event event;

  Notify notify{[]{ quit = true; }};
  TestThread thread(notify);
  thread.Start();

  while (!quit && loop.Get(event))
    loop.Dispatch(event);

  ok1(quit);

  thread.Join();

  return exit_status();
}
