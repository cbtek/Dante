/*
    MSBuildParseEngine.cpp
    
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
//----------------------------------------
//MSBuildParseEngine.cpp generated by cberry on 11-17-2016 at 10:40:09 PM
//----------------------------------------
#include "MSBuildParseEngine.h"

#include "utility/inc/FileUtils.hpp"
#include "utility/inc/StringUtils.hpp"
#include "utility/inc/XMLUtils.h"
#include "utility/inc/TimeUtils.hpp"
#include "utility/inc/Exception.hpp"
#include "utility/inc/SystemUtils.hpp"


using namespace cbtek::common::utility;

namespace cbtek {
namespace common {
namespace dante {
namespace platform {
namespace msbuild {


CREATE_EXCEPTION_NO_MSG(PoolIdNotFoundException)

MSBuildParseEngine::MSBuildParseEngine()
{

}

std::string MSBuildParseEngine::getId() const
{
    return "MSBuild";
}

void MSBuildParseEngine::parse(const std::string &topLevelSolutionFile)
{

    parseSLN(topLevelSolutionFile);
    int index = 0;
    for (const auto & it : m_parseNodeMap)
    {
        const ProjectNode & node = it.second;
        m_indexIdMap[index]=node.getId();
        m_idIndexMap[node.getId()]=index;
        m_orderedProjectNodeMap[node.getNumDependencyIds()].push_back(index);
        ++index;
    }

    std::vector<int> zeroDependNodes = m_orderedProjectNodeMap[0];
    std::set<int> lastPool = std::set<int>(zeroDependNodes.begin(),
                                           zeroDependNodes.end());

    size_t poolId = m_buildPoolSetVector.size();
    m_buildPoolSetVector.push_back(lastPool);
    m_orderedProjectNodeMap.erase(0);
    while (m_orderedProjectNodeMap.size())
    {
        for (auto & it : m_orderedProjectNodeMap)
        {
            std::set<int> currentPool;
            adjustDependencyPoolAssignment(poolId,lastPool,it.second,currentPool);
            if (currentPool.size())
            {
                poolId = m_buildPoolSetVector.size();
                m_buildPoolSetVector.push_back(currentPool);
                lastPool = currentPool;
                if (it.second.size() == 0)
                {
                    m_orderedProjectNodeMap.erase(it.first);
                    break;
                }
            }
        }
    }
}

void MSBuildParseEngine::buildProjectNodeTreeList(std::vector<ProjectNodeTree> & treeNodes)
{
    size_t count = m_parseNodeMap.size();
    for (const auto & it : m_parseNodeMap)
    {
        ProjectNodeTree node;
        auto itFind = m_idIndexMap.find(it.second.getId());
        if (itFind!=m_idIndexMap.end())
        {
            node.value = itFind->second;
            createDepedencyTree(node,0);
            treeNodes.push_back(node);
            std::cerr << it.second.getId()<<":"<<(--count)<<std::endl;
        }
    }   
}

void MSBuildParseEngine::createDepedencyTree(
                                             ProjectNodeTree &node,
                                             int level)
{
    auto itFindId = m_indexIdMap.find(node.value);
    if (itFindId == m_indexIdMap.end())
    {
       return;
    }
    std::string nodeId = itFindId->second;
    auto itFindNode = m_parseNodeMap.find(itFindId->second);
    if (itFindNode == m_parseNodeMap.end())
    {
        return;
    }

    std::set<std::string> ids = itFindNode->second.getDependencyIds();
    for(const std::string & id : ids)
    {
        const auto & itPrevChildFind = m_projectNodeTreeMap.find(id);
        if (itPrevChildFind == m_projectNodeTreeMap.end())
        {
            const auto & itChildFind  = m_parseNodeMap.find(id);
            if (itChildFind != m_parseNodeMap.end())
            {
                ProjectNodeTree child;
                const auto & itChildFindIndex = m_idIndexMap.find(id);
                if (itChildFindIndex == m_idIndexMap.end())
                {
                    continue;
                }

                child.value = itChildFindIndex->second;
                if (nodeId != id)
                {
                    createDepedencyTree(child,level+1);
                    m_projectNodeTreeMap[id]=child;
                    node.children.push_back(child);
                }
            }
        }
        else
        {
            node.children.push_back(itPrevChildFind->second);
        }

    }
}

MSBuildParseEngine::~MSBuildParseEngine()
{

}

void MSBuildParseEngine::parseSLN(const std::string & url)
{
    std::string slnData = FileUtils::getFileContents(url);

    std::vector<std::string> lines = StringUtils::split(slnData,"\n");

    for (const auto & line : lines)
    {
        std::string trimmedLine = StringUtils::trimmed(line);
        if (StringUtils::startsWith(trimmedLine,"Project(",false) || StringUtils::startsWith(trimmedLine,"Project (",false))
        {
            std::string data = StringUtils::textAfter(line,"=");
            std::vector<std::string> dataItems = StringUtils::split(data,",");
            StringUtils::clean(dataItems);
            if (dataItems.size() > 2)
            {
                ProjectNode node;
                std::string name = StringUtils::remove(dataItems[0],"\"");
                std::string projectUrl = StringUtils::remove(dataItems[1],"\"");
                std::string id = StringUtils::remove(dataItems[2],"\"");

                StringUtils::removeInPlace(id,"{");
                StringUtils::removeInPlace(id,"}");
                StringUtils::trimmedInPlace(id);
                StringUtils::toUpperInPlace(id);
                StringUtils::trimmedInPlace(name);
                StringUtils::trimmedInPlace(projectUrl);

                node.setName(name);
                node.setId(id);
                if (m_parseNodeMap.count(name))
                {
                    continue;
                }
                std::string path = FileUtils::getParentPath(url);
                size_t cdbackCount = StringUtils::countOccurences(projectUrl,"..\\");
                projectUrl = StringUtils::remove(projectUrl,"..\\");
                path = FileUtils::getParentPath(path,cdbackCount);
                projectUrl = FileUtils::buildFilePath(path,projectUrl);
                node.setUrl(projectUrl);
                if (parseCXProj(node))
                {
                    m_parseNodeMap[id]=node;
                }
            }
        }
    }
}

bool MSBuildParseEngine::parseCXProj(ProjectNode & node)
{
    XMLReader reader;
    std::string universalUrl = StringUtils::replace(node.getUrl(),"\\","/");
    std::string data = FileUtils::getFileContents(universalUrl);

    reader.loadFromString(data);
    XMLDataElement * projects = reader.getElement("Project");
    if (!projects)
    {
        std::cerr << "skipped: "<<universalUrl<<std::endl;
        return false;
    }
    for (const XMLDataElement * child : projects->getChildren())
    {
        for (const XMLDataElement * subchild : child->getChildren())
        {
            if (StringUtils::equals(subchild->getElementName(), "ProjectReference"))
            {
                if (subchild->getNumChildren())
                {
                    const XMLDataElement * projectRef = subchild->getChild("Project");
                    if (projectRef)
                    {
                        std::string id = projectRef->getElementData();
                        StringUtils::removeInPlace(id,"{");
                        StringUtils::removeInPlace(id,"}");
                        StringUtils::toUpperInPlace(id);
                        node.addDependencyId(id);
                    }
                }
            }
        }
    }
    return true;
}

void MSBuildParseEngine::adjustDependencyPoolAssignment(size_t poolId,
                                                        const std::set<int> &lastPool,
                                                        std::vector<int> &nodesIn,
                                                        std::set<int> &currentPool)
{
    for (int a1 = nodesIn.size()-1; a1 >= 0; --a1)
    {

        int nodeIndex = nodesIn[a1];
        std::string nodeId = m_indexIdMap[nodeIndex];
        if (nodeId.empty())
        {
            continue;
        }
        ProjectNode & node = m_parseNodeMap[nodeId];
        std::set<std::string> deps = node.getDependencyIds();
        for (const std::string & id : deps)
        {
            int childIndex = m_idIndexMap[id];
            auto itFind  = lastPool.find(childIndex);

            //If dependency is found then assign it to map
            if (itFind != lastPool.end())
            {
                m_projectPoolAssignmentMap[nodeIndex][childIndex]=poolId;
            }
            else
            {
                try
                {
                    int previousPoolId = checkOtherPools(childIndex);
                    if (previousPoolId >=0 )
                    {
                        m_projectPoolAssignmentMap[nodeIndex][childIndex]=static_cast<size_t>(previousPoolId);
                    }
                }
                catch(...){}
            }

            //When total number of assignments in map equal to
            //the total number of dependencies for a node then
            //the dependency can be added to this pool and
            //the node can be removed from the vector
            if (m_projectPoolAssignmentMap[nodeIndex].size() ==
                node.getNumDependencyIds())
            {
                currentPool.insert(nodeIndex);
                nodesIn.erase(nodesIn.begin()+a1);
                break;
            }
        }
    }
}
//need to create reverselookup here
size_t MSBuildParseEngine::checkOtherPools(int index) const
{
    for(size_t a1 = 0; a1 < m_buildPoolSetVector.size();++a1)
    {
        const auto & itFind = m_buildPoolSetVector[a1].find(index);
        if (itFind != m_buildPoolSetVector[a1].end())
        {
            return a1;
        }
    }

    throw PoolIdNotFoundException(EXCEPTION_TAG+"Node not currently associated with any build pools");
}

void MSBuildParseEngine::createDOT(const ProjectNodeTree & node,
                             std::ostream & out,
                             std::set<std::string> * treeNodeOutputCache,
                             int level)
{
    auto itFindId = m_indexIdMap.find(node.value);
    if (itFindId!=m_indexIdMap.end())
    {
        std::string parentId=itFindId->second;
        const auto & itFindNode = m_parseNodeMap.find(parentId);
        if (itFindNode == m_parseNodeMap.end())
        {
            return;
        }
        const ProjectNode & parentNode = itFindNode->second;
        std::string output;
        output = "    " + parentNode.getName() +";\n";
        auto itFindInCache = treeNodeOutputCache->find(output);
        if (itFindInCache == treeNodeOutputCache->end())
        {
            out << output;
            treeNodeOutputCache->insert(output);
        }
        std::string rank;
        if (node.children.size() > 1)
        {
            for (const ProjectNodeTree & child : node.children)
            {
                auto itFindChildId = m_indexIdMap.find(child.value);
                if (itFindChildId!=m_indexIdMap.end())
                {

                    std::string childId=itFindChildId->second;
                    const auto & itFindChildNode = m_parseNodeMap.find(childId);
                    if (itFindChildNode == m_parseNodeMap.end())
                    {
                        return;
                    }
                    ProjectNode & childNode = itFindChildNode->second;
                    rank += " \""+childNode.getName() +"\";";
                }
            }
        }

        if (!rank.empty())
        {
            rank = "    { rank = same;"+rank+"};\n";
            //out << rank;
        }

        for (const ProjectNodeTree & child : node.children)
        {

            auto itFindChildId = m_indexIdMap.find(child.value);
            if (itFindChildId!=m_indexIdMap.end())
            {

                std::string childId=itFindChildId->second;
                const auto & itFindChildNode = m_parseNodeMap.find(childId);
                if (itFindChildNode == m_parseNodeMap.end())
                {
                    return;
                }
                ProjectNode & childNode = itFindChildNode->second;
                output = "    " + parentNode.getName() +" -> "+childNode.getName()+";\n";
                auto itFindInChildCache = treeNodeOutputCache->find(output);
                if (itFindInChildCache == treeNodeOutputCache->end())
                {
                    out << output;
                    treeNodeOutputCache->insert(output);
                }

            }
        }
        for (const ProjectNodeTree & child : node.children)
        {
            createDOT(child,out,treeNodeOutputCache,level+1);
        }
    }
}


void MSBuildParseEngine::generateDOTs(const std::string &dotsPath,
                                      const std::string &pngsPath,
                                      const std::vector<ProjectNodeTree> & treeNodes)
{
    if (!FileUtils::isDirectory(dotsPath))
    {
        FileUtils::createDirectory(dotsPath);
    }

    if (!FileUtils::isDirectory(pngsPath))
    {
        FileUtils::createDirectory(pngsPath);
    }

    std::set<std::string> filenames;
    for (const auto & tree : treeNodes)
    {
        auto itFindId = m_indexIdMap.find(tree.value);

        if (itFindId!=m_indexIdMap.end())
        {
            const auto & itFindNode = m_parseNodeMap.find(itFindId->second);
            if (itFindNode != m_parseNodeMap.end())
            {
                std::string filename = itFindNode->second.getName();
                filenames.insert(filename);
                size_t count = filenames.count(filename);
                filename+="_"+StringUtils::toString(count)+".dot";
                std::string outputFilename = FileUtils::buildFilePath(pngsPath,filename+".png");
                std::string inputFilename = FileUtils::buildFilePath(dotsPath,filename);
                createDOT(inputFilename,tree);
                std::string command = "dot -Tpng \""+inputFilename+"\" -o \""+outputFilename+"\"";
                std::cerr << command<<std::endl;
                SystemUtils::execute(command);

            }
        }
    }
}

void MSBuildParseEngine::createD3CSV(const std::string &filename,
                                     const ProjectNodeTree &tree)
{
    std::ofstream fout(filename.c_str());
    auto itFindId = m_indexIdMap.find(tree.value);
    std::set<std::string> * treeNodeOutputCache = new std::set<std::string>();
    if (itFindId!=m_indexIdMap.end())
    {
        std::string treeId=itFindId->second;
        auto itFindNode = m_parseNodeMap.find(treeId);
        if (itFindNode == m_parseNodeMap.end())
        {
            return;
        }
        ProjectNode & node = itFindNode->second;
        fout << "id,value"<<std::endl;
        std::string output;
        createD3CSV(tree,fout,treeNodeOutputCache,output,0);
    }
    fout.close();
    delete treeNodeOutputCache;
    treeNodeOutputCache = 0;
}

void MSBuildParseEngine::createD3CSV(const ProjectNodeTree &node,
                                     std::ostream &out,
                                     std::set<std::string> *treeNodeOutputCache,
                                     const std::string & output,
                                     int level)
{
    auto itFindId = m_indexIdMap.find(node.value);
    std::string newOutput = output;
    if (itFindId!=m_indexIdMap.end())
    {
        std::string parentId=itFindId->second;
        const auto & itFindNode = m_parseNodeMap.find(parentId);
        if (itFindNode == m_parseNodeMap.end())
        {
            return;
        }
        const ProjectNode & parentNode = itFindNode->second;
        newOutput += parentNode.getName();
        auto itFindInCache = treeNodeOutputCache->find(newOutput);
        if (itFindInCache == treeNodeOutputCache->end())
        {
            out << newOutput << std::endl;
            treeNodeOutputCache->insert(newOutput);
        }

//        for (const ProjectNodeTree & child : node.children)
//        {

//            auto itFindChildId = m_indexIdMap.find(child.value);
//            if (itFindChildId!=m_indexIdMap.end())
//            {

//                std::string childId=itFindChildId->second;
//                const auto & itFindChildNode = m_parseNodeMap.find(childId);
//                if (itFindChildNode == m_parseNodeMap.end())
//                {
//                    return;
//                }
//                ProjectNode & childNode = itFindChildNode->second;
//                std::string newChildOutput = newOutput + ".";
//                auto itFindInChildCache = treeNodeOutputCache->find(newChildOutput);
//                if (itFindInChildCache == treeNodeOutputCache->end())
//                {
//                    out << newChildOutput << std::endl;
//                    treeNodeOutputCache->insert(newChildOutput);
//                }

//            }
//        }

        for (const ProjectNodeTree & child : node.children)
        {
            auto itFindChildId = m_indexIdMap.find(child.value);
            if (itFindChildId!=m_indexIdMap.end())
            {

                std::string childId=itFindChildId->second;
                const auto & itFindChildNode = m_parseNodeMap.find(childId);
                if (itFindChildNode == m_parseNodeMap.end())
                {
                    return;
                }
                ProjectNode & childNode = itFindChildNode->second;
                std::string newChildOutput = newOutput + ".";
                createD3CSV(child,out,treeNodeOutputCache,newChildOutput,level+1);
            }
        }
    }
}

void MSBuildParseEngine::generateD3CSVs(const std::string &csvsPath,
                                        const std::vector<ProjectNodeTree> &treeNodes)
{
    if (!FileUtils::isDirectory(csvsPath))
    {
        FileUtils::createDirectory(csvsPath);
    }

    std::set<std::string> filenames;
    for (const auto & tree : treeNodes)
    {
        auto itFindId = m_indexIdMap.find(tree.value);

        if (itFindId!=m_indexIdMap.end())
        {
            const auto & itFindNode = m_parseNodeMap.find(itFindId->second);
            if (itFindNode != m_parseNodeMap.end())
            {
                std::string filename = itFindNode->second.getName();
                filenames.insert(filename);
                size_t count = filenames.count(filename);
                filename+="_"+StringUtils::toString(count)+".csv";
                std::string outputFilename = FileUtils::buildFilePath(csvsPath,filename);
                std::cerr <<"Creating D3CSV at \""<<outputFilename<<"\"..."<<std::endl;
                std::uint64_t start = TimeUtils::getMillisecondsNow();
                createD3CSV(outputFilename,tree);
                std::cerr << "Done in "<<(TimeUtils::getMillisecondsNow()-start)<<"ms"<<std::endl;
            }
        }
    }
}

void MSBuildParseEngine::createDOT(const std::string & filename,
                             const ProjectNodeTree & tree)
{
    std::ofstream fout(filename.c_str());
    auto itFindId = m_indexIdMap.find(tree.value);
    std::set<std::string> * treeNodeOutputCache = new std::set<std::string>();
    if (itFindId!=m_indexIdMap.end())
    {
        std::string treeId=itFindId->second;
        auto itFindNode = m_parseNodeMap.find(treeId);
        if (itFindNode == m_parseNodeMap.end())
        {
            return;
        }
        ProjectNode & node = itFindNode->second;
        fout << "digraph "<<StringUtils::toUpper(node.getName())<<std::endl;
        fout << "{"<<std::endl;
        fout << "    compound=true;"<<std::endl;
        fout << "    concentrate=true;"<<std::endl;
        fout << "    ranksep=2.25;"<<std::endl;
        createDOT(tree,fout,treeNodeOutputCache,0);
        fout << "}"<<std::endl;
    }
    fout.close();
    delete treeNodeOutputCache;
    treeNodeOutputCache = 0;
}




}}}}}//end namespace


