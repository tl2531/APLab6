Emily Schultz, ess2183
CS3157 Lab 6

Part 1:
This part of my program works as specified. After you make, simply type in the mdb file and a port, like:
./mdb-lookup-server /home/jae/cs3157-pub/bin/cs3157-mdb 8000
Then, you can connect using:
nc <servername> 8000
The server computer will say Handling client <IP address>, 
and on the client, type in whatever you want to search, like:
Emily or <Enter>, then Ctrl+D to quit.
Then the server computer will be ready to take the next client who tries to connect. To end, press Ctrl+C. It will print a new line and exit.
I ran valgrind on this part of my code and it said all heap blocks were freed and no leaks are possible with 0 errors.

Part 2:
This part of my program works as specified. Simply type make, then you can request a webpage by typing a command like:
./http-client www.gnu.org 80 /software/make/manual/make.html
then you should have a file (in this case called make.html) in your current directory that contains only the HTML file content of that page.
If you made a typo and accidentally typed:
./http-client www.gnu.org 80 /software/make/manual/mak.html
then the page doesn't exist, so it will print out into the terminal the error
(in this case "HTTP\1.1 404 Not Found) and then the program will terminate.

