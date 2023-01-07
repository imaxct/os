.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1badb002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
  .long MAGIC
  .long FLAGS
  .long CHECKSUM

.section .text
.extern call_constructors
.extern kernel_main
.global _start

_start:
  mov $stack_top, %esp

  call call_constructors

  push %eax
  push %ebx
  call kernel_main

_stop:
  cli
  hlt
  jmp _stop

.section .bss
.space 2*1024*1024;
stack_top: