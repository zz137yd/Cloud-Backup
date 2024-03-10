#include "httplib.h"

void Hello(const httplib::Request& req, httplib::Response& rsp)
{
    rsp.set_content("Hello World!", "text/plain");
    rsp.status = 200;
}

void Numbers(const httplib::Request& req, httplib::Response& rsp)
{
    //0保存的是整体path, 往后下标中保存的是捕捉的数据
    auto num = req.matcher[1];
    rsp.set_content(num, "text/plain");
    rsp.status = 200;

}

void Multipart(const httplib::Request& req, httplib::Response& rsp)
{
    auto ret = req.has_file("file");
    if(ret == false)
    {
        std::cout << "not file upload\n";
	rsp.status = 400;
	return;
    }
    const auto& file = req.get_file_value("file");
    rsp.body.clear();
    rsp.body = file.filename;//文件名称
    rsp.body += "\n";
    rsp.body += file.content;//文件内容
    rsp.set_header("Content-Type", "text/plain");
    rsp.status = 200;
    return;
}

int main()
{
    httplib::Server server;
    server.Get("/hi", Hello);
    server.Get(R"(/number/(\d+))", Numbers);
    server.POST("/multipart", Multipart);
    server.listen("0.0.0.0", 9090);
    return 0;
}
