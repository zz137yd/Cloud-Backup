#ifndef __MY_CONFIG__
#define __MY_CONFIG__

#include "util.hpp"
#include <mutex>

namespace cloud
{
    #define CONFIG_FILE "./cloud.conf"//Configuration file

    class Config
    {
    private:
        Config() { ReadConfigFile(); }
        static std::mutex _mutex;
        static Config* _instance;
    private:
        int _hot_time;//Hotspot judgment time
        int _server_port;//Server listening port
        std::string _server_ip;//Server IP address
        std::string _download_prefix;//Url prefix path for download
        std::string _packfile_suffix;//Compressed package suffix name
        std::string _back_dir;//Directory to store backup files
        std::string _pack_dir;//Directory to store compressed packages
        std::string _backup_file;//File that store data information

        bool ReadConfigFile()
        {
            Util fu(CONFIG_FILE);
            std::string body;
            //The configuration file has been set to CONFIG_FILE
            //GetContent function will take out the content from this file and put it into the body
            if(fu.GetContent(&body) == false)
            {
                std::cout << "Failed to load configuration file \n";
                return false;
            }
            //After reading the data, the body is a string in Json format.
            Json::Value root;
            if(JsonUtil::Deserialize(body, &root) == false)
            {
                std::cout << "Failed to parse configuration file \n";
                return false;
            }
            _hot_time = root["hot_time"].asInt();
            _server_port = root["server_port"].asInt();
            _server_ip = root["server_ip"].asString();
            _download_prefix = root["download_prefix"].asString();
            _packfile_suffix = root["packfile_suffix"].asString();
            _back_dir = root["back_dir"].asString();
            _pack_dir = root["pack_dir"].asString();    
            _backup_file = root["backup_file"].asString();
            return true;
        }
    public:
        static Config* GetInstance()
        {
            if(_instance == NULL)
            {
                _mutex.lock();
                if(_instance == NULL) _instance = new Config();
                _mutex.unlock();
            }
            return _instance;
        }

        int GetHotTime() { return _hot_time; }
        int GetServerPort() { return _server_port; }
        std::string GetServerIp() { return _server_ip; }
        std::string GetDownloadPrefix() { return _download_prefix; }
        std::string GetPackFileSuffix() { return _packfile_suffix; }
        std::string GetBackDir() { return _back_dir; }
        std::string GetPackDir() { return _pack_dir; }
        std::string GetBackupFile() { return _backup_file; }
    };

    Config* Config::_instance = NULL;
    std::mutex Config::_mutex;
}

#endif