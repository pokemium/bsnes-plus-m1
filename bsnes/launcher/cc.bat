@windres resource.rc resource.o
@mingw32-g++ -std=gnu++11 -mwindows -s -O3 -fomit-frame-pointer -I.. -o ../out/bsnes launcher.cpp resource.o
@del *.o
@pause
