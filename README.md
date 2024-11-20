# `asprintf` - One True `asprintf`, `vasprintf`! 🛠️

Robust, portable implementation of `asprintf()`, `vasprintf()`.
Thoroughly tested.

## Highlights

- 🚀 _**Fast**_ - performance akin other implementations
- 📦 _**Simple**_ - zero dependencies, lightweight
  (`37 lines`, `566 bytes`) and ISO [C99] compatible.
- 🔒 _**Robust**_ - safety-first, with substantial [unit testing](test.c).
- ⚙️ _**Compatible**_ - interop across different systems, drop-in `asprintf`, `vasprintf` replacement for [glibc], [*BSD libc][OpenBSD `asprintf`],
  [musl libc] and [many more](#see-also).

## Motivation

Wanted to have a version of `asprintf` that is simple, robust and just works
everywhere. Needed it to be backward-compatible with existing `libc` libraries,
consistent across diverse platforms and systems. It should also focus on
correctness, minimize unexpected behavior, be thoroughly tested, be easy to read
and stick to both [C99] and [POSIX.1-2024].

This process also gave me the opportunity to review existing implementations and
deepen my understanding of pointers and systems programming.

## Getting Started

> [!IMPORTANT]  
> `asprintf()` and `vasprintf()` are now included in
> [POSIX.1-2024][posix-2024-announced], following [Austin Group Bug
> #1496](https://www.austingroupbugs.net/view.php?id=1496). You can read the
> latest version at [posix-asprintf]. For systems supporting `POSIX.1-2024` or
> later, `Neved4/asprintf` is no longer necessary. For systems limited to
> `POSIX.1-2017` or under, it offers a practical solution.

### Prerequisites

If you are building [`asprintf`][Neved4/asprintf], ensure you have:

- A [C] compiler that supports [C99]

### Installing

If you have [clib] installed, run:
```console
$ clib install Neved4/asprintf
```

If you don't have the above, start by cloning the repository:
```console
$ git clone https://github.com/Neved4/asprintf
```

Once you've cloned the repository, build by executing:
```console
$ make asprintf
```

Alternatively, if you have [`zig`]:
```console
$ zig cc asprintf.c -t asprintf
```

### Usage

```c
// asprintf, vasprintf - print to allocated string

#include "asprintf.h"

int asprintf(char **strp, const char *fmt, ...);

int vasprintf(char **strp, const char *fmt, va_list ap);
```

### Examples

Consider a `getconf()` function to retrieve a `config` specified path, that
supports both `XDG_CONFIG_HOME` and fallbacks:

```c
char *getconf() {
    const char *file = "tz.conf", *home = getenv("HOME"),
        *xdg_config_home = getenv("XDG_CONFIG_HOME");
    char *config = NULL;

    // Path building logic

    return config;
}
```

After which we'll have to lay down our path building logic.

###### Before `asprintf`

```c
if (access("tz.conf", F_OK) != -1) {
    config = strdup("tz.conf");
} else if (xdg_config_home) {
    size_t len = strlen(xdg_config_home) + strlen("twc") + strlen(file) + 3;
    config = (char *)malloc(len);
    if (config != NULL) {
        snprintf(config, len, "%s/%s/%s", xdg_config_home, "twc", file);
    }
} else if (home) {
    size_t len = strlen(home) + strlen(".config/twc") + strlen(file) + 3;
    config = (char *)malloc(len);
    if (config != NULL) {
        snprintf(config, len, "%s/%s/%s", home, ".config/twc", file);
    }
}
```

###### After `asprintf`

```c
if (access("tz.conf", F_OK) != -1) {
    config = strdup("tz.conf");
} else if (xdg_config_home) {
    asprintf(&config, "%s/%s/%s", xdg_config_home, "twc", file);
} else if (home) {
    asprintf(&config, "%s/%s/%s", home, ".config/twc", file);
}
```

### Testing

To run all the tests against `asprintf` execute the following command:
```sh
cc test.c asprintf.c -o test && ./test
```

To use your system's default `asprintf(3)`, run this instead:
```sh
cc test.c -o test && ./test
```

To link it against other `asprintf` implementations, run:
```sh
cc test.c /path/to/asprintf.c -o test && ./test
```

Any of the above will output something like:
```rust
 Ok "Memory allocation"
 Ok "Empty string as input"
 Ok "String formatting variations"
 Ok "Special characters in format string"
Err "Boundary cases for integers"

// More tests

-----------
Passing: 4
 Failed: 1
  Total: 5
```

### Docker

To compile the binary inside a [Docker] image, run:
```sh
docker build .
```

## Compatibility

Runs on _**Linux**_, _**macOS**_ and _**\*BSD**_ systems on both [`x86_64`]
and [`arm64`]. Builds with [`clang`], [`gcc`], [`tcc`], [`zig`] and any
other compiler that supports [C99] or later.

Should be compatible with [glibc], [GLib], FreeBSD libc, musl libc `asprintf`.

## Standards

`asprintf` is compatible with both [POSIX.1-2024],[^1] and [C99].[^2]

## License

`asprintf` is licensed under the terms of the [MIT License].

See the [LICENSE](LICENSE) file for details.

## See Also

#### Reference

- [ISO/IEC 9899][C99]
- [ISO/IEC DIS 9945][POSIX.1-2024]
- [ISO/IEC TS 17961:2013](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1579.pdf)
- [MISRA C:2012](https://electrovolt.ir/wp-content/uploads/2022/09/MISRA-C_2012_-Guidelines-for-the-Use-of-the-C-Language-in-Critical-Systems-Motor-Industry-Research-Association-2013-2013.pdf)
- [SEI CERT C](https://wiki.sei.cmu.edu/confluence/display/c) [.pdf](https://resources.sei.cmu.edu/downloads/secure-coding/assets/sei-cert-c-coding-standard-2016-v01.pdf)

#### System implementations

|               Source | Docs                           |
| -------------------: | ------------------------------ |
|  [Debian `asprintf`] | [manpages.debian.org/asprintf] |
| [FreeBSD `asprintf`] | [man.freebsd.org/asprintf]     |
| [OpenBSD `asprintf`] | [man.openbsd.org/asprintf]     |
|  [NetBSD `asprintf`] | [man.netbsd.org/asprintf.3]    |
|   Solaris `asprintf` | [docs.oracle.com/asprintf-3c]  |
|      z/OS `asprintf` | [ibm.com/docs/zos/asprintf]    |
|  [sortix `asprintf`] |                                |

#### Other `libc` implementations

- [cosmopolitan/asprintf.c](https://github.com/jart/cosmopolitan/blob/master/libc/stdio/asprintf.c)
- [dietlibc](https://www.fefe.de/dietlibc/)
- [glibc/asprintf.c](https://sourceware.org/git/?p=glibc.git;a=blob;f=stdio-common/asprintf.c;h=bb8f4bffc19292c7173d8180b4c2428411d5c269;hb=HEAD)
- [musl/asprintf.c](https://git.musl-libc.org/cgit/musl/tree/src/stdio/asprintf.c)
- [newlib/asprintf.c](https://sourceware.org/git/?p=newlib-cygwin.git;a=blob;f=newlib/libc/stdio/asprintf.c;hb=HEAD)
- [picolibc/asprintf.c](https://github.com/picolibc/picolibc/blob/main/newlib/libc/tinystdio/asprintf.c#L42)
- [uclibc/asprintf.c](https://gogs.waldemar-brodkorb.de/oss/uclibc-ng/src/master/libc/stdio/asprintf.c)

#### Other `asprintf` implementations

- [akharrou/ft_asprintf](https://github.com/akharrou/42-Project-Ft_Printf/blob/master/ft_asprintf.c)
- [andrelmbackman/ft_printf](https://github.com/andrelmbackman/ft_printf)
- [d4wae89d498/ft_asprintf](https://github.com/d4wae89d498/libftprintf/blob/master/ft_asprintf.c)
- [dragonblocks/asprintf](https://github.com/dragonblocks/asprintf/blob/main/asprintf.c)
- [eiszapfen2000/asprintf](https://github.com/eiszapfen2000/asprintf/blob/master/asprintf.c)
- [finwo/c-asprintf](https://github.com/finwo/c-asprintf/blob/main/src/asprintf.c)
- [jkaivo/asprintf](https://github.com/jkaivo/asprintf/blob/asprintf/asprintf.h)
- [jwerle/asprintf.c](https://github.com/jwerle/asprintf.c/blob/master/asprintf.c)
- [libressl/portable](https://github.com/libressl/portable/blob/master/crypto/compat/bsd-asprintf.c#L84)
- [lordmulder/asprintf-msvc](https://github.com/lordmulder/asprintf-msvc/blob/master/asprintf_msvc.c#L35)
- [n-nino/vc6-asprintf](https://github.com/n-nino/vc6-asprintf/blob/master/asprintf.c)
- [naev/naev/asprintf](https://github.com/naev/naev/blob/main/utils/model-view-c/nstr.c)
- [tap3edit/mp/mpasprintf](https://github.com/tap3edit/mp/blob/master/src/mp.c#L705)
- [timbaker/asprintf](https://github.com/timbaker/asprintf/blob/master/asprintf.c)
- [weiss/c99-snprintf](https://github.com/weiss/c99-snprintf/blob/master/snprintf.c#L1528)

#### Further reading

- [Chris Wellons: How to Write Portable C Without Complicating Your Build](https://nullprogram.com/blog/2017/03/30/)
- [Fedora Defensive Coding Guide: The C Programming Language](https://docs.fedoraproject.org/en-US/defensive-coding/programming-languages/C/#sect-Defensive_Coding-C-Libc)
- [FreeBSD Developers' Handbook: Chapter 3. Secure Programming](https://docs.freebsd.org/en/books/developers-handbook/secure/)
- [GNU Autoconf: Portability of C Functions](https://www.gnu.org/savannah-checkouts/gnu/autoconf/manual/autoconf-2.71/autoconf.html#Function-Portability)
- [GNU Autoconf: Portable C and C++ Programming](https://www.gnu.org/savannah-checkouts/gnu/autoconf/manual/autoconf-2.71/autoconf.html#Portable-C-and-C_002b_002b)
- [Oracle Developer's Guide to Solaris: Security Considerations When Using C Functions](https://docs.oracle.com/cd/E36784_01/html/E36855/gnclc.html)

[`arm64`]: https://en.wikipedia.org/wiki/AArch64
[`x86_64`]: https://en.wikipedia.org/wiki/X86-64
[`clang`]: https://clang.llvm.org/
[`gcc`]: https://gcc.gnu.org/
[`tcc`]: https://bellard.org/tcc/
[`zig`]: https://ziglang.org/
[Neved4/asprintf]: https://github.com/Neved4/asprintf
[C]: https://en.wikipedia.org/wiki/C_(programming_language)
[C99]: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
[POSIX.1-2024]: https://pubs.opengroup.org/onlinepubs/9799919799/
[posix-asprintf]: https://pubs.opengroup.org/onlinepubs/9799919799/functions/asprintf.html
[posix-2024-announced]: https://ieeexplore.ieee.org/document/10555529
[MIT License]: https://opensource.org/license/mit/
[clib]: https://github.com/clibs/clib
[Docker]: https://www.docker.com/
[GLib]: https://docs.gtk.org/glib/index.html
[glibc]: https://www.gnu.org/software/libc/
[musl libc]: https://musl.libc.org/

[Debian `asprintf`]: https://sourceware.org/git/?p=glibc.git;a=blob;f=stdio-common/asprintf.c;h=bb8f4bffc19292c7173d8180b4c2428411d5c269;hb=HEAD
[FreeBSD `asprintf`]: https://github.com/lattera/freebsd/blob/master/lib/libc/stdio/asprintf.c#L46
[OpenBSD `asprintf`]: https://github.com/openbsd/src/blob/434871b630fbbd06e39f45c13bc9e199cfd986ee/lib/libc/stdio/asprintf.c#L30
[NetBSD `asprintf`]: https://github.com/IIJ-NetBSD/netbsd-src/blob/master/lib/libc/stdio/vasprintf.c
[sortix `asprintf`]: https://gitlab.com/sortix/sortix/-/commit/2fe13d33c9e1da46aa444367f16680cafa3a63a3
<!-- [Solaris `asprintf`]: -->
<!-- [z/OS `asprintf`]: -->

[manpages.debian.org/asprintf]: https://manpages.debian.org/unstable/manpages-dev/asprintf.3.en.html
[man.freebsd.org/asprintf]: https://man.freebsd.org/cgi/man.cgi?query=asprintf
[man.openbsd.org/asprintf]: https://man.openbsd.org/asprintf
[man.netbsd.org/asprintf.3]: https://man.netbsd.org/asprintf.3
[docs.oracle.com/asprintf-3c]: https://docs.oracle.com/cd/E88353_01/html/E37843/asprintf-3c.html
[ibm.com/docs/zos/asprintf]: https://www.ibm.com/docs/en/zos/3.1.0?topic=functions-asprintf-vasprintf-print-allocated-string

[^1]: _IEEE Std 1003.1-2024: Standard for Information Technology
    — Portable Operating System Interface (POSIX®)_, \
    ISO/IEC DIS 9945. URL: https://pubs.opengroup.org/onlinepubs/9799919799/
[^2]: _ISO/IEC 9899: Standard for Information Technology
    — Programming languages — C_, ISO/IEC 9899:2023. \
    URL: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf
