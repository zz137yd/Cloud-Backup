#include "httplib.h"
#define SERVER_IP "106.75.12.79"
#define SERVER_PORT 3389

int main()
{
    httplib::Client client(SERVER_IP, SERVER_PORT);
    httplib::MultipartFormData item;
    item.name = "file";
    item.filename = "hellp.txt";
    item.content = "Hello Worle!";
    item.content_type = "text/plain";
    httplib::MultipartFormDataItems items; 
    items.push_back(item);

    auto res = client.Post("/multipart", items);
    std::cout << res->status << std::endl;
    std::cout << res->body << std::endl;
    return 0;
}