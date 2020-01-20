#include "boost/log/trivial.hpp"
#include "boost/log/utility/setup.hpp"
#include <iostream>

namespace logging = boost::log;
// namespace sinks = boost::log::sinks;
// namespace src = boost::log::sources;
// namespace expr = boost::log::expressions;
// namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

void configureLogging();

int main(int argc, char *argv[])
{
    configureLogging();
    BOOST_LOG_TRIVIAL(info) << "Hello, world!\n";
    return 0;
}

void configureLogging()
{
    static const auto format = std::string{"[%TimeStamp%] [%Severity%]:  %Message%"};
    logging::add_console_log(
        std::cout,
        keywords::format = format,
        keywords::auto_flush = true);

    logging::add_common_attributes();
}