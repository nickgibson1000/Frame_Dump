# Frame_Dump
`Frame_Dump` is a low-level utility written in C and Intel 64-bit assembly that traverses and prints stack frames at runtime. It provides a detailed view of active function frames and their contents without relying on a debugger.

---

## Overview

The program captures the current values of the frame pointer (`%rbp`) and stack pointer (`%rsp`) and walks the call stack by following saved frame pointers. It prints:

- The address of each frame
- The saved return address at each frame
- A full stack dump from the current `%rsp` up to the final saved frame
- Local variables and other data stored within each stack frame

---

## File Descriptions

- `frames.c`: Core C implementation. Allocates structures for traversing the stack, prints frame and stack contents.
- `asm.s`: Contains two functions written in Intel 64-bit assembly to return the current `%rbp` and `%rsp` values:
  - `getFP`: Returns the value of the frame pointer register
  - `getSP`: Returns the value of the stack pointer register
 
  
---

## Build Instructions

To compile:

```bash
gcc -no-pie -o frame_dump frames.c asm.s


