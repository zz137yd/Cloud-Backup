#ifndef __MY_CLOUD__
#define __MY_CLOUD__

#include "data.hpp"
#include "httplib.h"
//if compiled in a windows environment, add windows.h

namespace cloud
{
    #define SERVER_ADDR "106.75.12.79"
    #define SERVER_PORT 3389

    class Backup
    {
    private:
        std::string _back_dir;//Folder to monitor
        DataManager* _data;
    public:
        Backup(const std::string& back_dir, const std::string& back_file):_back_dir(back_dir)
        {
            _data = new DataManager(back_file);
        }

        std::string GetFileIdentifier(const std::string& filename)//Get the unique identifier of file
        {
            //a.txt-fsize-mtime
            Util fu(filename);
            std::stringstream ss;
            ss << fu.FileName() << "-" << fu.FileSize() << "-" << fu.LastMTime();
            return ss.str();
        } 

        //Traverse the entire file, obtain the identifier of each file, and upload it when it needs to be uploaded.
        bool IsNeedUpload(const std::string& filename)
        {
            //Upload conditions: New or modified
            //New: Check if there is any previous backup information
            //Modification: There is previous backup information, but the previous unique identifier is different from the current one
            std::string id;
            if(_data->GetOneByKey(filename, &id))
            {
                //Has previous information
                std::string new_id = GetFileIdentifier(filename);
                if(new_id == id) return false;
            }
            //If a file is large, copy it slowly
            //Then according to the above method, it will see different identifier every time it traverses
            //It will keep uploading backup requests, and there will be many invalid backup files
            //Then we can simply set it. Only when the file hasn't been modified within a period of time can it be judged whether to upload it
            //If it has been modified, it will not be uploaded
            Util fu(filename);
            if(time(NULL) - fu.LastMTime() < 3) return false;//Upload only if it has not been modified within 3 seconds
            return true;
        }

        bool Upload(const std::string& filename)//Upload file
        {
            //1、Get file data
            Util fu(filename);
            std::string body;
            fu.GetContent(&body);
            //2、Build http client to upload file data
            httplib::Client client(SERVER_ADDR, SERVER_PORT);
            httplib::MultipartFormData item;
            item.content = body;
            item.filename = fu.FileName();
            item.name = "file";//Field name, written according to the server
            item.content_type = "application/octet-stream";//Represent binary stream data
            httplib::MultipartFormDataItems items;
            items.push_back(item);
            auto res = client.Post("/upload", items);//Send Post request
            if(!res || res->status != 200)//Indicate upload failed
                return false;
            return true;
        }

        bool RunModule()
        {
            while(1)
            {
                //1、Traverse to obtain all files in the specified folder
                Util fu(_back_dir);
                std::vector<std::string> arry;
                fu.ScanDirectory(&arry);
                //2、Determine whether files need to be uploaded one by one
                for(auto& a : arry)
                {
                    if(!IsNeedUpload(a)) continue;
                    if (Upload(a))
                    {
                        _data->Insert(a, GetFileIdentifier(a));//Add new backup information
                        std::cout << a << " Successfully uploaded \n";
                    }
                }
                sleep(1);
            }
        }
    };
}

#endif