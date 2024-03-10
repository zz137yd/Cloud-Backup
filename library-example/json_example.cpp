#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>

//序列化
/*int main()
{
    const char* name = "小明";
    int age = 18;
    float score[] = {77.5, 88, 93,6};

    Json::Value root;
    //其实实现方法就是重载了[]
    root["姓名"] = name;
    root["年龄"] = age;
    root["成绩"].append(score[0]);
    root["成绩"].append(score[1]);
    root["成绩"].append(score[2]);
    
    Json::StreamWriterBuilder swb;
    std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
    std::stringstream ss;
    sw->write(root, &ss);
    std::cout << ss.str() << std::endl;
    return 0;
}*/

//反序列化
int main()
{
    std::string str = R"({"姓名":"小黑", "年龄":19, "成绩":[58.5, 66, 35.5]})";
    Json::Value root;
    Json::CharReaderBuilder crb;
    std::unique_ptr<Json::CharReader> cr(crb.newCharReader());
    std::string err;
    bool ret = cr->parse(str.c_str(), std.c_str() + str.size(), &root, &err);
    if(ret == false)
    {
        std::cout << "parse error: " << err << std::endl;
	return -1;
    }
    std::cout << root["姓名"].asString() << std::endl;
    std::cout << root["年龄"].asInt() << std::endl;
    int sz = root["成绩"].size();
    for(int i = 0; i < sz; i++)
        std::cout << root["成绩"][i] << std::endl;
    return 0;
}
