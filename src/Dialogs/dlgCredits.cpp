// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Dialogs/Dialogs.h"
#include "Dialogs/WidgetDialog.hpp"
#include "Widget/CreateWindowWidget.hpp"
#include "Widget/ArrowPagerWidget.hpp"
#include "Widget/LargeTextWidget.hpp"
#include "Look/FontDescription.hpp"
#include "Look/DialogLook.hpp"
#include "Look/Colors.hpp"
#include "ui/canvas/Canvas.hpp"
#include "Screen/Layout.hpp"
#include "ui/canvas/Bitmap.hpp"
#include "ui/canvas/Font.hpp"
#include "Version.hpp"
#include "Inflate.hpp"
#include "util/ConvertString.hpp"
#include "util/AllocatedString.hxx"
#include "Resources.hpp"
#include "UIGlobals.hpp"
#include "Language/Language.hpp"

class LogoPageWindow final : public PaintWindow {
protected:
  /** from class PaintWindow */
  void OnPaint(Canvas &canvas) noexcept override;
};

void
LogoPageWindow::OnPaint(Canvas &canvas) noexcept
{
  const PixelRect rc = GetClientRect();

  const unsigned width = rc.GetWidth();
  int x = rc.left + Layout::FastScale(10);
  int y = rc.top + Layout::FastScale(10);

  canvas.ClearWhite();

  Bitmap logo(width > 360 ? IDB_LOGO_HD : IDB_LOGO);
  Bitmap title(width > 360 ? IDB_TITLE_HD : IDB_TITLE);

  // Determine title and logo image size
  PixelSize logo_size = logo.GetSize();
  PixelSize title_size = title.GetSize();

  unsigned int middle = width / 2;

  // Draw XCSoar Logo
  x = middle - (logo_size.width / 2);
  canvas.Copy({x, y}, logo_size, logo, {0, 0});

  // Draw 'XCSoar N.N' title
  x = middle - (title_size.width / 2);
  y += logo_size.height + Layout::FastScale(2);
  canvas.Copy({x, y}, title_size, title, {0, 0});
  y += title_size.height + Layout::FastScale(2);


  Font font;
  if (width > 360)
    font.Load(FontDescription(Layout::VptScale(16)));
  else
    font.Load(FontDescription(Layout::VptScale(8)));
  canvas.Select(font);
  canvas.SetBackgroundTransparent();

  canvas.SetTextColor(COLOR_BLACK);
  x = middle;
  const char *version = "Version: ";
  PixelSize ts = canvas.CalcTextSize(version);
  PixelSize ts2 = canvas.CalcTextSize(OpenSoar_VersionString);
  x = middle - ((ts.width + ts2.width) / 2 );
  canvas.DrawText({x, y}, version);
  x += ts.width;
  canvas.DrawText({x, y}, OpenSoar_VersionString);


#ifdef GIT_COMMIT_ID
  y += ts.height + Layout::FastScale(2);
  x = middle;
  const char *git = "git: ";
  ts = canvas.CalcTextSize(git);
  ts2 = canvas.CalcTextSize(GIT_COMMIT_ID);
  x = middle - ((ts.width + ts2.width) / 2 );
  canvas.DrawText({x, y}, git);
  x += ts.width;
  canvas.DrawText({x, y}, GIT_COMMIT_ID);
  y += ts.height + Layout::FastScale(2);
#endif

  y += Layout::FastScale(8);
  const char *visit = "Vist us at:";
  const char *url = "https://xcsoar.org";
  ts = canvas.CalcTextSize(visit);
  ts2 = canvas.CalcTextSize(url);
  x = middle - (ts.width / 2);
  y += ts.height;
  canvas.SetTextColor(COLOR_BLACK);
  canvas.DrawText({x, y}, visit);
  x = middle - (ts2.width / 2);
  y += ts2.height;
  canvas.SetTextColor(COLOR_XCSOAR);
  canvas.DrawText({x, y}, url);

}

static std::unique_ptr<Window>
CreateLogoPage(ContainerWindow &parent, const PixelRect &rc,
               WindowStyle style)
{
  auto window = std::make_unique<LogoPageWindow>();
  window->Create(parent, rc, style);
  return window;
}

extern "C"
{
  extern const uint8_t COPYING_gz[];
  extern const size_t COPYING_gz_size;

  extern const uint8_t OpenSoar_News_md_gz[];
  extern const size_t OpenSoar_News_md_gz_size;

  extern const uint8_t AUTHORS_gz[];
  extern const size_t AUTHORS_gz_size;
}

void
dlgCreditsShowModal([[maybe_unused]] UI::SingleWindow &parent)
{
  const DialogLook &look = UIGlobals::GetDialogLook();

  const auto authors = InflateToString(AUTHORS_gz, AUTHORS_gz_size);
  const UTF8ToWideConverter authors2(authors.c_str());

  const auto news =
      InflateToString(OpenSoar_News_md_gz, OpenSoar_News_md_gz_size);
  const UTF8ToWideConverter news2(news.c_str());

  const auto license = InflateToString(COPYING_gz, COPYING_gz_size);
  const UTF8ToWideConverter license2(license.c_str());

  WidgetDialog dialog(WidgetDialog::Full{}, UIGlobals::GetMainWindow(),
                      look, _("Credits"));

  auto pager = std::make_unique<ArrowPagerWidget>(look.button,
                                                  dialog.MakeModalResultCallback(mrOK));
  pager->Add(std::make_unique<CreateWindowWidget>(CreateLogoPage));
  pager->Add(std::make_unique<LargeTextWidget>(look, authors2));
  pager->Add(std::make_unique<LargeTextWidget>(look, news2));
  pager->Add(std::make_unique<LargeTextWidget>(look, license2));

  dialog.FinishPreliminary(std::move(pager));
  dialog.ShowModal();
}
