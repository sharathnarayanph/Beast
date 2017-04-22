//
// Copyright (c) 2013-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BEAST_HTTP_BASIC_DYNABUF_BODY_HPP
#define BEAST_HTTP_BASIC_DYNABUF_BODY_HPP

#include <beast/config.hpp>
#include <beast/core/error.hpp>
#include <beast/http/message.hpp>
#include <beast/core/detail/type_traits.hpp>
#include <boost/asio/buffer.hpp>

namespace beast {
namespace http {

/** A message body represented by a @b `DynamicBuffer`

    Meets the requirements of @b `Body`.
*/
template<class DynamicBuffer>
struct basic_dynabuf_body
{
    /// The type of the `message::body` member
    using value_type = DynamicBuffer;

#if BEAST_DOXYGEN
private:
#endif

    class reader
    {
        value_type& sb_;

    public:
        template<bool isRequest, class Fields>
        explicit
        reader(message<isRequest,
                basic_dynabuf_body, Fields>& m) noexcept
            : sb_(m.body)
        {
        }

        void
        init(error_code&) noexcept
        {
        }

        void
        write(void const* data,
            std::size_t size, error_code&) noexcept
        {
            using boost::asio::buffer;
            using boost::asio::buffer_copy;
            sb_.commit(buffer_copy(
                sb_.prepare(size), buffer(data, size)));
        }
    };

    class writer
    {
        DynamicBuffer const& body_;

    public:
        template<bool isRequest, class Fields>
        explicit
        writer(message<
                isRequest, basic_dynabuf_body, Fields> const& m) noexcept
            : body_(m.body)
        {
        }

        void
        init(error_code& ec) noexcept
        {
            beast::detail::ignore_unused(ec);
        }

        std::uint64_t
        content_length() const noexcept
        {
            return body_.size();
        }

        template<class WriteFunction>
        bool
        write(error_code&, WriteFunction&& wf) noexcept
        {
            wf(body_.data());
            return true;
        }
    };
};

} // http
} // beast

#endif
