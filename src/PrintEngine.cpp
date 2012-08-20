#include	"KBase.hh"
#include	"PrintEngine.hh"
#include	"CString.hh"
#include	"String.hh"

/*
 * 0 <= kX < 25
 * 0 <= kY < 80
 * http://a.michelizza.free.fr/uploads/TutoOS/video_memory_attribut.png
 */

PrintEngine&	kprint = PrintEngine::inst();

PrintEngine&		PrintEngine::inst(void)
{
  static PrintEngine	i;
  return (i);
}

PrintEngine::PrintEngine(void) : videoram(TVIDEORAM), kX(0), kY(0)
{
  setAttr(false, Black, false, White);
}

void	PrintEngine::scroll(int lines)
{
  while (lines && kY) {
    memcpy(TVIDEORAM, TVIDEORAM + LINESIZE, SCREENSIZE - LINESIZE);
    memset(TVIDEORAM + (SCREENSIZE - LINESIZE), 0, LINESIZE);
    --kY;
    --lines;
  }
}

void	PrintEngine::operator()(const char* msg)
{
  int	i = 0;

  while (msg[i]) {
    if (msg[i] == '\t')
      kX += 4;
    if (msg[i] == '\n') {
      kX = 0;
      ++kY;
    }
    if (msg[i] == '\r')
      kX = 0;

    if (msg[i] == '\e' && msg[i + 1]) {
      kAttr = (kAttr & 0b000) | ((msg[i + 1] - '0') & 0b0111);
      ++i;
    }
    else if (msg[i] >= ' ' && msg[i] <= '~') { /* Is visible */
      videoram[kX * 2 + kY * LINESIZE] = msg[i];
      videoram[kX * 2 + kY * LINESIZE + 1] = kAttr;
      ++kX;
    }
    if (kX >= 80) {
      kX = 0;
      ++kY;
    }
    if (kY >= 25)
      scroll();
    ++i;
  }
}

void		PrintEngine::operator()(const String& i)	{ (*this)(i.cstr()); }

void		PrintEngine::setAttr(bool blink, char backgroundColor,
			bool intensity, char foregroundColor)
{
  // FIXME : Blink not working? Or just Qemu...
  int	enc = ((blink) ? (1 << 7) : (0)) + ((backgroundColor & 0b111) << 4)
    + ((intensity) ? (1 << 3) : (0)) + ((foregroundColor & 0b111));
  setAttr(enc);
}

void		PrintEngine::setAttr(int encoded)
{
  kAttr = encoded;
}
