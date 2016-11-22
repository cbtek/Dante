/*
    ProjectNodeTree.h
    

*/

#ifndef _CBTEK_COMMON_DANTE_PROJECTNODETREE_H
#define _CBTEK_COMMON_DANTE_PROJECTNODETREE_H

#include "ProjectNode.h"

#include <vector>

namespace cbtek {
namespace common {
namespace dante {

struct ProjectNodeTree
{
    int value;
    std::vector<ProjectNodeTree> children;
};
}}}//end namespace

#endif

