SRC = /home/stam/workspace/chat/src
BUILD = /home/stam/workspace/chat/build
INSTALL = /home/stam/workspace/chat/release

all: chat

chat:chat.o
	g++ -o chat chat.o -std=c++17 -lpthread

chat.o: $(SRC)/chat.cpp
	g++ -o chat.o -c $(SRC)/chat.cpp -lpthread

clean:
	rm -rf *.o

install:
	mkdir -p $(INSTALL)
	cp $(SRC)/chat.cpp  $(INSTALL)
	cp $(BUILD)/chat  $(INSTALL)
