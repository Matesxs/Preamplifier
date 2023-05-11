#include "helpers.h"

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

String format_string(const char *format, ...)
{
  char loc_buf[64];
  char *temp = loc_buf;
  va_list arg;
  va_list copy;

  va_start(arg, format);
  
  va_copy(copy, arg);
  int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
  va_end(copy);

  if (len < 0)
  {
    va_end(arg);
    return String();
  }

  if (len >= sizeof(loc_buf))
  {
    temp = (char *)malloc(len + 1);
    if (temp == NULL)
    {
      va_end(arg);
      return String();
    }
    len = vsnprintf(temp, len + 1, format, arg);
  }
  va_end(arg);

  if (temp != loc_buf)
    free(temp);
  return String(temp);
}
