/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│ vi: set et ft=c ts=2 sts=2 sw=2 fenc=utf-8                               :vi │
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2021 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/intrin/describeflags.h"
#include "libc/intrin/maps.h"
#include "libc/runtime/memtrack.internal.h"
#include "libc/sysv/consts/map.h"
#include "libc/sysv/consts/prot.h"

static char DescribeMapType(int flags) {
  switch (flags & MAP_TYPE) {
    case MAP_FILE:
      if (flags & MAP_NOFORK)
        return 'i';  // executable image
      return '-';
    case MAP_PRIVATE:
      if (flags & MAP_NOFORK)
        return 'w';  // windows memory
      return 'p';
    case MAP_SHARED:
      return 's';
    default:
      return '?';
  }
}

char *DescribeProt(char p[4], int prot) {
  p[0] = (prot & PROT_READ) ? 'r' : '-';
  p[1] = (prot & PROT_WRITE) ? 'w' : '-';
  p[2] = (prot & PROT_EXEC) ? 'x' : '-';
  p[3] = 0;
  return p;
}

const char *_DescribeMapping(char p[8], int prot, int flags) {
  /* asan runtime depends on this function */
  DescribeProt(p, prot);
  p[3] = DescribeMapType(flags);
  p[4] = (flags & MAP_ANONYMOUS) ? 'a' : '-';
  p[5] = 0;
  return p;
}
