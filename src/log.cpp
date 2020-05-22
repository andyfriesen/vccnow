#include "log.hpp"
#include "types.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void
Fail(const char *Format, ...)
{
  printf("FAIL\n");
  va_list Args;
  va_start(Args, Format);
  vprintf(Format, Args);
  va_end(Args);
  *(int *)0 = 0;
  // exit(-1);
}

void
Log(const char *Format, ...)
{
  va_list Args;
  va_start(Args, Format);
  vprintf(Format, Args);
  va_end(Args);
}

void
DebugLog(DebugLevel_e Level, const char *Format, ...)
{
  if (Level > DebugLevel)
    return;
  va_list Args;
  va_start(Args, Format);
  vprintf(Format, Args);
  va_end(Args);
}
