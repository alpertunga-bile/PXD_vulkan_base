#pragma once

#include "SIMDString.h"
using String = SIMDString<64>;

inline bool
IsValid(void* object) noexcept
{
  return object != nullptr ? true : false;
}