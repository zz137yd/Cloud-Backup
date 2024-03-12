# Cloud-Backup
ZYD's project

I've tried it before and it was uploaded successfully, but couldn't be displayed  
Now I'll put step here.For faster and no-warning compilation  
  
<br><br>g++ -c bundle.cpp -o bundle.o -std=c++11&nbsp;&nbsp;//C++14 can be compiled, but C++17 can't  
  
ar -cr libbundle.a bundle.o  

rm -rf bundle.o&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;//Can also rm bundle.cpp by the way  
  
mkdir lib  

mv libbundle.a lib  
