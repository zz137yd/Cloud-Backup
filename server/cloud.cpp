#include "hot.hpp"
#include "service.hpp"
#include <thread>

void FileUtilTest(const std::string& filename)
{
    cloud::Util fu(filename);
    fu.CreateDirectory();
    std::vector<std::string> arry;
    fu.ScanDirectory(&arry);
    for(auto& a: arry)
    {
        std::cout << a << std::endl;
    }
    /*std::string packname = filename + ".lz";
    fu.Compress(packname);
    cloud::Util pfu(packname);
    pfu.UnCompress("./hello.txt");*/

    /*std::string body;
    fu.GetContent(&body);
    cloud::Util nfu("./hello.txt");
    nfu.SetContent(body);*/

    /*std::cout << fu.FileSize() << std::endl;
    std::cout << fu.LastMTime() << std::endl;
    std::cout << fu.LastATime() << std::endl;
    std::cout << fu.FileName() << std::endl;*/
    return ;
}

void JsonUtilTest()
{
    const char* name = "Paul";
    int age = 19;
    float score[] = {85, 88.5, 99};
    Json::Value root;
    root["Name"] = name;
    root["Age"] = age;
    root["Score"].append(score[0]);
    root["Score"].append(score[1]);
    root["Score"].append(score[2]);
    std::string json_str;
    cloud::JsonUtil::Serialize(root, &json_str);
    std::cout << json_str << std::endl;

    Json::Value val;
    cloud::JsonUtil::Deserialize(json_str, &val);
    std::cout << val["Name"].asString() << std::endl;
    std::cout << val["Age"].asInt() << std::endl;
    for(int i = 0; i < val["Score"].size(); i++)
    {
        std::cout << val["Score"][i].asFloat() << std::endl;
    }
}

void ConfigTest()
{
    cloud::Config* config = cloud::Config::GetInstance();
    std::cout << config->GetHotTime() << std::endl;
    std::cout << config->GetServerPort() << std::endl;
    std::cout << config->GetServerIp() << std::endl;
    std::cout << config->GetDownloadPrefix() << std::endl;
    std::cout << config->GetPackFileSuffix() << std::endl;
    std::cout << config->GetBackDir() << std::endl;
    std::cout << config->GetPackDir() << std::endl;
    std::cout << config->GetBackupFile() << std::endl;
}

void Print(cloud::BackupInfo& h)
{
    std::cout << h.pack_flag << std::endl;
    std::cout << h.fsize << std::endl;
    std::cout << h.mtime << std::endl;
    std::cout << h.atime << std::endl;
    std::cout << h.real_path << std::endl;
    std::cout << h.pack_path << std::endl;
    std::cout << h.url << std::endl;
}

void DataTest(const std::string& filename)
{
    cloud::DataManager data;
    std::vector<cloud::BackupInfo> arry;
    data.GetAll(&arry);
    for (auto& a : arry)
    {
        Print(a);
    }
    /*std::cout << "----------insert----------\n";
    cloud::BackupInfo info;
    info.NewBackupInfo(filename);
    cloud::DataManager data;
    data.Insert(info);

    cloud::BackupInfo tmp;
    data.GetOneByUrl("/download/bundle.h", &tmp);
    Print(tmp);

    std::cout << "----------update and getall----------\n";
    info.pack_flag = true;
    data.Update(info);
    std::vector<cloud::BackupInfo> arry;
    data.GetAll(&arry);
    for (auto& a : arry)
    {
        Print(a);
    }

    std::cout << "----------realpath----------\n";
    data.GetOneByRealPath(filename, &tmp);
    Print(tmp);*/
}

cloud::DataManager* _data;

void HotTest()
{
    cloud::HotManager hot;
    hot.RunModule();
}

void ServiceTest()
{
    cloud::Service srv;
    srv.RunModule();
}

int main(int argc, char* argv[])
{
    _data = new cloud::DataManager();
    //FileUtilTest(argv[1]);
    //JsonUtilTest();
    //ConfigTest();
    //DataTest(argv[1]);
    //HotTest();
    //ServiceTest();
    std::thread thread_hot_manager(HotTest);
    std::thread thread_service(ServiceTest);
    thread_hot_manager.join();
    thread_service.join();
    return 0;
}
