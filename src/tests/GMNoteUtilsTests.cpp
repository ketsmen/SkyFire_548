/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "GMNoteUtils.h"

#include <iostream>
#include <string>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool ExpectEqual(std::string const& actual, char const* expected, char const* message)
    {
        if (actual == expected)
            return true;

        std::cerr << message << ": expected '" << expected << "', got '" << actual << "'\n";
        return false;
    }
}

int main()
{
    bool passed = true;

    Skyfire::GMNoteValidation empty = Skyfire::ValidateGMNoteText("");
    passed &= Expect(!empty.IsValid, "empty note should be rejected");
    passed &= ExpectEqual(empty.Message, "Note text is required.", "empty note message failed");

    Skyfire::GMNoteValidation valid = Skyfire::ValidateGMNoteText(std::string(255, 'a'));
    passed &= Expect(valid.IsValid, "255 character note should be accepted");

    Skyfire::GMNoteValidation tooLong = Skyfire::ValidateGMNoteText(std::string(256, 'a'));
    passed &= Expect(!tooLong.IsValid, "256 character note should be rejected");
    passed &= ExpectEqual(tooLong.Message, "GM notes are limited to 255 characters.", "long note message failed");

    Skyfire::GMNoteLocation location;
    location.MapId = 1;
    location.ZoneId = 14;
    location.AreaId = 1637;
    location.X = 123.4567f;
    location.Y = -987.6543f;
    location.Z = 42.1234f;
    location.Orientation = 1.5707f;

    passed &= ExpectEqual(
        Skyfire::FormatGMNoteLocation(location),
        "map=1 zone=14 area=1637 x=123.46 y=-987.65 z=42.12 o=1.57",
        "location formatting failed"
    );

    return passed ? 0 : 1;
}
