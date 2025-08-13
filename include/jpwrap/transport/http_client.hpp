// clang-format off
#ifndef JPWRAP_TRANSPORT_HTTP_CLIENT_HPP
#define JPWRAP_TRANSPORT_HTTP_CLIENT_HPP

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

#include <expected>

namespace jpwrap
{
namespace transport
{

namespace net   = boost::asio;
namespace beast = boost::beast;
namespace http  = beast::http;
namespace ssl   = net::ssl;

struct http_response
{
    unsigned status = 0;
    std::string body;
    http::fields headers;
};

class https_client
{
public:
    explicit https_client(net::io_context &ioc, ssl::context &sslctx) : ioc_(ioc), sslctx_(sslctx) {}

    std::expected<http_response, std::string> 
    request(std::string_view host, 
            std::string_view port, 
            http::verb method,
            std::string target,
            std::string body = {},
            std::chrono::milliseconds timeout = std::chrono::seconds(10))
    {
        try
        {
            net::ip::tcp::resolver resolver{ioc_};
            auto const results = resolver.resolve(host, port);

            beast::ssl_stream<beast::tcp_stream> stream{ioc_, sslctx_};
            beast::get_lowest_layer(stream).expires_after(timeout);
            beast::get_lowest_layer(stream).connect(results);

            if (!SSL_set_tlsext_host_name(stream.native_handle(), std::string(host).c_str()))
            {
                beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
                return std::unexpected{"SNI setup failed: " + ec.message()};
            }

            stream.handshake(ssl::stream_base::client);

            http::request<http::string_body> req{method, target, 11};
            req.set(http::field::host, host);
            req.set(http::field::user_agent, "jpwrap/1.0");
            if (!body.empty())
            {
                req.set(http::field::content_type, "application/json");
                req.body() = std::move(body);
                req.prepare_payload();
            }

            http::write(stream, req);

            beast::flat_buffer buffer;
            http::response<http::string_body> res;
            http::read(stream, buffer, res);

            beast::error_code ec;
            stream.shutdown(ec);

            return http_response
            {
               .status  = static_cast<unsigned>(res.result_int()),
               .body    = std::move(res.body()),
               .headers = res.base()
            };
        }
        catch(std::exception const &e)
        {
            return std::unexpected{std::string{"HTTPS request failed: "} + e.what()};
        }
    }

private:
    net::io_context &ioc_;
    ssl::context &sslctx_;
};


} // namespace transport
} // namespace jpwrap

#endif // JPWRAP_TRANSPORT_HTTP_CLIENT_HPP
