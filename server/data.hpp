#ifndef __MY_DATA__
#define __MY_DATA__

#include "config.hpp"
#include <unordered_map>
#include <pthread.h>

namespace cloud
{
    typedef struct BackupInfo
    {
        bool pack_flag;//Compression flag
        size_t fsize;//File size
        time_t atime;//Last access time
        time_t mtime;//Last modified time
        std::string real_path;//Actual file storage path name
        std::string pack_path;//Compressed package storage path name
        std::string url;

        bool NewBackupInfo(const std::string& realpath)//Set new backup information
        {
            Util fu(realpath);
            if(fu.Exists() == false)
            {
                std::cout << "New backupinfo: The file doesn't exist \n";
                return false;
            }
            Config* config = Config::GetInstance();
            std::string packdir = config->GetPackDir();
            std::string packsuffix = config->GetPackFileSuffix();
            std::string download_prefix = config->GetDownloadPrefix();
            this->pack_flag = false;
            this->fsize = fu.FileSize();
            this->mtime = fu.LastMTime();
            this->atime = fu.LastATime();
            this->real_path = realpath;
            this->pack_path = packdir + fu.FileName() + packsuffix;//Compressed package path
            this->url = download_prefix + fu.FileName();
            return true;
        }
    }BackupInfo;

    class DataManager
    {
    private:
        std::string _backup_file;//Persistent storage file
        std::unordered_map<std::string, BackupInfo> _table;//Stored in memory as hash table
        pthread_rwlock_t _rwlock;//Read-write lock, read sharing, write mutex
    public:
        DataManager()
        {
            _backup_file = Config::GetInstance()->GetBackupFile();
            pthread_rwlock_init(&_rwlock, NULL);
            InitLoad();
        }

        ~DataManager() { pthread_rwlock_destroy(&_rwlock); }

        bool Insert(const BackupInfo& info)
        {
            pthread_rwlock_wrlock(&_rwlock);//Add write lock
            _table[info.url] = info;
            pthread_rwlock_unlock(&_rwlock);
            Storage();
            return true;
        }

        bool Update(const BackupInfo& info)
        {
            pthread_rwlock_wrlock(&_rwlock);
            _table[info.url] = info;//It can be overwritten and written because of hash table
            pthread_rwlock_unlock(&_rwlock);
            Storage();
            return true;
        }

        /*bool IU(const BackupInfo& info)
        {
            pthread_rwlock_wrlock(&_rwlock);
            _table[info.url] = info;
            pthread_rwlock_unlock(&_rwlock);
            Storage();
            return true;
        }*/

        bool GetOneByUrl(const std::string& url, BackupInfo* info)//Get data based on url
        {
            pthread_rwlock_wrlock(&_rwlock);
            //url是key值
            auto it = _table.find(url);
            if(it == _table.end())
            {
                pthread_rwlock_unlock(&_rwlock);
                return false;
            }
            *info = it->second;
            pthread_rwlock_unlock(&_rwlock);
            return true;
        }

        bool GetOneByRealPath(const std::string& realpath, BackupInfo* info)//Get data based on path
        {
            pthread_rwlock_wrlock(&_rwlock);
            auto it = _table.begin();
            for(; it != _table.end(); ++it)
            {
                if(it->second.real_path == realpath)
                {
                    *info = it->second;
                    pthread_rwlock_unlock(&_rwlock);
                    return true;
                }
            }
            pthread_rwlock_unlock(&_rwlock);
            return false;
        }

        bool GetAll(std::vector<BackupInfo>* arry)
        {
            pthread_rwlock_wrlock(&_rwlock);
            auto it = _table.begin();
            for(; it != _table.end(); ++it)
            {
                arry->push_back(it->second);
            }
            pthread_rwlock_unlock(&_rwlock);
            return true;
        }

        //Every time data is added or modified, it must be stored persistently to avoid data loss
        bool Storage()
        {
            //1、Get all data
            std::vector<BackupInfo> arry;
            this->GetAll(&arry);
            //2、Add to Json::Value
            Json::Value root;
            for(int i = 0; i < arry.size(); ++i)
            {
                Json::Value item;
                item["pack_flag"] = arry[i].pack_flag;
                //size_t is not overloaded, so it needs to be converted
                item["fsize"] = (Json::Int64)arry[i].fsize;
                item["atime"] = (Json::Int64)arry[i].atime;
                item["mtime"] = (Json::Int64)arry[i].mtime;
                item["real_path"] = arry[i].real_path;
                item["pack_path"] = arry[i].pack_path;
                item["url"] = arry[i].url;
                root.append(item);//Add array elements
            }       
            //3、Serialize Json::Value
            std::string body;
			JsonUtil::Serialize(root, &body);
            //4、Write to file
            Util fu(_backup_file);
            fu.SetContent(body);
            return true;
        }

        //Initial load. Every time the system restarts, previous data must be read from the file.
        bool InitLoad()
        {
            //1、Read data from file
            Util fu(_backup_file);
            if(fu.Exists() == false) return true;
            std::string body;
            fu.GetContent(&body);
            //2、Deserialization
            Json::Value root;
            JsonUtil::Deserialize(body, &root);
            //3、Add the data in Json::Value obtained by deserialization to the table
            for(int i = 0; i < root.size(); ++i)
            {
                BackupInfo info;
                info.pack_flag = root[i]["pack_flag"].asBool();
                info.fsize = root[i]["fsize"].asInt64();
                info.atime = root[i]["atime"].asInt64();
                info.mtime = root[i]["mtime"].asInt64();
                info.pack_path = root[i]["pack_path"].asString();
                info.real_path = root[i]["real_path"].asString();
                info.url = root[i]["url"].asString();
                Insert(info);
            }
            return true;
        }
    };
}

#endif