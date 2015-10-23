#ifndef GLA_ENGINE_STACKGRAPH_H
#define GLA_ENGINE_STACKGRAPH_H

#include <vector>

#include <iostream>
#include <string>

namespace gla {
namespace engine {



/**
 * @brief The StackGraph class
 *
 * Implements a graph structure using a vector instead of pointers.
 */
template<class T>
class StackGraph
{

public:
    using NodeID = unsigned int;

    static const NodeID NoNode = 0xFFFFFFFF;


    struct InstanceData {
      //  T   data;

        NodeID parent      = NoNode;
        NodeID nextsibling = NoNode;
        NodeID prevsibling = NoNode;
        NodeID firstchild  = NoNode;
    };

    StackGraph() : mNumNodes(0){}

    void print(std::string spaces = std::string(""), NodeID root=0 )
    {
        std::cout << spaces << "ⱶ-->" << mObjects[root] << std::endl;
        if( mData[root].firstchild != NoNode ) print( spaces+"    ", mData[root].firstchild);

        if( mData[root].nextsibling != NoNode ) print( spaces, mData[root].nextsibling);


    }

    NodeID Create(NodeID parentID = NoNode, const T & InitialData = T() )
    {
        auto id = GetFreeID(InitialData);

        mObjects[id]          = InitialData;
//        mData[id].data        = InitialData;
        mData[id].parent      = NoNode;
        mData[id].nextsibling = NoNode;
        mData[id].prevsibling = NoNode;
        mData[id].firstchild  = NoNode;
        //std::cout << "Child Created: " << id << std::endl;
        //std::cout << "  Parent     : " << GetParent(id) << std::endl;
        //std::cout << "  Child      : " << GetFirstChild(id) << std::endl;
        //std::cout << "  NSibling   : " << GetNextSibling(id) << std::endl;
        //std::cout << "  PSibling   : " << GetPrevSibling(id) << std::endl;
        Link(parentID, id);

        //std::cout << "Child Linked: " << id << std::endl;
        //std::cout << "  Parent     : " << GetParent(id) << std::endl;
        //std::cout << "  Child      : " << GetFirstChild(id) << std::endl;
        //std::cout << "  NSibling   : " << GetNextSibling(id) << std::endl;
        //std::cout << "  PSibling   : " << GetPrevSibling(id) << std::endl;
        return id;

    }


    void Destroy(NodeID id)
    {

        auto i = GetFirstChild(id);//mData[id].firstchild;
        while( !IsValid(i)   )
        {
            auto j = i;
            i = GetNextSibling(i);//mData[i].nextsibling;
            Destroy( j );
        }

        UnLink(id);

        mNumNodes--;
        mFreeNodes.push_back(id);
        std::cout << id << " freed." << std::endl;
    }

    inline bool IsValid(NodeID id)          const { return id!=NoNode; }
    inline NodeID GetFirstChild(NodeID id)  const { return mData[id].firstchild; }
    inline NodeID GetNextSibling(NodeID id) const { return mData[id].nextsibling; }
    inline NodeID GetPrevSibling(NodeID id) const { return mData[id].prevsibling; }
    inline NodeID GetParent(NodeID id)      const { return mData[id].parent;      }

    // links a parent to a child
    void  Link(NodeID parent, NodeID child)
    {
        // if the parent already has a child, add it to the sibling
        mData[child].parent = parent;

        std::cout << "Linking Parent: " << parent << " with child " << child << std::endl;
        if( !IsValid(parent) ) return;


        if( IsValid( GetFirstChild(parent) ) )
        {
            std::cout << "Valid first child: " << GetFirstChild(parent) << std::endl;
            AddSibling(mData[parent].firstchild, child);
        } else {
            mData[child].parent = parent;
            mData[parent].firstchild = child;
        }
    }

    // unlinks a child from its parent
    void   UnLink(NodeID child)
    {
        // if the child is the first child, make sure the
        // parent points to the next sibling.
        if( IsValid( GetParent(child) ) )
        {
            if( mData[mData[child].parent].firstchild == child )
            {
                mData[mData[child].parent].firstchild = mData[child].nextsibling;
            }
        }

        // if it has a previous sibling, then make sure the previous sibling is pointing to the next child.
        //if( mData[child].prevsibling != NoNode )
        if( IsValid( GetPrevSibling(child) ))
        {
            mData[mData[child].prevsibling].nextsibling = mData[child].nextsibling;
        }

        // if it has a next sibling, make sure the next sibling is pointing to the previous child.
        //if( mData[child].nextsibling != NoNode )
        if( IsValid( GetNextSibling(child) ) )
        {
            mData[mData[child].nextsibling].prevsibling = mData[child].prevsibling;
        }

        mData[child].parent = NoNode;
    }

    NodeID AddSibling(NodeID siblingID, NodeID newSiblingID)
    {
        std::cout << "Adding sibling " << newSiblingID << " to: " << siblingID << std::endl;
        // can't add a sibling to no node
        if( !IsValid(siblingID) ) return NoNode;

        // loop until we get the last sibling
        //while( mData[siblingID].nextsibling != NoNode )
        while( IsValid( GetNextSibling(siblingID) ) )
        {
            siblingID = GetNextSibling(siblingID);//mData[siblingID].nextsibling;
        }

        mData[siblingID].nextsibling    = newSiblingID;
        mData[newSiblingID].prevsibling = siblingID;

        return siblingID;
    }

    NodeID GetFreeID(const T & InitialData)
    {
        //InstanceData D;
        if( mNumNodes == mData.size() )
        {
            mData.push_back( InstanceData() );
            mObjects.push_back( T() );
            mNumNodes++;
            std::cout << "New node: " << (mData.size()-1) << std::endl;
            return (NodeID)(mData.size()-1);
        }

        NodeID i = (NodeID)mFreeNodes.back();
        mFreeNodes.pop_back();
        mNumNodes++;

        return i;
    }




    inline T & Get(NodeID id) { return mObjects[id]; }

    inline std::vector<T> & GetObjects() { return mObjects; }


    std::vector<T>            mObjects;
    std::vector<InstanceData> mData;
    std::vector<unsigned int> mFreeNodes;
    unsigned int              mNumNodes;
};



}}

#endif // STACKGRAPH_H

