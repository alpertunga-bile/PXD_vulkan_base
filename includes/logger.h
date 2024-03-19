#pragma once

void
create_log_file();

void
log_info(const char* msg,
         const char* time,
         const char* filename,
         int         line,
         const char* function_name);

void
log_warning(const char* msg,
            const char* time,
            const char* filename,
            int         line,
            const char* function_name);

void
log_error(const char* msg,
          const char* time,
          const char* filename,
          int         line,
          const char* function_name);

#ifndef __FUNCTION_NAME__
#ifdef _WIN32
#define __FUNCTION_NAME__ __FUNCTION__
#else
#define __FUNCTION_NAME__ __func__
#endif
#endif

#if defined(_DEBUG)
#define LOG_INFO(msg)                                                          \
  log_info(msg, __TIME__, __FILE__, __LINE__, __FUNCTION_NAME__)
#define LOG_WARNING(msg)                                                       \
  log_warning(msg, __TIME__, __FILE__, __LINE__, __FUNCTION_NAME__)
#define LOG_ERROR(msg)                                                         \
  log_error(msg, __TIME__, __FILE__, __LINE__, __FUNCTION_NAME__)
#else
#define LOG_INFO(msg)
#define LOG_WARNING(msg)
#define LOG_ERROR(msg)
#endif