#include "boost/log/trivial.hpp"       // provides the BOOST_LOG_TRIVIAL macro.
#include "boost/log/utility/setup.hpp" // to configure logging.

// These 2 headers are needed for the timestamp expression.
#include "boost/log/expressions/formatters/date_time.hpp"
#include "boost/log/support/date_time.hpp"

// These 2 headers are needed for the BOOST_LOG_ATTRIBUTE_KEYWORD macro.
#include "boost/log/expressions/keyword_fwd.hpp"
#include "boost/log/expressions/keyword.hpp"

// headers for command-line parsing.
#include "boost/program_options.hpp"

#include <iomanip>
#include <iostream>

namespace log = boost::log;
namespace po = boost::program_options;

void configureLogging();
void coloring_formatter(log::record_view const &rec,
                        log::formatting_ostream &strm);

bool parseArgs(int argc, char *argv[]);

// This macro use is needed to access the timestamp using rec[timestamp] in
// the coloring formatter function.
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp",
                            log::attributes::local_clock::value_type);

int main(int argc, char *argv[])
{
    configureLogging();
    const auto runCode = parseArgs(argc, argv);
    if (runCode)
    {
        BOOST_LOG_TRIVIAL(debug) << "Hello, world!";
        BOOST_LOG_TRIVIAL(info) << "Hello, world!";
        BOOST_LOG_TRIVIAL(warning) << "Hello, world!";
        BOOST_LOG_TRIVIAL(error) << "Hello, world!";
        BOOST_LOG_TRIVIAL(fatal) << "Hello, world!";
    }

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

bool parseArgs(int argc, char *argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()("help", "produce help messaeg");

    int compression;
    desc.add_options()("compression",
                       po::value<int>(&compression)->default_value(10),
                       "compression level");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        BOOST_LOG_TRIVIAL(info) << "\n"
                                << desc;
        return false;
    }

    if (vm.count("compression"))
    {
        BOOST_LOG_TRIVIAL(info) << "compression level "
                                << vm["compression"].as<int>();

        BOOST_LOG_TRIVIAL(info) << "compression level from variable "
                                << compression;
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "compression not set";
    }
    return true;
}
