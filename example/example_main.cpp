// clang-format off
#include "../include/jpwrap/jsonplaceholder/client.hpp"

#include <boost/asio/ssl/context.hpp>

#include <iostream>

int main() 
{
    try 
    {
        boost::asio::io_context ioc;
        boost::asio::ssl::context sslctx{boost::asio::ssl::context::tls_client};
        sslctx.set_default_verify_paths();
        sslctx.set_verify_mode(boost::asio::ssl::verify_peer);

        jpwrap::jp::Client api{ioc, sslctx};

        if (auto posts = api.get_posts()) std::cout << "jpwrap: got " << posts->size() << " posts\n";
        else std::cerr << posts.error() << "\n";

        if (auto one = api.get_post(1)) std::cout << "post #1 title: " << one->title << "\n";
        else std::cerr << one.error() << "\n";

        jpwrap::jp::Post p{.userId=1, .id=0, .title="hello", .body="world"};
        if (auto created = api.create_post(p)) std::cout << "created id: " << created->id << " (JSONPlaceholder fakes this)\n";
        else std::cerr << created.error() << "\n";

        if (auto ok = api.delete_post(1)) std::cout << "deleted? " << (*ok ? "yes" : "no") << "\n";
        else std::cerr << ok.error() << "\n";
        
        return 0;
    } 
    catch (std::exception const& e) 
    {
        std::cerr << "fatal: " << e.what() << "\n";
        return 1;
    }
}
