#ifndef PARSER_HPP
#define PARSER_HPP

#include "ssmath3/paramSet.hpp"
#include <algorithm>
#include <functional>
#include <string>

namespace ssrt {
struct RunningOptions {
  std::string outfile = "";
  std::string scene;
};

// @author = Selan Santos
// ===
/// Lambda expression that transform a c-style string to a lowercase c++-stype
/// string version.
inline static auto str_to_lower = [](const char *c_str) -> std::string {
  std::string str{c_str};
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
};

/// Map that binds a convertion function to an attribute name.
using ConverterFunction = std::function<bool(
    const std::string &, const std::string &, ssmath3::ParamSet *)>;
// ===

class Parser 
{
  private:

  
  public:
  
    static void validate_arguments(int argc, char **argv, RunningOptions &run_opt);
    static void parse_scene(const std::string &filename);
    
    static void parse_attribute(const std::string &attr_name,
                                const std::string &attr_content,
                                ssmath3::ParamSet *ps);
};

}; // namespace ssrt

#endif //< PARSER_HPP