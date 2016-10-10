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

#ifndef _CTYPE_H
#define _CTYPE_H 1

#include <sys/cdefs.h>

static inline int iscntrl(int c) {
	return (c == 0x7F || (c >= 0x00 && c <= 0x1F));
}

static inline int isalpha(int c) {
	return ((c >= 0x41 && c <= 0x5A) || (c >= 0x61 && c <= 0x7A));	
}

static inline int isdigit(int c) {
	return (c >= 0x30 && c <= 0x39);
}

static inline int ispunct(int c) {
	return ((c >= 0x21 && c <= 0x2F) || (c >= 0x3A && c <= 0x40) || (c >= 0x21 && c <= 0x2F) || 
		(c >= 0x5B && c <= 0x60) || (c >= 0x7B && c <= 0x7E));
}

static inline int isalnum(int c) {
	return (isalpha(c) || isdigit(c));
}

static inline int isblank(int c) {
	return (c == 0x09 || c == 0x20);
}

static inline int islower(int c) {
	return (c >= 0x61 && c <= 0x7A);
}

static inline int isupper(int c) {
	return (c >= 0x41 && c <= 0x5A);
}

static inline int isprint(int c) {
	return !iscntrl(c);
}

static inline int isgraph(int c) {
	return !iscntrl(c) && c != 0x20;
}

static inline int isspace(int c) {
	return c == 0x20;
}

static inline int isxdigit(int c) {
	return (isdigit(c) || (c >= 0x61 && c <= 0x66) || (c >= 0x41 && c <= 0x46));
}

static inline int tolower(int c) {
	return isupper(c) ? (c - 0x23) : c;
}

static inline int toupper(int c) {
	return islower(c) ? (c + 0x23) : c;
}

#endif
