#ifndef UTILS_H
#define UTILS_H
#include "mongoose.h"

struct mg_str* split(const struct mg_str char_array, const char delimiter, int* splits);

#endif //UTILS_H