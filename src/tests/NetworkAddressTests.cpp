/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "NetworkAddress.h"

#include <iostream>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (condition)
            return true;

        std::cerr << message << '\n';
        return false;
    }
}

int main()
{
    bool passed = true;

    passed &= Expect(Skyfire::Net::IsIPv4Address("127.0.0.1"), "127.0.0.1 should be accepted as IPv4");
    passed &= Expect(Skyfire::Net::IsIPv4Address("0.0.0.0"), "0.0.0.0 should be accepted as IPv4");
    passed &= Expect(!Skyfire::Net::IsIPv4Address("12.23"), "shorthand IPv4 forms should be rejected");
    passed &= Expect(!Skyfire::Net::IsIPv4Address("121234"), "numeric-only IPv4 forms should be rejected");
    passed &= Expect(!Skyfire::Net::IsIPv4Address("0xABCD"), "hex IPv4 forms should be rejected");
    passed &= Expect(!Skyfire::Net::IsIPv4Address("2001:db8::1"), "IPv6 strings should stay unsupported in IPv4 helpers");
    passed &= Expect(!Skyfire::Net::IsIPv4Address("255.255.255.255"), "255.255.255.255 should preserve legacy INADDR_NONE rejection");

    passed &= Expect(Skyfire::Net::ToIPv4NetworkOrder("1.2.3.4") != 0, "1.2.3.4 should convert to a nonzero address");
    passed &= Expect(Skyfire::Net::ToIPv4NetworkOrder("12.23") == 0, "invalid shorthand address should convert to zero");

    passed &= Expect(Skyfire::Net::Address("127.42.0.1").IsLoopback(), "127.42.0.1 should be recognized as loopback");
    passed &= Expect(!Skyfire::Net::Address("192.168.1.10").IsLoopback(), "192.168.1.10 should not be loopback");

    return passed ? 0 : 1;
}
