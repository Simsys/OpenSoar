// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Port.hpp"
#include "Listener.hpp"
#include "Device/Error.hpp"
#include "time/TimeoutClock.hpp"
#include "Operation/Operation.hpp"
#include "Operation/Cancelled.hpp"
#include "util/Exception.hxx"
#include "util/SpanCast.hxx"

#include <algorithm>
#include <cassert>

Port::Port(PortListener *_listener, DataHandler &_handler) noexcept
  :listener(_listener), handler(_handler) {}

Port::~Port() noexcept = default;

bool
Port::WaitConnected(OperationEnvironment &env)
{
  while (GetState() == PortState::LIMBO) {
    if (env.IsCancelled())
      throw OperationCancelled{};

    env.Sleep(std::chrono::milliseconds(200));
  }

  return GetState() == PortState::READY;
}

std::size_t
Port::Write(std::string_view s)
{
  return Write(AsBytes(s));
}

void
Port::FullWrite(std::span<const std::byte> src,
                OperationEnvironment &env,
                std::chrono::steady_clock::duration _timeout)
{
  const TimeoutClock timeout(_timeout);

  while (!src.empty()) {
    if (timeout.HasExpired())
      throw DeviceTimeout{"Port write timeout"};

    std::size_t nbytes = Write(src);
    assert(nbytes > 0);

    if (env.IsCancelled())
      throw OperationCancelled{};

    src = src.subspan(nbytes);
  }
}

void
Port::FullWrite(std::string_view s,
                OperationEnvironment &env,
                std::chrono::steady_clock::duration timeout)
{
  FullWrite(AsBytes(s), env, timeout);
}

std::byte
Port::ReadByte()
{
  std::byte b;
  if (Read(&b, sizeof(b)) != sizeof(b))
    throw std::runtime_error{"Port read failed"};

  return b;
}

void
Port::FullFlush(OperationEnvironment &env,
                std::chrono::steady_clock::duration timeout,
                std::chrono::steady_clock::duration _total_timeout)
{
  Flush();

  const TimeoutClock total_timeout(_total_timeout);

  do {
    try {
      WaitRead(env, timeout);
    } catch (const DeviceTimeout &) {
      return;
    }

    if (char buffer[0x100];
        Read(buffer, sizeof(buffer)) <= 0)
      throw std::runtime_error{"Port read failed"};
  } while (!total_timeout.HasExpired());
}

void
Port::FullRead(void *buffer, std::size_t length, OperationEnvironment &env,
               std::chrono::steady_clock::duration first_timeout,
               std::chrono::steady_clock::duration subsequent_timeout,
               std::chrono::steady_clock::duration total_timeout)
{
  const TimeoutClock full_timeout(total_timeout);

  char *p = (char *)buffer, *end = p + length;

  p += WaitAndRead(buffer, length, env, first_timeout);

  while (p < end) {
    const auto ft = full_timeout.GetRemainingSigned();
    if (ft.count() < 0)
      /* timeout */
      throw DeviceTimeout{"Port read timeout"};

    const auto t = std::min(ft, subsequent_timeout);

    p += WaitAndRead(p, end - p, env, t);
  }
}

void
Port::FullRead(void *buffer, std::size_t length, OperationEnvironment &env,
               std::chrono::steady_clock::duration timeout)
{
  FullRead(buffer, length, env, timeout, timeout, timeout);
}

void
Port::WaitRead(OperationEnvironment &env,
               std::chrono::steady_clock::duration timeout)
{
  auto remaining = timeout;

  do {
    /* this loop is ugly, and should be redesigned when we have
       non-blocking I/O in all Port implementations */
    const auto t = std::min<std::chrono::steady_clock::duration>(remaining, std::chrono::milliseconds(500));

    try {
      WaitRead(t);
      return;
    } catch (const DeviceTimeout &){
    }

    if (env.IsCancelled())
      throw OperationCancelled{};

    remaining -= t;
  } while (remaining.count() > 0);

  throw DeviceTimeout{"Port read timeout"};
}

std::size_t
Port::WaitAndRead(void *buffer, std::size_t length,
                  OperationEnvironment &env,
                  std::chrono::steady_clock::duration timeout)
{
  WaitRead(env, timeout);

  int nbytes = Read(buffer, length);
  if (nbytes <= 0)
    throw std::runtime_error{"Port read failed"};

  return (std::size_t)nbytes;
}

std::size_t
Port::WaitAndRead(void *buffer, std::size_t length,
                  OperationEnvironment &env, TimeoutClock timeout)
{
  const auto remaining = timeout.GetRemainingSigned();
  if (remaining.count() < 0)
    throw DeviceTimeout{"Port read timeout"};

  return WaitAndRead(buffer, length, env, remaining);
}

void
Port::ExpectString(std::string_view token, OperationEnvironment &env,
                   std::chrono::steady_clock::duration _timeout)
{
  const char *const token_end = token.data() + token.size();

  const TimeoutClock timeout(_timeout);

  char buffer[256];

  const char *p = token.data();
  while (true) {
    auto nbytes = WaitAndRead(buffer,
                              std::min(sizeof(buffer),
                                       std::size_t(token_end - p)),
                              env, timeout);

    for (const char *q = buffer, *end = buffer + nbytes; q != end; ++q) {
      const char ch = *q;
      if (ch != *p)
        /* retry */
        p = token.data();
      else if (++p == token_end)
        return;
    }
  }
}

void
Port::WaitForByte(const std::byte token, OperationEnvironment &env,
                  std::chrono::steady_clock::duration _timeout)
{
  const TimeoutClock timeout(_timeout);

  while (true) {
    WaitRead(env, timeout.GetRemainingOrZero());

    // Read and compare character with token
    const auto b = ReadByte();
    if (b == token)
      break;

    if (timeout.HasExpired())
      throw DeviceTimeout{"Port read timeout"};
  }
}

void
Port::StateChanged() noexcept
{
  PortListener *l = listener;
  if (l != nullptr)
    l->PortStateChanged();
}

void
Port::Error(const char *msg) noexcept
{
  PortListener *l = listener;
  if (l != nullptr)
    l->PortError(msg);
}

void
Port::Error(std::exception_ptr e) noexcept
{
  PortListener *l = listener;
  if (l != nullptr)
    l->PortError(GetFullMessage(e).c_str());
}
