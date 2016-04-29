Author:

	Zack Arnett | arnett.zackary@gmail.com | 4/26/2016

Contents:

	rsh-client.c:
		
		This is the c file that contains the c code for my client

	rshell.h:

		This is just a copy of the shell.h file from PRGM4 I am using it for parsing commands
	
	rsh-server.c:
	
		This is the c file that contains the ccode for my server

	parser.c:

		This is the given file from PRGM4 that seperates command lines

	csapp.c:

		This is the given file with the code for all the wrapper functions
	
	csapp.h:

		This is the header file for csapp.c

	Makefile:

		This is the file that makes two executable (server and client) as well as cleans and zips my files

Running:

	To run my server you will need to call the program with the argument of a port number for example ./rrsh-server 2000 where 2000 is the Port number. This will start the server and it will then listen for a client to connect to. 
	To run my client you will need call the program a host and a port number. For example, ./rrsh-client localhost 2000 where localhost is the host and 2000 is the port number which also has to be the same as the server port. This will connect the client to the corresponding server.

Implementation notes:

	I began the program using the echo server and client given so that the socket connection was already done. I used the wrapper functions in the file csapp.c to make it a little easier for error handling. I also implemented an execv in to my server instead of using the wrapper execve. This was the requirement for the program so I at first made in the csapp.c but moved it to the server file just in case the csapp.c was universal. I also told the server the client disconnected by sending the keyword quit. Same thing but I sent login failed to tell the client that the server did not like the username or password and that they could not login

Limitations:

	I used thelimitation of MAXLINE for the user inputs and all the read and writen functions. I noticed that my program would core dump if there was a newline on any file. I didnt fix the bug because I could not find a way with the wrapper Fgets function. 

References:

	Evan Heaton helped me with the understanding of the Rio functions
