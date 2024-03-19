#pragma once

#include "utility.h"

enum VkResult;

void
vk_check(VkResult    result,
         const char* time,
         const char* filename,
         int         line,
         const char* function_name);

#define VK_CHECK(result)                                                       \
  vk_check(result, __TIME__, __FILE__, __LINE__, __FUNCTION_NAME__)

bool
vk_ret(VkResult    result,
       const char* time,
       const char* filename,
       int         line,
       const char* function_name);

#define VK_RET(result)                                                         \
  vk_ret(result, __TIME__, __FILE__, __LINE__, __FUNCTION_NAME__)