In this exercise we can see many file opening and different writings.
The first open just check the logs written in .log. Our goal is to open the .pass.
We can check when this step has been done when the .log have the message "Finished back up ".
The second fopen will be the one to open the .pass and put the answer into the stream then the stream 
will be written into the fd of (dest).

So we have to create an architecture that will open /home/users/level09/.pass and that will also be 
openable by open function inside a backups folder. the open function will open this path: 
./backups + argv[1] => so we need to create the good path to open at the same time /home/users/level09/.pass and to open /backups/home/users/level09/.pass. With the utilisation of the relative path.

So first thing first is to create our architecture, we can only create it inside /tmp:

mkdir -p ./backups/home/users/level09/

Attention! Do not create the .pass file because the open function has some flags:
fd = open(dest, 193, 432LL);

The open system call is used to open the file specified by dest with flags 193 (O_CREAT | O_WRONLY) and permissions 432 (0660). If the file fails to open, an error message is printed, and the program exits with a status of 1.

then execute the program:

~/level08 /home/users/level09/.pass

it will open /home/users/level09/.pass and put it into the stream and then it will put the stream 
into the fd (dest) so it will write into the file ./backups/home/users/level09/.pass

then the only thing to cat the .pass

cat backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S