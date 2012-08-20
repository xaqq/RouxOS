/*
 * This function is only here for the pleasure of ld.
 * It'll be called if a virtual call cannot be made (hope it won't).
 */

// GCC

extern "C" void __cxa_pure_virtual()
{
  // Do nothing or print an error message.
}

// Visual Studio
// int __cdecl _purecall()
// {
//   // Do nothing or print an error message.
// }
