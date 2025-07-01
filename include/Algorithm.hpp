#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <string>

namespace Algorithm {

std::string urlDecode(const std::string& input);

std::string xorDecode(const std::string& data, char key);

std::string base64Decode(const std::string& input);

std::string extractString(const std::string& source, 
                          const std::string& start, 
                          const std::string& end);

} 

#endif
