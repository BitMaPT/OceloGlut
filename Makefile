all: server client

client:
	ccgl main.c gamecontroller.c object.c putpoint.c ocelostone.c oceloboard.c syncgame.c mouse.c bitstring.c -o client.out -Wall -g
server:
	ccgl server.c syncgame.c ocelostone.c serverboard.c object.c putpoint.c -o server.out -Wall