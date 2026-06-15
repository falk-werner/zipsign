/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CLI_APP_HPP
#define CLI_APP_HPP

#include <string>
#include <vector>
#include <ostream>

#include "cli/verb.hpp"
#include "cli/command.hpp"

namespace cli
{

class App
{
public:
    App(std::string const & name_);    
    ~App();    
    int run(int argc, char* argv[], std::ostream & out, std::ostream & err) const;
    Verb & add(std::string const & name, Command command);
    App & setCopyright(std::string const & value);
    App & setDescription(std::string const & value);
    App & setAdditionalInfo(std::string const & value);
private:
    class Private;
    Private * d;
};

}

#endif
