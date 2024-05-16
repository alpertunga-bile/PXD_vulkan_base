#pragma once

#ifndef __FUNCTION_NAME__
#ifdef _WIN32
#define __FUNCTION_NAME__ __FUNCTION__
#else
#define __FUNCTION_NAME__ __func__
#endif
#endif

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