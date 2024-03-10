#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>

//Serialization
/*int main()
{
    const char* name = "Paul";
    int age = 18;
    float score[] = {77.5, 88, 93,6};

    Json::Value root;
    //In fact, the implementation method is to overload the []
    root["Name"] = name;
    root["Age"] = age;
    root["Score"].append(score[0]);
    root["Score"].append(score[1]);
    root["Score"].append(score[2]);
    
    Json::StreamWriterBuilder swb;
    std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
    std::stringstream ss;
    sw->write(root, &ss);
    std::cout << ss.str() << std::endl;
    return 0;
}*/

//Deserialization
int main()
{
    std::string str = R"({"Name":"Paul", "Age":19, "Score":[58.5, 66, 35.5]})";
    Json::Value root;
    Json::CharReaderBuilder crb;
    std::unique_ptr<Json::CharReader> cr(crb.newCharReader());
    std::string err;
    bool ret = cr->parse(str.c_str(), str.c_str() + str.size(), &root, &err);
    if (ret == false)
    {
        std::cout << "parse error: " << err << std::endl;
        return -1;
    }
    std::cout << root["Name"].asString() << std::endl;
    std::cout << root["Age"].asInt() << std::endl;
    int sz = root["Score"].size();
    for(int i = 0; i < sz; i++)
    {
        std::cout << root["Score"][i] << std::endl;
    }
    return 0;
}
