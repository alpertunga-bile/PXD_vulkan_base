#include "debug.h"

#include <filesystem>
#include <format>

String
get_output_string(const char* filename, int line, const char* function_name)
{
  auto base_filename = std::filesystem::path(filename).filename().string();

  auto formattedString =
    std::format("{} /_\\ {} /_\\ {}", base_filename, line, function_name);

  return String(formattedString);
}

void
log_info(const char* msg,
         const char* time,
         const char* filename,
         int         line,
         const char* function_name)
{
  printf("[%s] /_\\ [    INFO] /_\\ %s /_\\ %s\n",
         time,
         msg,
         get_output_string(filename, line, function_name).c_str());
}

void
log_warning(const char* msg,
            const char* time,
            const char* filename,
            int         line,
            const char* function_name)
{
  printf("[%s] /_\\ [ WARNING] /_\\ %s /_\\ %s\n",
         time,
         msg,
         get_output_string(filename, line, function_name).c_str());
}

void
log_error(const char* msg,
          const char* time,
          const char* filename,
          int         line,
          const char* function_name)
{
  printf("[%s] /_\\ [  FAILED] /_\\ %s /_\\ %s\n",
         time,
         msg,
         get_output_string(filename, line, function_name).c_str());
}
