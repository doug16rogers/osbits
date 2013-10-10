// This code is in the public domain. Use it as you wish.
// Author: Doug Rogers.

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500     // All the way back to Win2K.
#endif

#include <stdio.h>
#include <windows.h>

#define PROGRAM "osbits"

#ifdef _WIN64
#error "Not useful to build this for x64."
#endif

// ----------------------------------------------------------------------------
/**
 * @return the number of bits in the OS, or 0 if it could not be determined.
 */
int os_bits()
{
  int bits = 0;
  HMODULE lib = LoadLibraryA("kernel32");

  if (lib) {
    BOOL is_64 = FALSE;
    typedef BOOL (WINAPI* ISWOW64PROCESS)(HANDLE, PBOOL);
    ISWOW64PROCESS is_wow64_process =
                     (ISWOW64PROCESS) GetProcAddress(lib, "IsWow64Process");

    // IsWow64Process() returns non-zero on success (making is_64 valid).
    if (is_wow64_process && is_wow64_process(GetCurrentProcess(), &is_64))
      bits = is_64 ? 64 : 32;

    FreeLibrary(lib);
  }

  return bits;
}   // os_bits()

// ----------------------------------------------------------------------------
/**
 * Prints the number of bits in the OS, or 0 on error.
 * @return 0 on success, something else on error.
 */
int main(int argc, char* argv[])
{
  if (argc > 1) {
    fprintf(stderr,
            "\n"
            "Usage: " PROGRAM "\n"
            "\n"
            PROGRAM " prints the number of bits in the OS, or 0 on error.\n"
            "Returns exit code 0 on success, something else on error.\n"
            "\n"
            "This software by Doug Rogers is in the public domain.\n"
            "\n");
    return 1;
  }

  int bits = os_bits();
  printf("%u\n", bits);
  return (bits == 0) ? 2 : 0;
}
