/*
    ParseEngine.h
    
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

#ifndef _CBTEK_COMMON_DANTE_PARSEENGINE_H
#define _CBTEK_COMMON_DANTE_PARSEENGINE_H

#include "ProjectNodeTree.hpp"

#include <vector>
#include <string>

namespace cbtek {
namespace common {
namespace dante {

class ParseEngine
{
public:

    /**
     * @brief getId
     * @return
     */
    virtual std::string getId() const = 0;

    /**
     * @brief parse
     */
    virtual void parse(const std::string & inputFile) = 0;

    /**
     * @brief getProjectNodeTreeList
     * @return
     */
    virtual void buildProjectNodeTreeList(std::vector<ProjectNodeTree> &) = 0;

    //! Virtual Destructor
    virtual ~ParseEngine(){}
};
}}}//end namespace

#endif

