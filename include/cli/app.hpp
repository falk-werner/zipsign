/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CLI_APP_HPP
#define CLI_APP_HPP

#include <functional>
#include <string>
#include <vector>

#include "cli/argument.hpp"
#include "cli/arguments.hpp"

namespace cli
{

using Command = std::function<int (Arguments const &)>;

class App
{
public:
    App(std::string const & name_, Command defaultCommand_);    
    ~App();    
    int run(int argc, char* argv[]) const;
    App & add(Argument const & arg);
    App & setCopyright(std::string const & value);
    App & setDescription(std::string const & value);
    App & setAdditionalInfo(std::string const & value);
private:
    void printUsage() const;
    std::string name;
    std::string copyright;
    std::string description;
    std::string additionalInfo;
    Command defaultCommand;
    std::vector<Argument> args;
};

}

#endif
