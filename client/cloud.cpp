#include "cloud.hpp"

#define BACKUP_FILE "./backup.dat"
#define BACKUP_DIR "./backup/ "

int main()
{
    cloud::Backup backup(BACKUP_DIR, BACKUP_FILE);
    backup.RunModule();
    /*cloud::Util fu("./");
    std::vector<std::string> arry;
    fu.ScanDirectory(&arry);
    cloud::DataManager data(BACKUP_FILE);
    for(auto& a : arry)
    {
        data.Insert(a, "hfgdsf");
    }
    std::string str;
    data.GetOneByKey(".\\cloud.cpp", &str);
    std::cout << str << std::endl;*/
    return 0;
}