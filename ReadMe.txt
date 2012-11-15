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

Part 2:
