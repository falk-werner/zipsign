#include "cli/arguments.hpp"
#include <unordered_map>

namespace cli
{

class DefaultArguments: public Arguments
{
public:
    DefaultArguments();
    ~DefaultArguments() override;
    bool contains(char id) const override;
    std::string const & get(char id) const override;
    void set(char id, std::string const & value);
private:
    std::unordered_map<char, std::string> values;    

};

}