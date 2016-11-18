/*
    MSBuildParseEngine.h
    
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

#ifndef _CBTEK_COMMON_DANTE_PLATFORM_MSBuild_MSBuildPARSEENGINE_H
#define _CBTEK_COMMON_DANTE_PLATFORM_MSBuild_MSBuildPARSEENGINE_H

#include "dante/inc/ParseEngine.hpp"

namespace cbtek {
namespace common {
namespace dante {
namespace platform {
namespace msbuild {


class MSBuildParseEngine :public ParseEngine
{
public:
    //! Constructor for MSBuildParseEngine
	/*!
        Detailed description for MSBuildParseEngine
	*/
    MSBuildParseEngine();

    /**
     * @brief getId
     * @return
     */
    std::string getId() const;

    /**
     * @brief parse
     */
    void parse(const std::string &topLevelSolutionFile);

    /**
     * @brief getParseNodes
     * @return
     */
    std::vector<ParseNode> getParseNodes() const;


	//! Destructor
    ~MSBuildParseEngine();

};
}}}}}//end namespace

#endif // _CBTEK_COMMON_DANTE_PLATFORM_MSBuild_MSBuildPARSEENGINE_H

