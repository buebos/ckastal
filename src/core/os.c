#ifndef __CKASTAL_CORE_OS_C__
#define __CKASTAL_CORE_OS_C__

#ifdef __linux__
#define CKASTAL_CORE_OS_LINUX
#elif __APPLE__
#define CKASTAL_CORE_OS_MACOS
#elif defined(_WIN32) || defined(WIN32)
#define CKASTAL_CORE_OS_WINDOWS
#endif

#endif /* __CKASTAL_CORE_OS_C__ */