CC=gcc
CXX=g++

implant.exe:
	$(CXX) implant.cpp -lwinhttp -o bin/implant.exe

tasks.exe:
	$(CXX) tasks.cpp -o bin/tasks.exe

aes_test.exe:
	$(CXX) aes_gcm.cpp  test_aesgcm.cpp  -lcrypt32 -lbcrypt -static  -o bin/aes_test.exe

aescp:
	$(CXX) AESCOPIED.cpp -o aescp.exe