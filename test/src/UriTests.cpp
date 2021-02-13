/**
 * @file UriTests.cpp
 *
 * This module contains the unit tests of the Uri::Uri class.
 *
 * © 2018 by Richard Walters
 */

#include <gtest/gtest.h>
#include <Uri/Uri.hpp>

TEST(UriTests, ParseFromString) {
    UriParser::Uri uri;
    ASSERT_TRUE(uri.ParserFromSring("http://www.host.com/"));
    ASSERT_EQ("http",uri.GetScheme());
    ASSERT_EQ("www.host.com", uri.GetAuthority());

    //  ASSERT_EQ("host.com",uri.GetAuthority());
}

// test for scheme

TEST(UriTests, ParseFromStringNoScheme) {
    UriParser::Uri uri;
    ASSERT_TRUE(uri.ParserFromSring("host.com"));
    ASSERT_EQ("",uri.GetScheme());
    //  ASSERT_EQ("host.com",uri.GetAuthority());
}

TEST(UriTests, ParseFromStringUrnDefaultPathDelimiter) {
    UriParser::Uri uri;
    ASSERT_TRUE(uri.ParserFromSring("urn:book:fantasy:Hobbit"));
    ASSERT_EQ("urn", uri.GetScheme());
}
 


TEST(UriTests, ParseFromStrinSchemeIllegalCharacters) {
    UriParser::Uri uri;
    const std::vector<std::string> testVectors{
        {"://www.example.com/"},
        {"0://www.example.com/"},
        {"+://www.example.com/"},
        {"@://www.example.com/"},
        {".://www.example.com/"},
        {"h@://www.example.com/"},
    };
    for (const auto& testVector : testVectors) {
        ASSERT_FALSE(uri.ParserFromSring(testVector));
    }

}

TEST(UriTests, ParseFromStringSchemeBarelyLegal) {
      struct TestVector {
        std::string uriString;
        std::string scheme;
    };
    UriParser::Uri uri;
    const std::vector<TestVector> testVectors{
        {"h://www.example.com/", "h"},
        {"x+://www.example.com/", "x+"},
        {"y-://www.example.com/", "y-"},
        {"z.://www.example.com/", "z."},
        {"aa://www.example.com/", "aa"},
        {"a0://www.example.com/", "a0"},
        
    };
    for (const auto& testVector : testVectors) {
         ASSERT_TRUE(uri.ParserFromSring(testVector.uriString));
         ASSERT_EQ(testVector.scheme, uri.GetScheme());
    }
}



TEST(UriTests, ParseFromStringSchemeMixedCase) {
    UriParser::Uri uri;
    const std::vector<std::string> testVectors{
        {"http://www.example.com/"},
        {"hTtp://www.example.com/"},
        {"HTTP://www.example.com/"},
        {"Http://www.example.com/"},
        {"HttP://www.example.com/"},
    };
    size_t index = 0;
    for (const auto& testVector : testVectors) {
        std::cout<<testVector<<std::endl;
        ASSERT_TRUE(uri.ParserFromSring(testVector));
        ASSERT_EQ("http", uri.GetScheme());
    }

}

TEST(UriTests, ParseFromStringDontMisinterpretColonInOtherPlacesAsSchemeDelimiter) {
    UriParser::Uri uri;
    const std::vector<std::string> testVectors{
        {"//foo:bar@www.example.com/"},
        {"//www.example.com/a:b"},
        {"//www.example.com/foo?a:b"},
        {"//www.example.com/foo#a:b"},
        {"//[v7.:]/"},
        {"/:/foo"},
    };
    size_t index = 0;
    for (const auto& testVector : testVectors) {
        std::cout<<testVector<<std::endl;
        ASSERT_FALSE(uri.ParserFromSring(testVector));
        ASSERT_TRUE(uri.GetScheme().empty());
    }
}


