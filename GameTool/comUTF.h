#ifndef _COM_UTF_H_
#define _COM_UTF_H_
#pragma once
static int COM_wcslen(const unsigned short* str)
{
    int i=0;
    while(*str++) i++;
    return i;
}

/* Code from GLIB gutf8.c starts here. */

#define COM_UTF8_COMPUTE(Char, Mask, Len)        \
  if (Char < 128)                \
    {                        \
      Len = 1;                    \
      Mask = 0x7f;                \
    }                        \
  else if ((Char & 0xe0) == 0xc0)        \
    {                        \
      Len = 2;                    \
      Mask = 0x1f;                \
    }                        \
  else if ((Char & 0xf0) == 0xe0)        \
    {                        \
      Len = 3;                    \
      Mask = 0x0f;                \
    }                        \
  else if ((Char & 0xf8) == 0xf0)        \
    {                        \
      Len = 4;                    \
      Mask = 0x07;                \
    }                        \
  else if ((Char & 0xfc) == 0xf8)        \
    {                        \
      Len = 5;                    \
      Mask = 0x03;                \
    }                        \
  else if ((Char & 0xfe) == 0xfc)        \
    {                        \
      Len = 6;                    \
      Mask = 0x01;                \
    }                        \
  else                        \
    Len = -1;

#define COM_UTF8_LENGTH(Char)            \
  ((Char) < 0x80 ? 1 :                \
   ((Char) < 0x800 ? 2 :            \
    ((Char) < 0x10000 ? 3 :            \
     ((Char) < 0x200000 ? 4 :            \
      ((Char) < 0x4000000 ? 5 : 6)))))


#define COM_UTF8_GET(Result, Chars, Count, Mask, Len)    \
  (Result) = (Chars)[0] & (Mask);            \
  for ((Count) = 1; (Count) < (Len); ++(Count))        \
    {                            \
      if (((Chars)[(Count)] & 0xc0) != 0x80)        \
    {                        \
      (Result) = -1;                \
      break;                    \
    }                        \
      (Result) <<= 6;                    \
      (Result) |= ((Chars)[(Count)] & 0x3f);        \
    }

#define COM_UNICODE_VALID(Char)            \
  ((Char) < 0x110000 &&                \
   (((Char) & 0xFFFFF800) != 0xD800) &&        \
   ((Char) < 0xFDD0 || (Char) > 0xFDEF) &&    \
   ((Char) & 0xFFFE) != 0xFFFE)


static const char COM_utf8_skip_data[256] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5,
  5, 5, 5, 6, 6, 1, 1
};

static const char *const g_utf8_skip = COM_utf8_skip_data;

#define COM_utf8_next_char(p) (char *)((p) + g_utf8_skip[*(unsigned char *)(p)])

//
///*
//* @str:    the string to search through.
//* @c:        the character to find.
//* 
//* Returns the index of the first occurrence of the character, if found.  Otherwise -1 is returned.
//* 
//* Return value: the index of the first occurrence of the character if found or -1 otherwise.
//* */
//static unsigned int COM_utf8_find_char(std::vector<unsigned short> str, unsigned short c)
//{
//   unsigned int len = str.size();
//
//   for (unsigned int i = 0; i < len; ++i)
//       if (str[i] == c) return i;
//
//   return -1;
//}
//
///*
//* @str:    the string to search through.
//* @c:        the character to not look for.
//* 
//* Return value: the index of the last character that is not c.
//* */
//static unsigned int COM_utf8_find_last_not_char(std::vector<unsigned short> str, unsigned short c)
//{
//   int len = str.size();
//
//   int i = len - 1;
//   for (; i >= 0; --i)
//       if (str[i] != c) return i;
//
//   return i;
//}
//
///*
//* @str:    the string to trim
//* @index:    the index to start trimming from.
//* 
//* Trims str st str=[0, index) after the operation.
//* 
//* Return value: the trimmed string.
//* */
//static void COM_utf8_trim_from(std::vector<unsigned short>* str, int index)
//{
//   int size = str->size();
//   if (index >= size || index < 0)
//       return;
//
//   str->erase(str->begin() + index, str->begin() + size);
//}
//
///*
//* @ch is the unicode character whitespace?
//* 
//* Reference: http://en.wikipedia.org/wiki/Whitespace_character#Unicode
//* 
//* Return value: weather the character is a whitespace character.
//* */
//static bool COM_isspace_unicode(unsigned short ch)
//{
//   return  (ch >= 0x0009 && ch <= 0x000D) || ch == 0x0020 || ch == 0x0085 || ch == 0x00A0 || ch == 0x1680
//       || (ch >= 0x2000 && ch <= 0x200A) || ch == 0x2028 || ch == 0x2029 || ch == 0x202F
//       ||  ch == 0x205F || ch == 0x3000;
//}
//
//static void COM_utf8_trim_ws(std::vector<unsigned short>* str)
//{
//   int len = str->size();
//
//   if ( len <= 0 )
//       return;
//
//   int last_index = len - 1;
//
//   // Only start trimming if the last character is whitespace..
//   if (COM_isspace_unicode((*str)[last_index]))
//   {
//       for (int i = last_index - 1; i >= 0; --i)
//       {
//           if (COM_isspace_unicode((*str)[i]))
//               last_index = i;
//           else
//               break;
//       }
//
//       COM_utf8_trim_from(str, last_index);
//   }
//}
//
/*
* g_utf8_strlen:
* @p: pointer to the start of a UTF-8 encoded string.
* @max: the maximum number of bytes to examine. If @max
*       is less than 0, then the string is assumed to be
*       null-terminated. If @max is 0, @p will not be examined and
*       may be %NULL.
*
* Returns the length of the string in characters.
*
* Return value: the length of the string in characters
**/
long COM_utf8_strlen (const char * p, int max);

///*
//* g_utf8_get_char:
//* @p: a pointer to Unicode character encoded as UTF-8
//*
//* Converts a sequence of bytes encoded as UTF-8 to a Unicode character.
//* If @p does not point to a valid UTF-8 encoded character, results are
//* undefined. If you are not sure that the bytes are complete
//* valid Unicode characters, you should use g_utf8_get_char_validated()
//* instead.
//*
//* Return value: the resulting character
//**/
//static unsigned int
//COM_utf8_get_char (const char * p)
//{
// int i, mask = 0, len;
// unsigned int result;
// unsigned char c = (unsigned char) *p;
//
// COM_UTF8_COMPUTE (c, mask, len);
// if (len == -1)
//   return (unsigned int) - 1;
// COM_UTF8_GET (result, p, i, mask, len);
//
// return result;
//}
//
///*
//* cc_utf16_from_utf8:
//* @str_old: pointer to the start of a C string.
//* 
//* Creates a utf8 string from a cstring.
//*
//* Return value: the newly created utf8 string.
//* */
//static unsigned short* COM_utf16_from_utf8(const char* str_old)
//{
//   int len = COM_utf8_strlen(str_old, -1);
//
//   unsigned short* str_new = new unsigned short[len + 1];
//   str_new[len] = 0;
//
//   for (int i = 0; i < len; ++i)
//   {
//       str_new[i] = COM_utf8_get_char(str_old);
//       str_old = COM_utf8_next_char(str_old);
//   }
//
//   return str_new;
//}
//
//static std::vector<unsigned short> COM_utf16_vec_from_utf16_str(const unsigned short* str)
//{
//   int len = COM_wcslen(str);
//   std::vector<unsigned short> str_new;
//
//   for (int i = 0; i < len; ++i)
//   {
//       str_new.push_back(str[i]);
//   }
//   return str_new;
//}

#endif //C_C_UTF_H_