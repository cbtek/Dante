/**
MIT License

Copyright (c) 2016 cbtek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <iostream>

#include "utility/inc/StringUtils.hpp"
#include "utility/inc/FileUtils.hpp"

#include "dante/inc/platform/msbuild/MSBuildParseEngine.h"
#include "dante/inc/platform/cmake/CMakeParseEngine.h"

using namespace cbtek::common::utility;
using namespace cbtek::common::dante;
using namespace cbtek::common::dante::platform::msbuild;
using namespace cbtek::common::dante::platform::cmake;

int main(int argc, char ** argv)
{       
    MSBuildParseEngine pe;
    std::uint64_t start = TimeUtils::getMillisecondsNow();
    std::cerr<<"Parsing Solutions..."<<std::endl;
    pe.parse("D:/GitRepos/IMEA_11.1/Products/Build/IMEAProducts.sln");
    std::cerr << TimeUtils::getMillisecondsNow()-start <<std::endl;
    std::vector<ProjectNodeTree> nodeTreeList;
    pe.buildProjectNodeTreeList(nodeTreeList);
    std::cerr << "Done in "<<TimeUtils::getMillisecondsNow()-start<<"ms"<<std::endl;
    std::cerr<<"Generating output data..."<<std::endl;
    pe.generateD3CSVs("csv",nodeTreeList);
    return 0;
}

