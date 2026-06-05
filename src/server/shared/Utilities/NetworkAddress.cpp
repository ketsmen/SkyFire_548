/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "NetworkAddress.h"

#if PLATFORM == PLATFORM_WINDOWS
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include <boost/asio/ip/address_v4.hpp>
#include <boost/system/error_code.hpp>

namespace
{
    bool ParseIPv4Address(std::string const& host, boost::asio::ip::address_v4& address)
    {
        boost::system::error_code error;
        address = boost::asio::ip::make_address_v4(host, error);
        if (error)
            return false;

        if (address.to_string() != host)
            return false;

        return address.to_uint() != boost::asio::ip::address_v4::uint_type(-1);
    }
}

namespace Skyfire::Net
{
    Address::Address() : _host("0.0.0.0"), _port(0) { }

    Address::Address(std::string const& host, uint16 port) : _host(host), _port(port) { }

    bool IsIPv4Address(std::string const& host)
    {
        boost::asio::ip::address_v4 address;
        return ParseIPv4Address(host, address);
    }

    uint32 ToIPv4NetworkOrder(std::string const& host)
    {
        boost::asio::ip::address_v4 address;
        if (!ParseIPv4Address(host, address))
            return 0;

        return htonl(static_cast<uint32>(address.to_uint()));
    }

    bool Address::IsLoopback() const
    {
        uint32 const ip = ntohl(ToIPv4NetworkOrder());
        return (ip & 0xFF000000) == 0x7F000000;
    }

    uint32 Address::ToIPv4NetworkOrder() const
    {
        return Skyfire::Net::ToIPv4NetworkOrder(_host);
    }
}
