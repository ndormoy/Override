For this level we use brute force to crack the programm.

by analyzing the decrypt function we've learn we need to perform a XOR operation between a1 and "Q}|u`sfg~sf{}|a3"

and then if the result is not equal to "Congratulations!"
then it opens a new shell.

We've made a little cpp programm to test all the posibilities and we find that we need to put the character 18 of the ascii table inside the decrypt function.

then if we go to the test function we can see we have 
this operation a2 - a1 = 18 => 322424845 - a1 = 18

so a1 = 322424845 - 18 = 322424827

level03@OverRide:~$ ./level03 
***********************************
*		level03		**
***********************************
Password:322424827
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
$ 
