#include "debug.h"

#define VOLK_IMPLEMANTATION
#include "volk.h"

#include "vulkan/vk_enum_string_helper.h"

#include "logger.h"

void
vk_check(VkResult    result,
         const char* time,
         const char* filename,
         int         line,
         const char* function_name)
{
  if (result == VK_SUCCESS) {
    return;
  }

  std::string res = string_VkResult(result);

  log_error(res.c_str(), time, filename, line, function_name);
}

bool
vk_ret(VkResult    result,
       const char* time,
       const char* filename,
       int         line,
       const char* function_name)
{
  if (result == VK_SUCCESS) {
    return true;
  }

  std::string res = string_VkResult(result);

  log_warning(res.c_str(), time, filename, line, function_name);

  return false;
}
