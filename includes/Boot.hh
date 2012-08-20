#ifndef __BOOT_HH__
# define __BOOT_HH__

#include	"Multiboot.h"

namespace Boot {
  /* A GDT entry */
  /*
   * la base, sur 32 bits, est l'adresse lineaire ou debute le segment en mémoire
   * la limite, sur 16 bits, definit la longueur du segment.
   * == si le bit G est a 0, la limite est exprimee en octets,
   * == sinon, elle est exprimee en nombre de pages de 4 ko.
   * le bit D/B precise la taille des instructions et des donnees manipulees.
   * == Il est mis a 1 pour 32 bits.
   * le bit AVL est librement disponible.
   * le bit P est utilise pour determiner si le segment est present
   * == en memoire physique. Il est a 1 si c'est le cas.
   * le DPL indique le niveau de privilege du segment.
   * == Le niveau 0 correspond au mode super-utilisateur.
   * le bit S est mis a 1 pour un descripteur de segment et a 0 pour
   * == un descripteur systeme (un genre particulier de descripteur).
   * le type definit le type de segment (code, donnees ou pile).
   */
  //extern u8	*gdtseg;

# define GDTBASE    0x0		/* addr. physique ou doit resider la gdt */
# define GDTSIZE    0xFF	/* nombre max. de descripteurs dans la table */

  /* Descripteur de segment */
  struct gdtdesc {
    u16 lim0_15;
    u16 base0_15;
    u8 base16_23;
    u8 acces;
    u8 lim16_19:4;
    u8 other:4;
    u8 base24_31;
  } __attribute__ ((packed));

  /* Registre GDTR */
  struct gdtr {
    u16 limite;
    u32 base;
  } __attribute__ ((packed));

  void	init_gdt_desc(u32, u32, u8, u8, struct gdtdesc *);
  void	init_gdt(void);
  bool	checkMultibootInfos(void*);
};
#endif /* !__BOOT_HH__ */
