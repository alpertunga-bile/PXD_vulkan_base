#include "logger.hpp"

#include "debug.h"

#include "includes/shaders/empty_spv.h"

int
main()
{
  LOG_INFO("Startup");

  VK_CHECK(VK_SUCCESS);

  return 0;
}