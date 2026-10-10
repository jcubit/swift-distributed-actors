//===----------------------------------------------------------------------===//
//
// This source file is part of the Swift Distributed Actors open source project
//
// Copyright (c) 2018-2019 Apple Inc. and the Swift Distributed Actors project authors
// Licensed under Apache License v2.0
//
// See LICENSE.txt for license information
// See CONTRIBUTORS.txt for the list of Swift Distributed Actors project authors
//
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

#include <stdlib.h>
#include <stdio.h>
#if defined(__has_include)
#if __has_include(<execinfo.h>)
#define SACT_HAVE_EXECINFO 1
 #include <execinfo.h>
#endif
#endif

#include "include/backtrace_support.h"

void sact_dump_backtrace() {
    char** strs = NULL;
    int frames = sact_get_backtrace(&strs);
    if (strs == NULL) {
        fprintf(stderr, "(backtrace unavailable on this platform/libc)\n");
        return;
    }
    for (int i = 0; i < frames; ++i) {
        fprintf(stderr, "%s\n", strs[i]);
    }
    free(strs);
}

int sact_get_backtrace(char*** strs) {
#if defined(SACT_HAVE_EXECINFO)
     void* callstack[128];
     int frames = backtrace(callstack, 128);
     *strs = backtrace_symbols(callstack, frames);
     return frames;
#else
    // musl has no backtrace(3). Report "no frames" rather than failing to build.
    *strs = NULL;
    return 0;
#endif
}
