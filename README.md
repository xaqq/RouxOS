RouxOS
======

Just a student kernel project. Developped in C++11.
It aims to be a monolithic, multi-threaded.

Where am I going ?
I simply want something functional. Not a kick-ass OS
that can emulate Windows, Linux, and Mac OS. Just
something working on my machine, that I can have fun with.

For information:
- I am working on an HP EliteBook 8540p (4 go RAM, Core i5 M520),
  from Gentoo Linux. I use g++ (up-to-date) and did not intend
  to run too old stuff (I hate the idea of keeping _too much_
  backward compatibility. Of course it's very much needed, but
  who need 16-bits support on a i7 ?)

- I use Qemu-kvm, on a floppy that has grub installed on it.

- I really like the work of the Qt team. I can't describe it better
  than "there's nothing I wanted to be done an other way".
  And since they have a 20-year experience, I plan to borrow a lot
  of concepts from them.

- English isn't my mother language.
- Feel free to mail me if you're interested / curious / have any advice.


Roadmap:
- RouxOS currently boot from grub, recognize the multiboot infos,
  then print a message (start / end / size).
- It has Map<Key, Value>, Deque<Value>, String, and Iterators.

- I plan to improve String, and provide few additional features
  (Array, Queue, Stack, List, ConstIterator).

- Then I'll need to provide some interruptions / exceptions handler.
  Be aware that at this point, even if I have a memory allocator,
  I still don't use paging (and segmentation is "disabled").

- Finally I'll implement an EventLoop. RouxOS will start an EventLoop
  per Core (I have 4 on my i5), that will be used to implement
  a type & thread-safe signal-slot system, using (variadic?) template.

- IPC = signal / slot. After that, I'll have to implement some paging.
  Then I can start writing user-space (and user-space module), starting
  with a shell.