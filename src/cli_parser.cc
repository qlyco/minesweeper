#include <map>
#include <string>

std::map<std::string, std::string> parse_args(int argc, char *argv[]) {
    std::map<std::string, std::string> args;

    for (int i = 1; i != argc; i++) {
        if (std::string(argv[i]) == "--seed" || std::string(argv[i]) == "-S") {
            args.emplace("SEED", argv[++i]);
        } else if (std::string(argv[i]) == "--diff" || std::string(argv[i]) == "-D") {
            args.emplace("DIFFICULTY", argv[++i]);
        } else if (std::string(argv[i]) == "--width" || std::string(argv[i]) == "-W") {
            args.emplace("WIDTH", argv[++i]);
        } else if (std::string(argv[i]) == "--height" || std::string(argv[i]) == "-H") {
            args.emplace("HEIGHT", argv[++i]);
        } else if (std::string(argv[i]) == "--mines" || std::string(argv[i]) == "-M") {
            args.emplace("MINES", argv[++i]);
        }
    }

    return args;
}