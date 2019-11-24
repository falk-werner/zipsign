/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CLI_APP_HPP
#define CLI_APP_HPP

#include <string>
#include <vector>

#include "cli/app_info.hpp"
#include "cli/argument.hpp"
#include "cli/arguments.hpp"
#include "cli/verb.hpp"

namespace cli
{

class App: public AppInfo
{
public:
    App(std::string const & name_);    
    ~App() override;    
    std::string const & getName() const override;
    std::string const & getDescription() const override;
    std::string const & getCopyright() const override;
    int run(int argc, char* argv[]) const;
    App & add(Verb const & verb);
    App & setCopyright(std::string const & value);
    App & setDescription(std::string const & value);
    App & setAdditionalInfo(std::string const & value);
private:
    void printUsage() const;
    Verb const * getVerb(std::string const & name) const;
    std::string name;
    std::string copyright;
    std::string description;
    std::string additionalInfo;
    std::vector<Verb> verbs;
};

}

#endif
