On this level we can see that we have canary found.
So we cannot inject shellcode.

After reading the decompilated code we can see that our login must be 5 characters long. and we have also a ptrace to prevent us from debugging the program.

after the ptrace we have a sort of encryption with a serial number, our goal is to find the serial number.

If we find the serial number we can launch a /bin/sh with the serial number as argument.

So we've made a little C++ script to find the serial number.

for the login "saluttoi" Serial found: 6234471.

level06@OverRide:~$ ./level06 
***********************************
*		level06		  *
***********************************
-> Enter Login: saluttoi
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6234471
Authenticated!
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8