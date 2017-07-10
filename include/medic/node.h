#ifndef MEDIC_NODE_INCLUDED
#define MEDIC_NODE_INCLUDED


#include <vector>
#include <string>
#include <maya/MString.h>
#include <maya/MObject.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MItDependencyNodes.h>


namespace MEDIC
{
    class MdNode;
    class MdNodeContainer;
    class MdNodeIterator;
    typedef std::vector<MdNode *> NodePtrVec;

	class MdNode
	{
        public:
            MdNode();
            MdNode(MObject &obj);
            MdNode(std::string name);
            ~MdNode();
            static bool Match(MObject &obj);
            MObject object() const;
            std::string name() const;
            std::string type() const;
            bool isDag() const;
            void parents(MdNodeContainer *container) const;
            void children(MdNodeContainer *container) const;
            MDagPath getPath() const;
            MFnDependencyNode &dg();
            MFnDagNode &dag();

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
            MdNodeContainer();
            ~MdNodeContainer();
            bool append(MdNode *n);
            bool remove(MdNode *n);
            void clear();
            size_t size() const;
            MdNodeIterator iterator();

        private:
            NodePtrVec m_nodes;
    };

    class MdNodeIterator
    {
        public:
            MdNodeIterator();
            MdNodeIterator(MdNodeContainer *container);
            ~MdNodeIterator();
            MdNode *next();
            bool isDone();

        private:
            MdNodeContainer *m_container;
            NodePtrVec::iterator m_cur;
    };

    void GetNodes(MdNodeContainer *container);
}


#endif