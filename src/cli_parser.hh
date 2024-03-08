#ifndef CLI_PARSER_HH
#define CLI_PARSER_HH

#include <map>
#include <string>

std::map<std::string, std::string> parse_args(int argc, char *argv[]);

#endif