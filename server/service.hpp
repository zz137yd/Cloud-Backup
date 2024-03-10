#ifndef __MY_SERVICE__
#define __MY_SERVICE__

#include "data.hpp"
#include "httplib.h"

extern cloud::DataManager* _data;

namespace cloud
{
    class Service
    {
    private:
        int _server_port;
        std::string _server_ip;
        std::string _download_prefix;
        httplib::Server _server;
    private:
        //Static is used because if called within the class, the this pointer will be passed by default
        //And one more parameter needs to be passed in
        //If it is changed to static, there will be no this pointer
        static void Upload(const httplib::Request& req, httplib::Response& rsp)//Upload request handling function
        {
            //pos /upload  The text contains file data and other
            auto ret = req.has_file("file");//Determine whether there is an uploaded file area
            if(ret == false)
            {
                rsp.status = 400;
                return ;
            }
            const auto& file = req.get_file_value("file");
            std::string back_dir = Config::GetInstance()->GetBackDir();
            std::string realpath = back_dir + Util(file.filename).FileName();
            Util fu(realpath);
            fu.SetContent(file.content);//Data is written to the file
            BackupInfo info;
            info.NewBackupInfo(realpath);//Organize backup file information
            _data->Insert(info);//Add backed up file information to the data management module
            return ;
        }

        //Static need to call static
        static std::string TimetoStr(time_t t)//Convert timestamp to string
        {
            std::string tmp = std::ctime(&t);
            return tmp;
        }

        static void ListShow(const httplib::Request& req, httplib::Response& rsp)//Display page fetch request
        {
            //1、Get all file backup information
            std::vector<BackupInfo> arry;
            _data->GetAll(&arry);
            //2、Organize html file data based on backup information
            std::stringstream ss;
            ss << "<html><head><title>Download</title></head>";
            ss << "<body><h1>Download</h1><table>";
            for (auto &a : arry)
            {
                ss << "<tr>";
                std::string filename = Util(a.real_path).FileName();
                ss << "<td><a href='" << a.url << "'>" << filename << "</a></td>";
                ss << "<td align='right'>" << TimetoStr(a.mtime) << "</td>";
                ss << "<td align='right'>" << a.fsize / 1024 << "k</td>";
                ss << "</tr>";
            }
            ss << "</table></body></html>";
            rsp.body = ss.str();
            rsp.set_header("Content-Type", "text/html");
            rsp.status = 200;
            return ;
        }

        static std::string GetETag(const BackupInfo& info)
        {
            //etag: filename-fsize-mtime
            Util fu(info.real_path);
            std::string etag = fu.FileName();
            etag += "-";
            etag += std::to_string(info.fsize);
            etag += "-";
            etag += std::to_string(info.mtime);
            return etag;
        }

        static void Download(const httplib::Request& req, httplib::Response& rsp)//File download processing function
        {
            //1、Get the req.path requested by the client
            //2、Obtain file backup information based on resource path
            BackupInfo info;
            _data->GetOneByUrl(req.path, &info);
            //3、Determine whether the file is compressed, if so, decompress it first
            if(info.pack_flag == true)
            {
                Util fu(info.pack_path);
                fu.UnCompress(info.real_path);//Uncompress the file to the backup directory
                //4、Delete the compressed package and modify the backup (it is no longer compressed at this time)
                fu.Remove();
                info.pack_flag = false;
                _data->Update(info);
            }

            //5、Handling breakpoint resumption
            bool retrans = false;
            std::string old_etag;
            //If there is no such field, it means that there is no need to resume the download, just download it normally
            //If there is but its value is inconsistent with the etag of the current file, then we have to download it all again
            if(req.has_header("If-Range"))
            {
                old_etag = req.get_header_value("If-Range");
                if(old_etag == GetETag(info)) retrans = true;
            }
            //6、Read the file data and put it into rsp.body
            //7、Set response header fields: ETag, Accept-Ranages: bytes
            Util fu(info.real_path);
            if (!retrans)
            {
                fu.GetContent(&rsp.body);
                rsp.set_header("Accept-Ranges", "bytes");
                rsp.set_header("ETag", GetETag(info));
                rsp.set_header("Content-Type", "application/octet-stream");//Indicates that the response body is a binary data stream
                rsp.status = 200;
            }
            else
            {
                //In fact, the httplib library implements breakpoint resume transmission
                //The user only needs to read the data into the body
                //The library will retrieve the specified range of data from the body according to the request range, and respond
                //std::string range = req.get_header_val("Range"); Range: bytes = start-end, Parse this to get the size of the head and tail and get the range.
                fu.GetContent(&rsp.body);
                rsp.set_header("Accept-Ranges", "bytes");
                rsp.set_header("ETag", GetETag(info));
                rsp.set_header("Content-Type", "application/octet-stream");
                //After finishing, users need to set this field, but the library has already implemented it, so it is not needed.
                //rsp.set_header("Content-Range", "bytes start-end/fsize");
                rsp.status = 206;//The response to the interval request is 206
            }
        }
    public:
        Service()
        {
            Config* config = Config::GetInstance();
            _server_port = config->GetServerPort();
            _server_ip = config->GetServerIp();
            _download_prefix = config->GetDownloadPrefix();
        }

        bool RunModule()
        {
            _server.Post("/upload", Upload);
            _server.Get("/listshow", ListShow);
            _server.Get("/", ListShow);
            std::string download_url = _download_prefix + "(.*)";
            _server.Get(download_url, Download);
            _server.listen(_server_ip.c_str(), _server_port);
            return true;
        }
    };
}

#endif