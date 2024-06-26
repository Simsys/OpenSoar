// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "InfoBoxes/Content/Base.hpp"

struct SpeedVector;
class Brush;

void UpdateInfoBoxHumidity(InfoBoxData &data) noexcept;

void
UpdateInfoBoxTemperature(InfoBoxData &data) noexcept;

class InfoBoxContentTemperatureForecast : public InfoBoxContent
{
public:
  void Update(InfoBoxData &data) noexcept override;
  bool HandleKey(const InfoBoxKeyCodes keycode) noexcept override;
};

extern const InfoBoxPanel wind_infobox_panels[];

void
UpdateInfoBoxWindSpeed(InfoBoxData &data) noexcept;

void
UpdateInfoBoxWindBearing(InfoBoxData &data) noexcept;

void
UpdateInfoBoxInstWindSpeed(InfoBoxData &data) noexcept;

void
UpdateInfoBoxInstWindBearing(InfoBoxData &data) noexcept;

void
UpdateInfoBoxHeadWind(InfoBoxData &data) noexcept;

void
UpdateInfoBoxHeadWindSimplified(InfoBoxData &data) noexcept;

void 
PaintWindArrow(Canvas &canvas, const PixelRect &rc,
  const SpeedVector &wind, const Brush &brush) noexcept;

class InfoBoxContentWindArrow: public InfoBoxContent
{
public:
  void Update(InfoBoxData &data) noexcept override;
  void OnCustomPaint(Canvas &canvas, const PixelRect &rc) noexcept override;
  const InfoBoxPanel *GetDialogContent() noexcept override;
};
