/*
 * Copyright 2016 Zealous Development Team.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy
 * of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 */

#include <string.h>
#include <stdbool.h>

void reverse(char* str) {
    if (str) {
      char* end = str + strlen(str) - 1;
#     define XOR_SWAP(a,b) do {\
            a ^= b;\
            b ^= a;\
            a ^= b;\
      } while (0)

      while (str < end) {
          XOR_SWAP(*str, *end);
          str++;
          end--;
      }
#     undef XOR_SWAP
    }
}

char* itoa(int value, char* str, int base) {
    int i = 0;
    bool negative = false;
    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (value < 0 && base == 10) {
        negative = true;
        value = -value;
    }
 
    while (value != 0) {
        int rem = value % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        value = value/base;
    }
 
    if (negative) str[i++] = '-';
 
    str[i] = '\0';
    reverse(str);
    return str;
}
