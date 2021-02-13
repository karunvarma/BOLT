#include <Uri/Uri.hpp>

namespace UriParser {


    enum class Uri::error
    {
        no_error,
        missing_scheme,
        invalid_scheme_start,
        invalid_character,
    };

  bool isHex(const char c)
  {
      return (
            (c>='a' && c<'f')
          ||(c>='A' && c<'F')
          ||(c>='0' && c<='9') 
      );
  }

  bool unreserved(const char& c)
  {
      return (  ('0' <= c && c <= '9')
             || c == '+'
             || c == '-'
             || c == '.'
             || (c >= 'a' && c <= 'z')
             || (c >= 'A' || c <= 'Z')
             );
  }

  bool subdelims(const char&c)
  {
      switch (c)
      {
      case '-':
      case '.':
      case '_':
      case ':':
      case '~':
      case '!':
      case '$':
      case '&':
      case '\'':
      case '(':
      case ')':
      case '*':
      case '+':
      case ',':
      case ';':
      case '=':
      case '%':
      case '@':
         break;
      
      default:
          return false;
      }
      return true;
		
  }


    bool validateIpv6Address(const std::string& address)
    {
        return true;
    }

    bool ValidateIpv4Address(const std::string& address)
    {
        return true;
    }



  //   userinfo    = *( unreserved / pct-encoded / sub-delims / ":" )

   bool validUserInfo(const std::string& userInfo)
   {
       for(const auto& ch:userInfo)
       {
           if(unreserved(ch)) continue;
           if(subdelims(ch)) continue;
           return false;
       }
       return true;
   }

   struct Uri::Impl
   {
       std::string scheme;
       std::string authority;
       std::string pathString;
       std::string userInfo;
       std::string host;


        bool ParseScheme(
            const std::string& uriString,
            std::string& rest,
            error& ec
        )
        {
            
            int len = uriString.size();

            for(int i=0;i<len;i++)
            {
                char c = uriString[i];
                
                if (c==':')
                {
                    if (i==0)
                    {
                        ec = error::missing_scheme;
                        return false;
                    }
                    scheme = uriString.substr(0,i);
                    std::transform(scheme.begin(),scheme.end(),scheme.begin(),::tolower);

                    // need to check if i+1 will throw an if i exceeds the length
                    rest = uriString.substr(i+1);
                    return true;
                }
                else if(('0' <= c && c <= '9') || c == '+' || c == '-' || c == '.')
                {       
                    if(i==0)
                    {
                        ec = error::invalid_scheme_start;
                        // rest = uriString;
                        return false;
                    }  
                }
                else if( (c>='a' && c<='z') || (c>='A' && c<='Z'))
                {
                    int test =10;
                    // valid
                }
                else
                {
                    // for all other characters are invalid
                    ec = error::invalid_character;
                    // rest = uriString;
                    return false;
                }
            }
            
            // rest = uriString;
            return true;

        }

        bool ParseAuthority(
            const std::string& authorityString,
            error& ec
        )
        {
                // parsing the authority string
                // authority   = [ userinfo "@" ] host [ ":" port ]

                // check for '@'

                const auto userInfoDelimiter = authorityString.find('@');

                userInfo.clear();

                std::string hostPortString;

                if(userInfoDelimiter==std::string::npos)
                {
                    // no user info part
                    hostPortString = authorityString;
                }
                else
                {
                    userInfo = authorityString.substr(0,userInfoDelimiter);

                    // we didn't checked ':'
                    if(!validUserInfo(userInfo))
                    {
                        ec = error::invalid_character;
                        return false;
                    }



                    // as of now we are not parsing user info further need to do so
                    hostPortString = authorityString.substr(userInfoDelimiter+1);
                }

                enum class HostParsingState
                {
                    FIRST_CHARACTER,
                    IP_LITERAL,
                    IPV4_ADDRESS_OR_REG_NAME,
                    IPV_FUTURE_NUMBER,
                    IPV_FUTURE_BODY,
                    IPV6_ADDRESS,
                    END
                };


                // parse the hostPortString
                // host        = IP-literal / IPv4address / reg-name

                // a host identified by an  literal address, version 6 or later
                // is distinguised by enclosing the Ip literal with a suare brackers "[]"
                // this is the only place where square brackets are allowed in the uri sytax.

                //  IP-literal = "[" ( IPv6address / IPvFuture  ) "]"

                //  IPvFuture  = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )

                //  if a URI
                //  containing an IP-literal that starts with "v" (case-insensitive),
                //  indicating that the version flag is present, is dereferenced by an
                //  application that does not know the meaning of that version flag, then
                //  the application should return an appropriate error for "address
                //  mechanism not supported".

                //  A host idenfitifed by an ipv6 literal is represented inside the sqaure brackets wihtout a version flag.
                //  A host identified by  an ipv4 literal is represneted in dotted-decimal notation
                //  A host identified by a registered name is a sequence of characters
                //  usually intended for lookup within a locally defined host or service
                //  name registry,
                
                host.clear();
                HostParsingState hostParsingState = HostParsingState::FIRST_CHARACTER;
                
                for (const auto c:hostPortString)
                {
                        switch (hostParsingState)
                        {
                        case HostParsingState::FIRST_CHARACTER:
                            if(c == '[')
                            {
                                hostParsingState=HostParsingState::IP_LITERAL;
                            }
                            else
                            {
                                hostParsingState=HostParsingState::IPV4_ADDRESS_OR_REG_NAME;
                            }
                            break;
                        
                        case HostParsingState::IP_LITERAL:
                            if(c=='v')
                            {
                                hostParsingState=HostParsingState::IPV_FUTURE_NUMBER;
                                host.push_back(c);
                            }
                            else
                            {
                                hostParsingState=HostParsingState::IPV6_ADDRESS;
                            }
                            break;

                        case:HostParsingState::IPV6_ADDRESS:
                            if(c==']')
                            {
                                if()
                                {

                                }
                            }

                        case HostParsingState::IPV_FUTURE_NUMBER:
                            //  IPvFuture  = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
                            if(c=='.')
                            {
                                hostParsingState=HostParsingState::IPV_FUTURE_BODY;
                            }
                            else if(!isHex(c))
                            {
                                return false;
                            }
                            host.push_back(c);
                            break;

                        case HostParsingState::IPV_FUTURE_BODY:
                            if(c==']')
                            {
                                // IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
                                hostParsingState = HostParsingState::END;
                            }
                            else if(!unreserved(c) || !subdelims(c) || c !=':')
                            {
                                    return false;
                            }
                            else
                            {
                                host.push_back(c);
                            }
                            break;

                        case HostParsingState::END:
                            //  [ userinfo "@" ] host [ ":" port ]
                            if(c==':')
                            {
                                // then we have a port;
                            }
                            else
                            {
                                return false;
                            }
                        default:
                            break;
                        }
                }

        }

   };



    Uri::Uri():
    impl_(std::make_unique<Impl>()) 
    {
        ec=error::no_error;
    }

    Uri::Uri(Uri&&) = default;
    Uri::Uri(Uri const& other)
        : impl_{std::make_unique<Impl>(*other.impl_)}
    {}

    Uri::~Uri() = default;
    Uri& Uri::operator=(Uri&&) = default;
    Uri& Uri::operator=(Uri const& other) 
    {
        *impl_ = *other.impl_;
        return *this;
    }

   bool Uri::ParserFromSring(const std::string& uriString)
   {

        // parser the scheme
        std::string rest;

        if(!impl_->ParseScheme(uriString,rest,ec))
        {
            return false;
        }

        std::string pathString;

       
        // if authority exist
        if(rest.substr(0,2)=="//")
        {

            // If a URI contains an authority component, then the path component
            // must either be empty or begin with a slash ("/") character.

            // skip the '//'
            rest = rest.substr(2);

            // The authority component is preceded by a double slash ("//") and is
            // terminated by the next slash ("/"), question mark ("?"), or number
            // sign ("#") character, or by the end of the URI.

            auto authorityEnd = rest.find_first_of("/?#");

            if(authorityEnd == std::string::npos)
            {
                authorityEnd = rest.length();
            }

            std::string authorityString = rest.substr(0,authorityEnd);
            
            if(!impl_->ParseAuthority(authorityString,ec))
            {
                return false;
            }
        }
         
    


        return true;

   }

   std::string Uri::GetScheme() const
   {
       return impl_->scheme;
   }

   std::string Uri::GetAuthority() const
   {
       return impl_->authority;
   }

}

