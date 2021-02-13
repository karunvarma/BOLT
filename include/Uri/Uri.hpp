#ifndef URI_HPP
#define URI_HPP


#include <iostream>
#include <memory>
#include <algorithm>

namespace UriParser {
    class Uri
    {
        public:

        // Rule of 5
        ~Uri();
        Uri();
        Uri(const Uri& other);
        Uri(Uri&&);
        Uri& operator=(const Uri& other);
        Uri& operator=(Uri&&);


        bool ParserFromSring(const std::string& uriString);
        std::string GetScheme() const;
        std::string GetAuthority() const;
        private:

        enum class error;

        error ec;
        // forward decleration
        struct Impl;

        //  PIMPL IDIOM
        std::unique_ptr<Impl> impl_;

    };

}

#endif /* URI_HPP */
