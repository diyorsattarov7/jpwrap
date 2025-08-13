// clang-format off
#ifndef JPWRAP_JSONPLACEHOLDER_CLIENT_HPP
#define JPWRAP_JSONPLACEHOLDER_CLIENT_HPP

#include "../transport/http_client.hpp"
#include "models.hpp"

#include <boost/beast/http.hpp>

namespace jpwrap 
{
namespace jp 
{
namespace http = boost::beast::http;

struct ClientConfig 
{
    std::string host = "jsonplaceholder.typicode.com";
    std::string port = "443"; 
    std::string base = "/"; 
};

class Client {
  public:
    Client(boost::asio::io_context &ioc, boost::asio::ssl::context &sslctx, ClientConfig cfg = {})
        : ioc_(ioc), sslctx_(sslctx), cfg_(std::move(cfg)), http_(ioc_, sslctx_) {}

    std::expected<std::vector<Post>, std::string>  get_posts();
    std::expected<Post, std::string>               get_post(int id);
    std::expected<Post, std::string>               create_post(Post p);
    std::expected<Post, std::string>               update_post(int id, Post p);
    std::expected<bool, std::string>               delete_post(int id);

  private:
    std::string url(std::string path) const { return cfg_.base + path; }

    boost::asio::io_context   &ioc_;
    boost::asio::ssl::context &sslctx_;
    ClientConfig               cfg_;
    transport::https_client    http_;
};

} // namespace jp
} // namespace jpwrap

#endif // JPWRAP_JSONPLACEHOLDER_CLIENT_HPP
