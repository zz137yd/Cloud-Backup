#include "httplib.h"

void Hello(const httplib::Request& req, httplib::Response& rsp)
{
    rsp.set_content("Hello World!", "text/plain");
    rsp.status = 200;
}

void Numbers(const httplib::Request& req, httplib::Response& rsp)
{
    //0 saves the entire path, and subsequent subscripts save the captured data
    auto num = req.matches[1];
    rsp.set_content(num, "text/plain");
    rsp.status = 200;

}

void Multipart(const httplib::Request& req, httplib::Response& rsp)
{
    auto ret = req.has_file("file");
    if (ret == false)
    {
        std::cout << "not file upload\n";
        rsp.status = 400;
        return;
    }
    const auto& file = req.get_file_value("file");
    rsp.body.clear();
    rsp.body = file.filename;//File name
    rsp.body += "\n";
    rsp.body += file.content;//File content
    rsp.set_header("Content-Type", "text/plain");
    rsp.status = 200;
    return;
}

int main()
{
    httplib::Server server;
    server.Get("/hi", Hello);
    server.Get(R"(/number/(\d+))", Numbers);
    server.Post("/multipart", Multipart);
    server.listen("0.0.0.0", 3389);
    return 0;
}
