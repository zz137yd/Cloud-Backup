#ifndef __MY_UTIL__
#define __MY_UTIL__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <experimental/filesystem>
#include "bundle.h"
#include <jsoncpp/json/json.h>

namespace cloud
{
    //Rename std::experimental::filesystem namespace to fs
    namespace fs = std::experimental::filesystem;

    class Util
    {
    public:
        Util(const std::string& filename):_filename(filename) {}

        int64_t FileSize()
        {
            //Use the stat function to get file attributes
            //Header<sys/stat.h>
            struct stat st;
            if(stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "Failed to get file size \n";
                return -1;
            }
            return st.st_size;
        }

        time_t LastMTime()
        {
            struct stat st;
            if(stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "Failed to get file last modified time \n";
                return -1;
            }
            return st.st_mtime;
        }

        time_t LastATime()
        {
            struct stat st;
            if(stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "Failed to get file Last access time \n";
                return -1;
            }
            return st.st_atime;
        }

        std::string FileName()
        {
            size_t pos = _filename.find_last_of("/\\");//Common writing method(Linux Windows)
            if(pos == std::string::npos) return _filename;
            //return fs::path(_filename).filename().string() Get the file name with the path removed
            return _filename.substr(pos + 1);
        }

        bool GetPosLen(std::string* body, size_t pos, size_t len)//Specify starting position and length for reading
        {
            size_t fsize = this->FileSize();
            if(pos > fsize)
            {
                std::cout << "Error position value \n";
                return false; 
            }
            else if(pos + len > fsize)
            {
                std::cout << "Error length value \n";
                return false;
            }

            std::ifstream ifs;
            ifs.open(_filename, std::ios::binary);
            if(ifs.is_open() == false)
            {
                std::cout << "Failed to open file(For reading) \n";
                return false; 
            }
        
            ifs.seekg(pos, std::ios::beg);
            body->resize(len);
            ifs.read(&(*body)[0], len);
            if(ifs.good() == false)
            {
                std::cout << "Failed to get file content \n";
                ifs.close();
                return false;
            }
            ifs.close();
            return true;
        }

        bool GetContent(std::string* body)//Read all content
        {
            size_t fsize = this->FileSize();
            return GetPosLen(body, 0, fsize);
        }

        bool SetContent(const std::string& body)//Write
        {
            std::ofstream ofs;
            ofs.open(_filename, std::ios::binary);
            if(ofs.is_open() == false)
            {
                std::cout << "Failed to open file(For writing) \n";
                return false;
            }
            ofs.write(&body[0], body.size());
            if(ofs.good() == false)
            {
                std::cout << "Failed to write contents \n";
                ofs.close();
                return false;
            }
            ofs.close();
            return true;
        }

        bool Compress(const std::string& packname)
        {
            //1、Get source file data
            std::string body;
            if(this->GetContent(&body) == false)
            {
                std::cout << "Failed to get file content(Compress) \n";
                return false;
            }
            //2、Compress data
            std::string packed = bundle::pack(bundle::LZIP, body);
            //3、Put data into compressed file
            Util fu(packname);
            if(fu.SetContent(packed) == false)
            {
                std::cout << "compress write packed data failed!\n";
                return false;
            }
            return true;
        }

        bool UnCompress(const std::string& filename)
        {
            //Read the current compressed package data
            std::string body;
            if(this->GetContent(&body) == false)
            {
                std::cout << "Failed to get file content(Uncompress) \n";
                return false;
            }
            //Decompress compressed data
            std::string unpacked = bundle::unpack(body);
            //Write decompressed data to new file
            Util fu(filename);
            if(fu.SetContent(unpacked) == false)
            {
                std::cout << "Failed to write decompressed data \n";
                return false;
            }
            return true;
        }

        bool Exists() { return fs::exists(_filename); }

        bool CreateDirectory()
        {
            if(this->Exists()) return true;
            return fs::create_directories(_filename);
        }

        bool ScanDirectory(std::vector<std::string>* arry)//Browse file irectory
        {
            for(auto& p : fs::directory_iterator(_filename))
            {
                //If it's a directory, continue
                if(fs::is_directory(p) == true) continue;
                //Get the file name with relative path and convert it into a string
                arry->push_back(fs::path(p).relative_path().string());
            }
            return true;
        }

        bool Remove()
        {
            if (this->Exists() == false) return true;
            remove(_filename.c_str());
            return true;
        }

    private:
        std::string _filename;
    };

    class JsonUtil
    {
    public:
        static bool Serialize(const Json::Value& root, std::string* str)
        {
            Json::StreamWriterBuilder swb;
            std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
            std::stringstream ss;
            if(sw->write(root, &ss) != 0)
            {
                std::cout << "Json write failed \n";
                return false;
            }
            *str = ss.str();
            return true;
        }

        static bool Deserialize(const std::string& str, Json::Value* root)
        {
            Json::CharReaderBuilder crb;
            std::unique_ptr<Json::CharReader> cr(crb.newCharReader());
            std::string err;
            bool ret = cr->parse(str.c_str(), str.c_str() + str.size(), root, &err);
            if (ret == false)
            {
                std::cout << "parse error: " << err << std::endl;
                return false;
            }
            return true;
        }
    };
}

#endif
