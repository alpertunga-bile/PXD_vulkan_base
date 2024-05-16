#include "logger.hpp"

#include "debug.h"

int
main()
{
  LOG_INFO("Startup");

  VK_CHECK(VK_NOT_READY);

  return 0;
}