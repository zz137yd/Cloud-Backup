#include <iostream>
#include <string>
#include <fstream>
#include "bundle.h"

int main(int argc, char* argv[])
{
    std::cout << "argv[1] is the original file path name \n";
    std::cout << "argv[2] is compressed package name \n";
    if(argc < 3) return -1;

    std::string ifilename = argv[1];
    std::string ofilename = argv[2];

    std::ifstream ifs;
    ifs.open(ifilename, std::ios::binary);
    ifs.seekg(0, std::ios::end);//Set the read and write(rw) position to the end of the file
    size_t fsize = ifs.tellg();//Get the offset of the current rw position compared to the initial position
    ifs.seekg(0, std::ios::beg);
    std::string body;
    body.resize(fsize);
    ifs.read(&body[0], fsize); 

    std::string packed = bundle::pack(bundle::LZIP, body);//Compress data in LZIP format

    std::ofstream ofs;
    ofs.open(ofilename, std::ios::binary);
    ofs.write(&packed[0], packed.size());

    ifs.close();
    ofs.close();
    return 0;
}
