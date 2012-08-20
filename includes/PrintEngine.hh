#pragma once

#define TVIDEORAM	((u8*)0xB8000)
#define LINESIZE	(80 * 2)
#define SCREENSIZE	(LINESIZE * 25)

class String;

class PrintEngine {
 public:

  enum Color {
    /* Values for setAttr(bool, char, bool, char) */
    Black	= 0b000,
    Blue	= 0b001,
    Green	= 0b010,
    Cyan	= 0b011,
    Red		= 0b100,
    Magenta	= 0b101,
    Yellow	= 0b110,
    White	= 0b111
  };

  static PrintEngine&	inst(void);

  void		operator()(const String&);
  void		operator()(const char*);
  void		setAttr(bool blink, char backgroundColor,
			bool intensity, char foregroundColor);
  void		setAttr(int encoded);
  void		scroll(int line = 1);
  
 protected:
  PrintEngine(void);

  u8* const	videoram;
  u32		kAttr;
  u32		kX;
  u32		kY;
};

extern PrintEngine&	kprint;
