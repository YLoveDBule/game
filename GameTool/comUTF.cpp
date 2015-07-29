#include "comUTF.h"

long COM_utf8_strlen (const char * p, int max)
{
   long len = 0;
   const char *start = p;

   if (!(p != 0 || max == 0))
   {
       return 0;
   }

   if (max < 0)
   {
       while (*p)
       {
           p = COM_utf8_next_char (p);
           ++len;
       }
   }
   else
   {
       if (max == 0 || !*p)
           return 0;

       p = COM_utf8_next_char (p);

       while (p - start < max && *p)
       {
            ++len;
            p = COM_utf8_next_char (p);
       }

       /* only do the last len increment if we got a complete
       * char (don't count partial chars)
       */
       if (p - start == max)
           ++len;
   }

   return len;
}