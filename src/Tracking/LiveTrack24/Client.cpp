// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Client.hpp"
#include "Operation/Operation.hpp"
#include "util/StringCompare.hxx"
#include "lib/curl/CoRequest.hxx"
#include "lib/curl/Setup.hxx"
#include "lib/fmt/RuntimeError.hxx"
#include "Geo/GeoPoint.hpp"
#include "co/Task.hxx"
#include "util/StringCompare.hxx"
#include "Version.hpp"

#include <cassert>
#include <cstdlib>
#include <stdexcept>

using std::string_view_literals::operator""sv;

namespace LiveTrack24 {

Co::Task<UserID>
Client::GetUserID(const char *username, const char *password)
{
  // http://www.livetrack24.com/client.php?op=login&user=<username>&pass=<pass>

  assert(username != NULL);
  assert(!StringIsEmpty(username));
  assert(password != NULL);
  assert(!StringIsEmpty(password));

  // Request the file
  CurlEasy easy;

  {
    StaticString<1024> url;
    url.Format("http://%s/client.php?op=login&user=%s&pass=%s",
               GetServer(),
               easy.Escape(username).c_str(),
               easy.Escape(password).c_str());
    easy.SetURL(url);
  }

  Curl::Setup(easy);
  easy.SetFailOnError();

  const auto response = co_await Curl::CoRequest(curl, std::move(easy));
  const char *body = response.body.c_str();

  char *p_end;
  UserID user_id = strtoul(body, &p_end, 10);
  if (p_end == body || user_id == 0)
    throw std::runtime_error("Login failed");

  co_return user_id;
}

Co::Task<void>
Client::StartTracking(SessionID session, const char *username,
                      const char *password, [[maybe_unused]] unsigned tracking_interval,
                      VehicleType vtype, const char *vname)
{
  // http://www.livetrack24.com/track.php?leolive=2&sid=42664778&pid=1&
  //   client=YourProgramName&v=1&user=yourusername&pass=yourpass&
  //   phone=Nokia 2600c&gps=BT GPS&trk1=4&vtype=16388&
  //   vname=vehicle name and model

  CurlEasy easy;

  {
    const char *version = OpenSoar_VersionLong;
    StaticString<2048> url;
    url.Format("http://%s/track.php?leolive=2&sid=%u&pid=%u&"
               "client=%s&v=%s&user=%s&pass=%s&vtype=%u&vname=%s",
               GetServer(), session, 1,
//               "OpenSoar", easy.Escape(version).c_str(),
               "XCSoar", easy.Escape(version).c_str(),
               easy.Escape(username).c_str(),
               easy.Escape(password).c_str(),
               vtype,
               easy.Escape(vname).c_str());

    easy.SetURL(url);
  }

  co_return co_await SendRequest(std::move(easy));
}

Co::Task<void>
Client::SendPosition(SessionID session, unsigned packet_id,
                     GeoPoint position, unsigned altitude,
                     unsigned ground_speed, Angle track,
                     std::chrono::system_clock::time_point timestamp_utc)
{
  // http://www.livetrack24.com/track.php?leolive=4&sid=42664778&pid=321&
  //   lat=22.3&lon=40.2&alt=23&sog=40&cog=160&tm=1241422845

  StaticString<2048> url;
  url.Format("http://%s/track.php?leolive=4&sid=%u&pid=%u&"
             "lat=%f&lon=%f&alt=%d&sog=%d&cog=%d&tm=%lld",
             GetServer(), session, packet_id,
             (double)position.latitude.Degrees(),
             (double)position.longitude.Degrees(),
             altitude, ground_speed,
             (unsigned)track.AsBearing().Degrees(),
             (long long)std::chrono::system_clock::to_time_t(timestamp_utc));

  co_return co_await SendRequest(url);
}

Co::Task<void>
Client::EndTracking(SessionID session, unsigned packet_id)
{
  // http://www.livetrack24.com/track.php?leolive=3&sid=42664778&pid=453&prid=0

  StaticString<1024> url;
  url.Format("http://%s/track.php?leolive=3&sid=%u&pid=%u&prid=0",
             GetServer(), session, packet_id);

  co_return co_await SendRequest(url);
}

void
Client::SetServer(const char * _server) noexcept
{
  server.SetASCII(_server);
}

Co::Task<void>
Client::SendRequest(CurlEasy easy)
{
  Curl::Setup(easy);
  easy.SetFailOnError();

  const auto _response = co_await Curl::CoRequest(curl, std::move(easy));
  std::string_view response{_response.body};
  if (response.starts_with("OK"sv))
    co_return;

  if (SkipPrefix(response, "NOK : "sv) && !response.empty())
    throw FmtRuntimeError("Error from server: {}", response);

  throw std::runtime_error("Error from server");
}

Co::Task<void>
Client::SendRequest(const char *url)
{
  return SendRequest(CurlEasy{url});
}

} // namespace LiveTrack24
