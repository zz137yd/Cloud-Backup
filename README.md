# Cloud-Backup
ZYD's project

I've tried it before and it was uploaded successfully, but couldn't be displayed.  
I implemented the client on VS2019. You can add header <windows.h> under #include "cloud.hpp" or not.  
Now I'll put step here.In order to allow the server to complete compilation faster.  

<br>********************************************************************************************

<br>g++ -c bundle.cpp -o bundle.o -std=c++11&nbsp;&nbsp;&nbsp;&nbsp;//Can be compiled with C++14, but not C++17  
  
ar -cr libbundle.a bundle.o  

rm -rf bundle.o&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;//Can also rm bundle.cpp by the way  
  
mkdir lib  

mv libbundle.a lib 

<br>********************************************************************************************

<br>makefile change:  
<br>cloud:cloud.cpp bundle.cpp  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;g++ -g $^ -o $@ -lpthread -lstdc++fs -ljsoncpp -std=c++11
<br>cloud:cloud.cpp  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;g++ -g $^ -o $@ -L./lib -lpthread -lstdc++fs -ljsoncpp -lbundle -std=c++11  
