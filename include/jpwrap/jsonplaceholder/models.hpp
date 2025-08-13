// clang-format off
#ifndef JPWRAP_JSONPLACEHOLDER_MODELS_HPP
#define JPWRAP_JSONPLACEHOLDER_MODELS_HPP

#include <boost/json.hpp>

namespace jpwrap
{
namespace jp
{
namespace json = boost::json;

struct Post
{
    int32_t userId{};
    int32_t id{};
    std::string title;
    std::string body;
};

inline json::value to_json(Post const &p)
{
    return json::object
    {
        {"userId",  p.userId},
        {"id",      p.id},
        {"title",   p.title},
        {"body",    p.body}
    };
}

inline Post from_json(json::value const &v) 
{
    auto const &obj = v.as_object();
    Post p{};
    p.userId = static_cast<int32_t>(obj.at("userId").as_int64());
    if (auto idit = obj.if_contains("id")) p.id = static_cast<int32_t>(idit->as_int64());
    p.title = std::string(obj.at("title").as_string().c_str());
    p.body  = std::string(obj.at("body").as_string().c_str());
    return p;
}

inline std::vector<Post> posts_from_json_array(json::value const &v) 
{
    std::vector<Post> out;
    for (auto const &e : v.as_array()) out.push_back(from_json(e));
    return out;
}
} // namespace jp
} // namespace jpwrap

#endif // JPWRAP_JSONPLACEHOLDER_MODELS_HPP
