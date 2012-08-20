#include	"Kernel.hh"

/*
 * The main boot process.
 * This function have name mangling disabled because it's called from ASM code.
 * Other function are located in namespace 'Boot'.
 * See: http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh
 */

extern "C" void	boot_kernel(void* mb_infos, unsigned int magic)
{
  memset(TVIDEORAM, 0, SCREENSIZE);
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    kprint("\e4Bootloader is not multiboot compliant!\e7\n");
    kprint("\e4Hurrah! Kernel Panic: Waiting forever...\e7");
    while (42);
  }

  if (!Boot::checkMultibootInfos(mb_infos))
    while (42);

  kprint("Reloading gdt...\t\t");
  Boot::init_gdt();
  kprint("[\e2 OK \e7]\n");
  /* TSS */
  kmain(mb_infos);
}

namespace Boot {

  void	init_gdt_desc(u32 base, u32 limit, u8 acces, u8 other,
			   struct gdtdesc *desc)
  {
    desc->lim0_15 = (limit & 0xffff);
    desc->base0_15 = (base & 0xffff);
    desc->base16_23 = (base & 0xff0000) >> 16;
    desc->acces = acces;
    desc->lim16_19 = (limit & 0xf0000) >> 16;
    desc->other = (other & 0xf);
    desc->base24_31 = (base & 0xff000000) >> 24;
    return;
  }

  void	init_gdt(void)
  {
    struct gdtdesc	kgdt[GDTSIZE];	/* GDT */
    struct gdtr		kgdtr;		/* GDTR */

    /* initialisation des descripteurs de segment */
    init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);		/* NULL */
    init_gdt_desc(0x0, 0xFFFFFFFF, 0x9B, 0x0D, &kgdt[1]);	/* code */
    init_gdt_desc(0x0, 0xFFFFFFFF, 0x93, 0x0D, &kgdt[2]);	/* data */
    init_gdt_desc(0x0, 0xFFFFF, 0x97, 0x0D, &kgdt[3]);		/* stack */

    /* initialisation de la structure pour GDTR */
    kgdtr.limite = GDTSIZE * 8;
    kgdtr.base = GDTBASE;

    /* recopie de la GDT a son adresse */
    // FIXME  : I can see a pb in 64b mode...
    memcpy((u8*)kgdtr.base, (u8*)kgdt, kgdtr.limite);

    /* chargement du registre GDTR */
    // FIXME : Portability -compiler- issue
    //    asm("lgdtl (_ZN4Boot5kgdtrE)"); // Name mangling ftw

    /* initialisation des segments */
    asm("   movw $0x10, %ax\n \
            movw %ax, %ds\n \
            movw %ax, %es\n \
            movw %ax, %fs\n \
            movw %ax, %gs\n \
            ljmp $0x08, $reinitsp\n \
            reinitsp:\n");
  }

  bool	checkMultibootInfos(void* mb)
  {
    u32*		mdb = (u32*)mb;

    kprint("Configuration:\nFlags: ");
    kprint(StaticString::number((size_t)mdb[0], 2));

    assert(mdb[0] & MULTIBOOT_INFO_MEMORY);
    kprint("\nMemory: (lower) ");
    kprint(StaticString::number((size_t)mdb[1], 10));
    kprint(" - (Upper) ");
    kprint(StaticString::number((size_t)mdb[2], 10));

    // assert(mdb[0] & MULTIBOOT_INFO_BOOTDEV);
    // kprint("\nBoot device: "));
    // kprint(StaticString::number((size_t)mdb[3], 2));

    assert(mdb[0] & MULTIBOOT_INFO_CMDLINE);
    kprint("\nCommand line: ");
    kprint((const char*)mdb[4]);
    kprint("\n");

    //assert(mdb[0] & MULTIBOOT_INFO_MODS);
    if (mdb[0] & MULTIBOOT_INFO_MODS)
      kprint("There's a MULTIBOOT_INFO_MODS\n");

    if (mdb[0] & MULTIBOOT_INFO_AOUT_SYMS)
      kprint("There's a MULTIBOOT_AOUT_SYMS\n");
    if (mdb[0] & MULTIBOOT_INFO_ELF_SHDR)
      kprint("There's a MULTIBOOT_ELF_SHDR\n");

    if (mdb[0] & MULTIBOOT_INFO_MEM_MAP)
      kprint("There's a MULTIBOOT_INFO_MEM_MAP\n");
    if (mdb[0] & MULTIBOOT_INFO_DRIVE_INFO)
      kprint("There's a MULTIBOOT_INFO_DRIVE_INFO\n");
    if (mdb[0] & MULTIBOOT_INFO_CONFIG_TABLE)
      kprint("There's a MULTIBOOT_INFO_CONFIG_TABLE\n");
    if (mdb[0] & MULTIBOOT_INFO_BOOT_LOADER_NAME)
      kprint("There's a MULTIBOOT_INFO_BOOT_LOADER_NAME\n");
    if (mdb[0] & MULTIBOOT_INFO_APM_TABLE)
      kprint("There's a MULTIBOOT_INFO_APM_TABLE\n");
    if (mdb[0] & MULTIBOOT_INFO_VIDEO_INFO)
      kprint("There's a MULTIBOOT_INFO_VIDEO_INFO\n");
    return (true);
  }
};
