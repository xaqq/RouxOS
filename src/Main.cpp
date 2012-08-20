#include	"Kernel.hh"
#include    "EventLoop.hh"

void	kmain(void* mb_infos)
{
    u32*		mdb = (u32*)mb_infos;
    size_t      kStart;
    size_t      kEnd;

    // memAlloc->showAllocatedMemory();
    // while(42);

    asm("movl $kernel_start, %0" : "=r"(kStart));
    assert(kStart == KERNEL_TEXT_BOT);
    asm("movl $kernel_end, %0" : "=r"(kEnd));
    assert(kEnd < KERNEL_TEXT_TOP);

     kprint(StaticString("Kernel goes from 0x")
     	 .append(StaticString::number(kStart, 16))
     	 .append(" to 0x").append(StaticString::number(kEnd, 16))
     	 .append("\nKernel size is ")
     	 .append(StaticString::number(kEnd - kStart, 10))
     	 .append("\n"));

//    kprint(String("Kernel goes from 0x%1 to 0x%2\nKernel size is %3\n")
//           .arg(String::number(kStart, 16), String::number(kEnd, 16),
//                String::number(kEnd - kStart, 10)));
     //kprint("Kikou2\n");

    kprint(StaticString("\nAdress of kmain is: 0x")
           .append(StaticString::number((size_t)(&kmain), 16))
           .append("\nAdress of mb_infos is: 0x")
           .append(StaticString::number(mb_infos, 16))
           .append("\n"));

    //assert((size_t)&kmain == 0x1006F4);
    assert((size_t)mb_infos == 0x31334);

    while (42);
}
