// clang-format off

#include "../../../include/jpwrap/jsonplaceholder/client.hpp"

#include <boost/json.hpp>

namespace jpwrap 
{
namespace jp 
{
namespace json = boost::json;

std::expected<std::vector<Post>, std::string> Client::get_posts() 
{
    auto res = http_.request(cfg_.host, cfg_.port, http::verb::get, url("posts"));
    if (!res) return std::unexpected{res.error()};
    if (res->status != 200) return std::unexpected{"GET /posts status " + std::to_string(res->status)};

    auto parsed = json::parse(res->body);
    return posts_from_json_array(parsed);
}

std::expected<Post, std::string> Client::get_post(int id) 
{
    auto res = http_.request(cfg_.host, cfg_.port, http::verb::get, url("posts/" + std::to_string(id)));
    if (!res) return std::unexpected{res.error()};
    if (res->status != 200) return std::unexpected{"GET /posts/{id} status " + std::to_string(res->status)};

    auto parsed = json::parse(res->body);
    return from_json(parsed);
}

std::expected<Post, std::string> Client::create_post(Post p) 
{
    auto body = json::serialize(to_json(p));
    auto res = http_.request(cfg_.host, cfg_.port, http::verb::post, url("posts"), std::move(body));
    if (!res) return std::unexpected{res.error()};
    if (res->status != 201) return std::unexpected{"POST /posts status " + std::to_string(res->status)};

    auto parsed = json::parse(res->body);
    return from_json(parsed);
}

std::expected<Post, std::string> Client::update_post(int id, Post p) 
{
    p.id = id;
    auto body = json::serialize(to_json(p));
    auto res = http_.request(cfg_.host, cfg_.port, http::verb::put, url("posts/" + std::to_string(id)), std::move(body));
    if (!res) return std::unexpected{res.error()};
    if (res->status != 200) return std::unexpected{"PUT /posts/{id} status " + std::to_string(res->status)};

    auto parsed = json::parse(res->body);
    return from_json(parsed);
}

std::expected<bool, std::string> Client::delete_post(int id) 
{
    auto res = http_.request(cfg_.host, cfg_.port, http::verb::delete_, url("posts/" + std::to_string(id)));
    if (!res) return std::unexpected{res.error()};
    if (res->status != 200 && res->status != 204) return std::unexpected{"DELETE /posts/{id} status " + std::to_string(res->status)};
    return true;
}

} // namespace jp
} // namespace jpwrap
