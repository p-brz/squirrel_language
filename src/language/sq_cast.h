#ifndef SQ_CAST_H
#define SQ_CAST_H

#include "sq_types.h"

string cast_integer_to_string(long value);
string cast_real_to_string(double value);

byte    string_to_byte  (const string value);
short   string_to_short (const string value);
int     string_to_int   (const string value);
long    string_to_long  (const string value);
float   string_to_float (const string value);
double  string_to_double(const string value);

#endif