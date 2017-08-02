#include "../include/detail/detail.hpp"
#include "include/test.hpp"

#include <iostream>

using namespace test;


class ConsoleLogger : public Logger
{
public:
    void log(const std::string &str) override
    {
        std::cout << str << std::endl;
    }
};


auto scgicxx_detail_suite()
{
    auto suite = create_suite("scgicxx.detail");

    suite->add_case("parse_net_string()", [](auto &assert){

        const char *netstring = "12:Hello world! 34354333231";
        array_view<const char> av{netstring, strlen(netstring)};
        auto res = scgicxx::detail::parse_net_string(av);

        assert.equal("size", res.size(), 12);
        assert.equal("data", res, string_view{netstring + 3, 12});
        assert("data2", res == "Hello world!");
    });

    suite->add_case("split_into_tokens()", [](auto &assert){

        const char *str = "Hello   world this is me! ";
        string_view sv{str, strlen(str)};
        std::vector<string_view> tokens;
        std::size_t res = scgicxx::detail::split_into_tokens(sv, tokens, ' ');

        assert.equal("size", res, 5);
        assert("word hello", tokens[0] == "Hello");
        assert("word world", tokens[1] == "world");
        assert("word this", tokens[2] == "this");
        assert("word is", tokens[3] == "is");
        assert("word me!", tokens[4] == "me!");
    });

    return suite;
}


int main(int argc, const char * const * argv)
{
    ConsoleLogger logger;

    std::vector<std::shared_ptr<TestSuite>> suites = {
            scgicxx_detail_suite()
    };

    execute(suites, logger);

    return 0;
}