#include "boost/log/trivial.hpp"
#include "boost/log/utility/setup.hpp"
#include "boost/log/sources/logger.hpp"
#include "boost/log/sources/basic_logger.hpp"
#include "boost/log/expressions.hpp"

// These 2 heards ae needed for the timestamp expression.
#include "boost/log/expressions/formatters/date_time.hpp"
#include "boost/log/support/date_time.hpp"

// These 2 headers are needed for the BOOST_LOG_ATTRIBUTE_KEYWORD macro.
#include "boost/log/expressions/keyword_fwd.hpp"
#include "boost/log/expressions/keyword.hpp"

#include <iomanip>
#include <iostream>

namespace log = boost::log;

void configureLogging();
void coloring_formatter(log::record_view const &rec,
                        log::formatting_ostream &strm);

// This macro use is needed to access the timestamp using rec[timestamp] in
// the coloring formatter function.
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp",
                            log::attributes::local_clock::value_type);

int main(int argc, char *argv[])
{
    configureLogging();
    BOOST_LOG_TRIVIAL(debug) << "Hello, world!";
    BOOST_LOG_TRIVIAL(info) << "Hello, world!";
    BOOST_LOG_TRIVIAL(warning) << "Hello, world!";
    BOOST_LOG_TRIVIAL(error) << "Hello, world!";
    BOOST_LOG_TRIVIAL(fatal) << "Hello, world!";

    return 0;
}

void configureLogging()
{
    log::add_console_log(
        std::cout,
        log::keywords::auto_flush = true)
        ->set_formatter(&coloring_formatter);

    static const auto format = std::string{
        "[%TimeStamp%] [%Severity%]:  %Message%"};

    log::add_file_log(
        log::keywords::file_name = "../logs/console.log",
        log::keywords::format = format,
        log::keywords::auto_flush = true);

    // This registers the LineID, TimeStamp, ProcessID and ThreadID globally.
    // LineID: counter for each log record, starting from 1.
    // TimeStamp: time at which record is created.
    // ProcessId, ThreadID: process and thread form which log record is emitted.
    // ThreadID not available in single-threaded builds.
    log::add_common_attributes();
}

void coloring_formatter(
    log::record_view const &rec, log::formatting_ostream &strm)
{
    // This function comes from the following Stack Overflow question:
    // https://stackoverflow.com/questions/38309479/how-to-add-color-coding-to-boostlog-console-output
    auto severity = rec[log::trivial::severity];
    if (severity)
    {
        // Set the color
        switch (severity.get())
        {
        case log::trivial::info:
            strm << "\033[32m";
            break;
        case log::trivial::warning:
            strm << "\033[33m";
            break;
        case log::trivial::error:
        case log::trivial::fatal:
            strm << "\033[31m";
            break;
        default:
            break;
        }
    }

    // Format the message here...
    // Accessing the timestamp here is based on the discussion in:
    // https://stackoverflow.com/questions/38618094/how-to-output-timestamp-and-threadid-attributes-with-custom-boostlog-formatter
    strm << "[" << rec[timestamp] << "]["
         << std::setw(8) << severity << "] "
         << rec[log::expressions::smessage];

    if (severity)
    {
        // Restore the default color
        strm << "\033[0m";
    }
}
