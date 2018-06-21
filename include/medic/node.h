#ifndef MEDIC_NODE_INCLUDED
#define MEDIC_NODE_INCLUDED


#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <maya/MString.h>
#include <maya/MObject.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItDag.h>
#include <maya/MGlobal.h>
#include "medic/platform.h"


namespace MEDIC
{
    class MdNode;
    class MdNodeContainer;
    class MdNodeIterator;
    typedef std::vector<MdNode *> NodePtrVec;

	class MdNode
	{
        public:
            MEDIC_EXPORT MdNode();
            MEDIC_EXPORT MdNode(MObject &obj);
            MEDIC_EXPORT MdNode(std::string name);
            MEDIC_EXPORT ~MdNode();
            static bool Match(MObject &obj);
            MEDIC_EXPORT MObject &object();
            MEDIC_EXPORT std::string name() const;
            MEDIC_EXPORT std::string type() const;
            MEDIC_EXPORT bool isDag() const;
            MEDIC_EXPORT void parents(MdNodeContainer *container) const;
            MEDIC_EXPORT void children(MdNodeContainer *container) const;
            MEDIC_EXPORT MDagPath getPath() const;
            MEDIC_EXPORT MFnDependencyNode &dg();
            MEDIC_EXPORT MFnDagNode &dag();

        private:
            MObject m_obj;
            bool m_is_dag;
            MFnDependencyNode m_dg;
            MFnDagNode m_dag;
	};


    class MdNodeContainer
    {
        friend class MdNodeIterator;
        public:
            MEDIC_EXPORT MdNodeContainer();
            MEDIC_EXPORT ~MdNodeContainer();
            bool append(MdNode *n);
            bool remove(MdNode *n);
            void clear();
            size_t size() const;
            MEDIC_EXPORT MdNodeIterator iterator();

        private:
            NodePtrVec m_nodes;
    };

    class MdNodeIterator
    {
        public:
            MEDIC_EXPORT MdNodeIterator();
            MEDIC_EXPORT MdNodeIterator(MdNodeContainer *container);
            MEDIC_EXPORT ~MdNodeIterator();
            MEDIC_EXPORT MdNode *next();
            MEDIC_EXPORT bool isDone();

        private:
            MdNodeContainer *m_container;
            NodePtrVec::iterator m_cur;
    };

    void GetNodes(MdNodeContainer *container);

    void GetNodesInSelection(MdNodeContainer *container);
}


#endif