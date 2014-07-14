CC=g++
CFLAGS=-Wall -g 
OBJ=localsearch


$(OBJ) :  Search.o dict.o  SearchServer.o UserFace.o
	$(CC) $(CFLAGS) -o $@ $^

UserFace.o: UserFace.cc
	$(CC) $(CFLAGS) -c $?
Search.o :  Search.cc
	$(CC) $(CFLAGS) -c $?
dict.o :  dict.cc
	$(CC) $(CFLAGS) -c $?
SearchServer.o : SearchServer.cc
	$(CC) $(CFLAGS) -c $?


clean:
	rm -f $(OBJ) *.o
