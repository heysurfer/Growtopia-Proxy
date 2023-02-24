mkdir -p build
g++ -std=c++2a -x c++-header Main/pch.h -o Main/pch.h.gch
g++ -std=c++2a -pthread -include Main/pch.h Main/Main.cpp -I/usr/local/ssl/include -L/usr/local/ssl/lib -lssl -lcrypto -o build/proxy.out
chmod +x build/proxy.out