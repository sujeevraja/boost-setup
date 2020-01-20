#include "boost/log/trivial.hpp"
#include "boost/log/utility/setup.hpp"
#include "boost/log/sources/logger.hpp"
#include "boost/log/sources/basic_logger.hpp"
#include "boost/log/expressions.hpp"

// These 2 heards ae needed for the timestamp expression.
#include "boost/log/expressions/formatters/date_time.hpp"
#include "boost/log/support/date_time.hpp"

#include "boost/log/expressions/keyword_fwd.hpp"
#include "boost/log/expressions/keyword.hpp"

#include <iomanip>
#include <iostream>

namespace logging = boost::log;
// namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

void configureLogging();
void coloring_formatter(logging::record_view const &rec,
                        logging::formatting_ostream &strm);

enum severity_level
{
    debug,
    info,
    warning,
    error
};

BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", attrs::local_clock::value_type);

int main(int argc, char *argv[])
{
    // ----- basic logging
    configureLogging();
    BOOST_LOG_TRIVIAL(debug) << "Hello, world!";
    BOOST_LOG_TRIVIAL(info) << "Hello, world!";
    BOOST_LOG_TRIVIAL(warning) << "Hello, world!";
    BOOST_LOG_TRIVIAL(error) << "Hello, world!";
    BOOST_LOG_TRIVIAL(fatal) << "Hello, world!";

    // ---- simple custom logger.
    // src::logger lg;
    // BOOST_LOG(lg) << "Hello, world!\n";

    // ----- Creates a severity logger with custom severity levels.
    // src::severity_logger<severity_level> lg;
    // BOOST_LOG_SEV(lg, debug) << "Hello, World\n";

    return 0;
}

void configureLogging()
{
    logging::add_console_log(
        std::cout,
        // keywords::format = format,
        keywords::auto_flush = true)
        ->set_formatter(&coloring_formatter);

    static const auto format = std::string{"[%TimeStamp%] [%Severity%]:  %Message%"};

    // This registers the LineID, TimeStamp, ProcessID and ThreadID globally.
    // LineID: counter for each log record, starting from 1.
    // TimeStamp: time at which record is created.
    // ProcessId, ThreadID: process and thread form which log record is emitted.
    // ThreadID not available in single-threaded builds.
    logging::add_common_attributes();
}

void coloring_formatter(
    logging::record_view const &rec, logging::formatting_ostream &strm)
{
    auto severity = rec[logging::trivial::severity];
    if (severity)
    {
        // Set the color
        switch (severity.get())
        {
        case logging::trivial::info:
            strm << "\033[32m";
            break;
        case logging::trivial::warning:
            strm << "\033[33m";
            break;
        case logging::trivial::error:
        case logging::trivial::fatal:
            strm << "\033[31m";
            break;
        default:
            break;
        }
    }

    // Format the message here...
    strm << "[" << rec[timestamp] << "]["
         << std::setw(8) << severity << "] " << rec[expr::smessage];

    if (severity)
    {
        // Restore the default color
        strm << "\033[0m";
    }
}