# Functions to get frame pointer and stack pointer

    .text
    .align 8             # align by 8 words
    .globl getFP, getSP  # global function

getFP:
    movq %rbp, %rax # Move value of frame pointer into %rax return register
    ret


getSP:
    movq %rsp, %rax # Move value in stack pointer into %rax return register
    ret
