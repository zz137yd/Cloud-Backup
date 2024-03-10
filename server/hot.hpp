#ifndef __MY_HOT__
#define __MY_HOT__

#include "data.hpp"
#include <unistd.h>

extern cloud::DataManager* _data;

namespace cloud
{
    class HotManager
    {
    private:
        std::string _back_dir;//Backup file path
        std::string _pack_dir;//Compressed file path
        std::string _pack_suffix;//Compressed package suffix name
        int _hot_time;//Hotspot judgment time

        bool HotJudge(const std::string& filename)
        {
            Util fu(filename);
            time_t last_atime = fu.LastATime();
            time_t cur_time = time(NULL);
            return cur_time - last_atime > _hot_time;
        }
    public:
        HotManager()
        {
            Config* config = Config::GetInstance();
            _back_dir = config->GetBackDir();
            _pack_dir = config->GetPackDir();
            _pack_suffix = config->GetPackFileSuffix();
            _hot_time = config->GetHotTime();

            //Create a directory if there is no directory
            Util tmp1(_back_dir);
            Util tmp2(_pack_dir);
            tmp1.CreateDirectory();
            tmp2.CreateDirectory();
        }

        bool RunModule()//Run function
        {
            while (1)
            {
                //1、Traverse the backup file directory and obtain all file names
                Util fu(_back_dir);
                std::vector<std::string> arry;
                //Get the file name with relative path and convert it into a string and insert it into arry
                fu.ScanDirectory(&arry);
                //2、Traverse to determine whether the file is a non-hot file
                for (auto &a : arry)
                {
                    if (HotJudge(a) == false) continue;
                    //3、Get backup information for file
                    BackupInfo bi;
                    if (_data->GetOneByRealPath(a, &bi) == false)
                    {
                        //File exists but no backup information
                        bi.NewBackupInfo(a);//Set new backup information
                    }
                    //4、Compress non-hotspot file
                    Util tmp(a);
                    tmp.Compress(bi.pack_path);
                    //5、Delete source file and modify backup information
                    tmp.Remove();
                    bi.pack_flag = true;
                    _data->Update(bi);
                }
                usleep(1000);//Avoid looping through empty directory and consuming too much CPU resources
            }
            return true;
        }
    };
}

#endif