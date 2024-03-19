#include "includes/logger.h"

int
main()
{
  create_log_file();

  LOG_INFO("Startup");

  return 0;
}