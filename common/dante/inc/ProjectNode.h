/*
    ProjectNode.h
    
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

#ifndef _CBTEK_COMMON_DANTE_PARSENODE_H
#define _CBTEK_COMMON_DANTE_PARSENODE_H

#include <string>
#include <set>

namespace cbtek {
namespace common {
namespace dante {


class ProjectNode
{
	public:
    //! Constructor for ProjectNode
	/*!
        Detailed description for ProjectNode
	*/
    ProjectNode();

    /**
    * @brief Setter for m_id
    * @param Value to replace m_id
    */
    void setId(const std::string & value);

    /**
    * @brief Setter for m_name
    * @param Value to replace m_name
    */
    void setName(const std::string & value);

    /**
    * @brief Setter for m_projectFilename
    * @param Value to replace m_projectFilename
    */
    void setUrl(const std::string & value);


    /**
    * @brief Getter for m_id
    * @return Return copy of m_id
    */
    const std::string & getId() const;

    /**
    * @brief Getter for m_name
    * @return Return copy of m_name
    */
    const std::string & getName() const;

    /**
    * @brief Getter for m_projectFilename
    * @return Return copy of m_projectFilename
    */
    const std::string & getUrl() const;

    /**
     * @brief addDependencyId
     * @param dependentId
     */
    void addDependencyId(const std::string & dependentId);

    /**
     * @brief getNumDependencyIds
     * @return
     */
    size_t getNumDependencyIds() const;

    /**
     * @brief getDependencyIds
     * @return
     */
    const std::set<std::string> getDependencyIds() const;

	//! Destructor
    ~ProjectNode();

private:
    std::set<std::string> m_dependencies;
    std::string m_id;
    std::string m_name;
    std::string m_url;

};
}}}//end namespace

#endif // _CBTEK_COMMON_DANTE_PARSENODE_H

