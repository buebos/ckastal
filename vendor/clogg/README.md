# Clogg: C Logger

Basic helper functions for formatting logging messages using the stdio lib.

```c
#include "package/clogg/include/clogg.h"

int main(int argc, char** argv) {
    cloggln(CLOGG_INFO, "Hello world!");
    return 0;
}
```

## Building

It's just one file by now, so it's really simple lol, you have three options:

```bash
$ gcc -shared -o lib/clogg.a src/clogg.c
```

```bash
$ mkdir lib && make
```

```bash
$ cmake -S . -B lib -G "MinGW Makefiles" && make -C lib
```
