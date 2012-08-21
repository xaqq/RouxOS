#include	"KBase.hh"
#include	"PrintEngine.hh"

extern "C" void	InterruptDefault(void)
{
  kprint("DefaultInterrupt\n");
}

extern "C" void	InterruptClock(void)
{
  kprint("Tick\n");
}

extern "C" void	InterruptKeyboard(void)
{
  kprint("Keyboard key pressed\n");
}
