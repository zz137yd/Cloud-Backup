#ifndef __MY_DATA__
#define __MY_DATA__

#include "util.hpp"
#include <unordered_map>
#include <sstream>

namespace cloud
{
    class DataManager
    {
    private:
        std::string _backup_file;//Persistent storage file for backup information
        std::unordered_map<std::string, std::string> _table;
    public:
        DataManager(const std::string& backup_file):_backup_file(backup_file)
        {
            InitLoad();
        }
 
        bool Storage()
        {
            //1、Get all backup information
            std::stringstream ss;
            auto it = _table.begin();
            for(; it != _table.end(); ++it)
            {
                //2、Organize information into a specified persistence format
                ss << it->first << " " << it->second << "\n";
            }
            //3、Persistent storage
            Util fu(_backup_file);
            fu.SetContent(ss.str());
            return true;
        }

        int Split(const std::string& str, const std::string& sep, std::vector<std::string>* arry)
        {
            int count = 0;
            size_t pos = 0, idx = 0;
            while(1)
            {
                pos = str.find(sep, idx);
                if(pos == std::string::npos) break;
                //The starting position is the character we are looking for
                if(pos == idx)
                {
                    idx = pos + sep.size();
                    continue;
                }
                std::string tmp = str.substr(idx, pos - idx);
                arry->push_back(tmp);
                idx = pos + sep.size();
                count++;
            }
            if(idx < str.size())
            {
                arry->push_back(str.substr(idx));//Add the rest in.
                count++;
            }
            return count;
        }

        bool InitLoad()
        {
            //Format: a.txt-fsize-mtime
            //1、Read all data from file
            Util fu(_backup_file);
            std::string body;
            fu.GetContent(&body);
            //2、Parse data according to persistence format and add to table
            //First split each line, and then split the string in each line
            std::vector<std::string> arry;
            Split(body, "\n", &arry);
            for(auto& a : arry)
            {
                std::vector<std::string> tmp;
                Split(a, " ", &tmp);
                if(tmp.size() != 2) continue;//File name and file attributes, two items in total
                _table[tmp[0]] = tmp[1];//Find file attributes by file name
            }
            return true;
        }

        bool Insert(const std::string& key, const std::string& val)
        {
            _table[key] = val;
            Storage();
            return true;
        }

        bool Update(const std::string& key, const std::string& val)
        {
            _table[key] = val;
            Storage();
            return true;
        }

        bool GetOneByKey(const std::string& key, std::string* val)
        {
            auto it = _table.find(key);
            if(it == _table.end()) return false;
            *val = it->second;
            return true;
        }
    };
}

#endif