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
#include "dante/inc/ProjectNodeTree.hpp"

#include <map>
#include <set>

namespace cbtek {
namespace common {
namespace dante {
namespace platform {
namespace msbuild {


class MSBuildParseEngine : public ParseEngine
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
    virtual std::string getId() const;

    /**
     * @brief parse
     */
    virtual void parse(const std::string & inputFile);

    /**
     * @brief getProjectNodeTreeList
     * @return
     */
    void buildProjectNodeTreeList(std::vector<ProjectNodeTree> &);

    /**
     * @brief createDOT
     * @param filename
     * @param tree
     */
    void createDOT(const std::string & filename,
                   const ProjectNodeTree & tree);



    /**
     * @brief generateDOTs
     * @param dotsPath
     * @param pngsPath
     * @param nodes
     */
    void generateDOTs(const std::string & dotsPath,
                      const std::string & pngsPath, const std::vector<ProjectNodeTree> &nodes);


    void generateD3CSVs(const std::string & csvsPath,
                        const std::vector<ProjectNodeTree> &treeNodes);

    //! Destructor
    ~MSBuildParseEngine();
private:
    std::map<std::string,ProjectNode> m_parseNodeMap;
    std::map<size_t, std::vector<int> > m_orderedProjectNodeMap;
    std::vector<std::set<int> > m_buildPoolSetVector;
    std::map<int, std::map<int, size_t> > m_projectPoolAssignmentMap;
    std::map<std::string,ProjectNodeTree> m_projectNodeTreeMap;
    std::map<int, std::string>  m_indexIdMap;
    std::map<std::string, int>  m_idIndexMap;

    /**
     * @brief parseSLN
     * @param url
     */
    void parseSLN(const std::string &url);

    /**
     * @brief parseCXProj
     * @param node
     * @return
     */
    bool parseCXProj(ProjectNode &node);

    /**
     * @brief adjustDependencyPoolAssignment
     * @param poolId
     * @param lastPool
     * @param nodesIn
     * @param currentPool
     */
    void adjustDependencyPoolAssignment(size_t poolId, const std::set<int> &lastPool,
                                        std::vector<int> &nodesIn,
                                        std::set<int> &currentPool);

    /**
     * @brief checkOtherPools
     * @param index
     * @return
     */
    size_t checkOtherPools(int index) const;

    /**
     * @brief createDepedencyTree
     * @param node
     * @param level
     */
    void createDepedencyTree(ProjectNodeTree &node,
                             int level);

    /**
     * @brief createDOT
     * @param node
     * @param out
     * @param level
     */
    void createDOT(const ProjectNodeTree & node,
                   std::ostream &out,
                   std::set<std::string> *treeNodeOutputCache,
                   int level);


    /**
     * @brief createD3CSV
     * @param filename
     * @param node
     */
    void createD3CSV(const std::string & filename,
                     const ProjectNodeTree &tree);

    /**
     * @brief createD3CSV
     * @param node
     * @param out
     * @param treeNodeOutputCache
     * @param level
     */
    void createD3CSV(const ProjectNodeTree & node,
                     std::ostream &out,
                     std::set<std::string> *treeNodeOutputCache,
                     const std::string &output,
                     int level);

};
}}}}}//end namespace

#endif // _CBTEK_COMMON_DANTE_PLATFORM_MSBuild_MSBuildPARSEENGINE_H

