all: server client

client:
	ccgl main.c gamecontroller.c object.c string3d.c putpoint.c ocelostone.c oceloboard.c syncgame.c mouse.c -o client.out
server:
	ccgl server.c syncgame.c ocelostone.c serverboard.c object.c string3d.c putpoint.c -o server.out