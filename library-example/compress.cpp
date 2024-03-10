#include <iostream>
#include <string>
#include <fstream>
#include "bundle.h"

int main(int argc, char* argv[])
{
    std::cout << "argv[1] 是原始文件路径名称\n";
    std::cout << "argv[2] 是压缩包名称\n";
    if(argc < 3) return -1;

    std::string ifilename = argv[1];
    std::string ofilename = argv[2];

    std::ifstream ifs;
    ifs.open(ifilename, std::ios::binary);
    ifs.seekg(0, std::ios::end);//读写位置走到文件末尾
    size_t fsize = ifs.tellg();//获取当前读写位置相较于初始位置的偏移量
    ifs.seekg(0, std::ios::beg);
    std::string body;
    body.resize(fsize);
    ifs.read(&body[0], fsize); 

    std::string packed = bundle::pack(bundle::LZIP, body);//以LZIP格式来压缩数据

    std::ofstream ofs;
    ofs.open(ofilename, std::ios::binary);
    ofs.write(&packed[0], packed.size());

    ifs.close();
    ofs.close();
    return 0;
}
