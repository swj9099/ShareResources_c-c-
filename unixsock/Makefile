all:server client

server:Usock.cpp servermain.cpp Userver.cpp
	g++ -g -o server Userver.cpp Usock.cpp servermain.cpp -lpthread
client:Usock.cpp clientmain.cpp Uclient.cpp
	g++ -g -o client Uclient.cpp Usock.cpp clientmain.cpp
clean:
	rm *.o server client -f
