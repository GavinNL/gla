#ifndef GLA_OCTREE_HPP
#define GLA_OCTREE_HPP

#include <glm/glm.hpp>

#include "../geometry/boundingbox.h"
#include "../geometry/frustrum.h"

#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <type_traits>
#include <cmath>
#include <functional>

namespace gla
{


#define NODE_EXPANSION_COEF 1.3



#define PRINTv(A) "(" << A.x << ", " << A.y << ", " << A.z << ")"





//template<typename object> class Node;



//struct iVec3_KeyHash
//{
//     std::size_t operator()(const glm::ivec3 & k) const
//     {
//         return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1) ^ (std::hash<int>()(k.z) << 2);
//     }

//     bool operator()(const glm::ivec3 & lhs, const glm::ivec3 & rhs) const
//     {
//        return lhs.x==rhs.x && lhs.y==rhs.y && lhs.z==rhs.z;
//     }

//};


#if 0

template< typename object  >
class OctreeScene
{
public:

    using aabb_type = gla::BoundingBox<float>;
    using scene_type= OctreeScene<object>;

    using NodeType       = Octree<object,float,8,5>;
    using map_type       = std::unordered_map<glm::ivec3, NodeType*, iVec3_KeyHash, iVec3_KeyHash>;
    using const_iterator = typename map_type::const_iterator;
    using iterator       = typename map_type::iterator      ;



    static const int RootNodeSize = 1<<10;

private:
    std::vector<object>    m_TooBig; // object that are too big to fit into each octree
    NodeType   				m_Root;

    map_type  m_RootNodes;

public:

    OctreeScene();

    void        print();
    std::size_t count( const aabb_type & volume );
    std::size_t count();

    node_type*  Insert( object & obj);

    NodeType* GetRootNode();
    NodeType* GetRootNode(const glm::ivec3 & id);
    NodeType* GetRootNode(const vec_type & position);

    void Optimize();
    void Remove( object & obj);
    bool Update( object & obj );


    const_iterator  begin() const { return m_RootNodes.begin(); }
    iterator        begin() { return m_RootNodes.begin(); }
    const_iterator  end() const { return m_RootNodes.end(); }
    iterator        end() { return m_RootNodes.end(); }

};


#endif

template<typename object, typename dim_type=float, int Maxobject =8 , int Minobject=5>
class Octree
{
public:
    //friend class OctreeScene<object>;

    using node_type = Octree<object, dim_type, Maxobject, Minobject>;
    using aabb_type = gla::BoundingBox<dim_type>;
    using vec_type  = typename aabb_type::vec_type;



private:
    aabb_type                        m_aabb;
    std::vector<object>              m_Obj; // list of object in this node
    std::vector< node_type >         m_Children;

    std::size_t                      m_Totalobject=0; // total object in this node+child nodes.

    void                            *m_parent = nullptr;
    bool                             m_IsRootNode = true;


public:


    /**
     * @brief GetNode
     * @param obj
     * @return
     *
     * This method must be implemented by the user. Given a reference to an object, return
     * the bounding box that encloses it.
     */
    static aabb_type    GetBoundingBox( object & obj );

    /**
     * @brief SetNode
     * @param obj
     * @param m
     *
     * This method must be implemented by the user. Given a refernce to an object and
     * a pointer to a node. Save the node in the object
     */
    static void         SetNode( object & obj, node_type *m);

    /**
     * @brief GetNode
     * @param obj
     * @return
     *
     * This method must be implemented by the user. Given a reference to an object and
     * return a pointer to the node that this object belongs to, or nullptr if it
     * does not exist in a node
     */
    static node_type*   GetNode( object & obj );

    bool IsRoot() const { return m_IsRootNode; }


    static int & total()
    {
        static int t=0;
        return t;
    }

    Octree(float size) : m_aabb( vec_type( -size ), vec_type(size) )
    {
        ++total();
    }

    Octree() : m_aabb( vec_type( -(1<<20) ), vec_type(1<<20) )
    {
        ++total();
    }


    ~Octree()
    {
        Merge();


        --total();
       // std::cout << "Total Nodes:" << total() << std::endl;
    }

    /**
     * @brief GetParent
     * @return
     *
     * Returns the parent node, or a nullptr if there is no parent.
     */
    node_type* GetParent()
    {
        return m_IsRootNode ? nullptr : static_cast< node_type* >(m_parent);
    }


    /**
     * @brief GetAABB
     * @return
     *
     * Returns the bounding box of the node.
     */
    const aabb_type & GetAABB() const
    {
        return m_aabb;
    }




    int GetClosestOctant( const vec_type  & p)
    {
        int i=0;
        auto c = m_aabb.Centre();
        return ((p.x > c.x) << 2) | ((p.y > c.y) << 1) | ((p.z > c.z) );
    }


    void ForAllInFrustum( const gla::Frustum & F, const std::function<void(object&)> & f)
    {
        for(auto & ob : m_Obj)
        {
            if( F.Intersects( GetBoundingBox( ob ) ) )
            {
                f(ob);
            }
        }

        for(auto & child : m_Children)
        {
            if( F.Intersects( child.GetAABB() ) )
            {
                child.ForAllInFrustum(F, f);
            }
        }
    }

    /**
     * @brief Merge
     *
     * Collapses all child nodes, all objects will now be placed in the collapsed node.
     */
    void Merge()
    {
        if( m_Children.size() == 0 ) return;

        auto current_size = m_Obj.size();

        for(auto & c : m_Children)
            c.merge( m_Obj );


        for(int i=current_size;i < m_Obj.size() ; i++)
            SetNode( m_Obj[i], this);

        m_Children.clear();
    }



    bool Contains( const aabb_type & bb) const
    {
        return m_aabb.Contains(bb);
    }

    // Splits the node into 8 subnodes and reallocates the object to their appropriate child node.
    void Split()
    {

        CreateChildren(); // Creates the children if it hasn't already been created

        //std::cout << "Splitting" << std::endl;
        // Move all the object to a new vector.
        std::vector<object> current = std::move( m_Obj );

        // Loop through all the object that are possibly able to fit
        // in the children and try to reallocate them to their new nodes.
        for(int j = 0; j < current.size(); j++ )
        {
            auto obj  = std::move(current[j]);
            auto bb   = GetBoundingBox( obj );
            bool fits = false;

            for( auto & ChildNode : m_Children )
            {
                //auto & ChildBB = ChildNode.m_aabb;

                if( ChildNode.Contains( bb ) ) // If the object can fit into this child
                {
                    // Insert the object into this child.
                    ChildNode.Insert(obj);

                    fits = true;						  // Set the flag so that we know it was placed into a child node.

                    break;
                }
            }

            if( !fits )   // If the object doesn't fit into any of the children, it needs to be added back into the parent node.
            {
                SetNode( obj, this );
                m_Obj.push_back( std::move( obj ) );
            }

        }

    }

    // Inserts an object into the Node, returns a pointer to the node it is in.
    node_type*  Insert( object & obj)
    {

        ++m_Totalobject;

        m_Obj.push_back( obj );                 // Place the object into the list
        SetNode( obj, this );            // Tell the object which node it belongs to

        if( m_Obj.size() > Maxobject )          // If we have too many object in the node
        {
            Split();                            //    attempt to split the node
        }
        else
        {
        }

        return GetNode(obj);             // return the new node that the object is in. This could have changed when the node split
    }


    /**
     * @brief Remove
     * @param obj
     * @param DeepRemove
     * @return
     *
     * Removes the object from the Octree. If DeepRemove is set to true
     * it will traverse to the child nodes and look to remove teh object
     * from the children.
     */
    bool Remove( object & obj, bool DeepRemove = false)
    {
        for(int i=0;i<m_Obj.size();i++)
        {
            if( m_Obj[i] == obj )
            {
                std::swap( m_Obj[i], m_Obj.back() );
                m_Obj.resize( m_Obj.size()-1);
                --m_Totalobject;
                return true;
            }
        }

        if( DeepRemove)
        {
            for(auto & c : m_Children)
            {
                if( c.Remove(obj, DeepRemove) )
                {
                    --m_Totalobject;
                    return true;
                }
            }
        }

        return false;
    }




    /**
     * @brief Update
     * @param obj
     * @return
     *
     * Updates an object that has moved.
     */
    static bool Update( object & obj )
    {
        node_type * node = GetNode(obj);

        if( !node ) return false;

        auto bb = GetBoundingBox(obj);

        // if the object is still contained within its node, we can ignore this.
        if( node->Contains( bb ) )
        {
            return false;  // Does not need to be updated.
        }

        // We need to remove the object from this node, and add it to it's parent
        assert( node->Remove(obj) == true);

        // Find the smallest parent that will contain the entire object
        auto OldNode = node;

        // If the node is a root node, it needs to be added to the OctreeScene so a new
        // root node can be created.
        if( node->IsRoot() )
        {
            exit(1);
            //static_cast< OctreeScene<object>*>(node->m_parent)->Insert( obj );
        }
        else
        {
            static_cast< node_type*>(node->m_parent)->__InsertOrGiveToParent(obj, bb );
        }

        return true;
    }


    void print(int spaces = 0, int level = 0, int index = 0 )
    {

        if( m_Totalobject == 0 ) return;

        std::cout << std::string(spaces+1, '-' ) << index << "(" << m_Totalobject << ")" << ":   " << m_Obj.size() << std::endl;

        for(int i=0;i<m_Children.size();i++)
        {
            m_Children[i].print(spaces+3, level+1, i);
        }

    }

    std::size_t DeepCount()
    {
        std::size_t me = m_Obj.size();

        for(auto & c : m_Children)
        {
            me += c.DeepCount();
        }

        return me;
    }

    std::size_t ObjectCount() const
    {
        return m_Obj.size();
    }

    std::size_t count() const
    {
        return m_Totalobject;
    }



    std::vector< node_type > & GetChildNodes()
    {
        return m_Children;
    }

    std::vector<object> & Getobject()
    {
        return m_Obj;
    }

    std::size_t count( const aabb_type & volume )
    {
        size_t c = 0;
        for(int i=0; i < m_Children.size() ; i++ )
        {
            if( m_Children[i].m_aabb.Intersects(volume) )
            {
                c += m_Children[i].count(volume);
            }
        }

        for(int i=0;i<m_Obj.size();i++)
        {
            c += GetBoundingBox(m_Obj[i]).Intersects(volume) ? 1 : 0;
        }
        return c;
    }

    bool HasChildNodes() { return m_Children.size()!=0; }

private:
    /**
     * @brief __InsertOrGiveToParent
     * @param obj
     * @param obj_bb
     * @return
     *
     * Attemps to insert the object into the node, if it cannot fit
     * in this node, it will pass the object to it's parent.
     */
    node_type* __InsertOrGiveToParent( object & obj, const aabb_type & obj_bb)
    {
        // first check if it can it into any of the children
        for(auto & child : m_Children)
        {
            if( child.Contains(obj_bb) )
            {
                child.m_Obj.push_back(obj);
                SetNode(obj, &child);

                child.m_Totalobject++;

                if( child.m_Obj.size() > Maxobject)
                    child.Split();
                return GetNode( obj);
            }
        }

        // if it cant fit into any children, check if it can fit into itself
        if( Contains(obj_bb) )
        {
            m_Obj.push_back(obj);
            SetNode(obj, this);
            if( m_Obj.size() > Maxobject)          // If we have too many object in the node
                Split();                            //    attempt to split the node
            return GetNode( obj);
        }


        // if the object cant fit into this node, then try to give it to the parent.
        if( m_IsRootNode )
        {
            exit(1);
            //return static_cast< scene_type* >(m_parent)->Insert( obj );
        }
        else
        {
            return static_cast< node_type* >(m_parent)->__InsertOrGiveToParent( obj , obj_bb);
        }

    }

    /**
     * @brief merge
     * @param d
     * Merges all objects into the new vector, but does not set the object's node.
     */
    void merge( std::vector<object> & d)
    {
        auto it = d.insert(
            d.end(),
            std::make_move_iterator(m_Obj.begin()),
            std::make_move_iterator(m_Obj.end())
          );

       for(auto & c : m_Children)
       {
           c.merge( d );
       }

    }

    /**
     * @brief CreateChildren
     * @return
     *
     * Creates 8 child nodes, or returns false if the nodes are already created.
     */
    bool CreateChildren()
    {
        if( m_Children.size() ) return false;

        dim_type size = ( m_aabb.max.x  -  m_aabb.min.x ) * static_cast<dim_type>(0.5);

        m_Children.resize( 8 );

        const vec_type Node_Offsets[] = {
                                            vec_type(0,0,0),
                                            vec_type(0,0,1),
                                            vec_type(0,1,0),
                                            vec_type(0,1,1),
                                            vec_type(1,0,0),
                                            vec_type(1,0,1),
                                            vec_type(1,1,0),
                                            vec_type(1,1,1)
                                        };

        for( int i = 0 ; i < 8 ; i++ )
        {
            m_Children[i].m_aabb.min   = m_aabb.min + Node_Offsets[i]*size;
            m_Children[i].m_aabb.max   = m_Children[i].m_aabb.min + vec_type(size);

            m_Children[i].m_parent     = this;
            m_Children[i].m_IsRootNode = false;

            m_Children[i].m_aabb = m_Children[i].m_aabb * static_cast<dim_type>(NODE_EXPANSION_COEF);
        }



        return true;
    }

};

#if 0
template<typename object>
inline void OctreeScene<object>::print()
{
    m_Root.print();
}

template<typename object>
inline std::size_t OctreeScene<object>::count(const aabb_type &volume ) { return m_Root.count(volume); }

template<typename object>
inline std::size_t OctreeScene<object>::count() { return m_Root.count(); }

template<typename object>
inline node_type*  OctreeScene<object>::Insert( object & obj)
{
    auto bb = GetBoundingBox(obj);
    auto RootNode = GetRootNode(  bb.Centre() );

    //assert( RootNode->GetAABB().Contains(bb) );

    RootNode->Insert(obj);



    return NodeType::GetNode(obj);

}

template<typename object>
inline typename OctreeScene<object>::NodeType* OctreeScene<object>::GetRootNode() { return &m_Root; }

template<typename object>
inline void OctreeScene<object>::Optimize()
{
    for(auto & i : m_RootNodes)
    {
        i.second->Merge();
        i.second->Split();
    }
}

template<typename object>
inline void OctreeScene<object>::Remove( object & obj)
{
    NodeType * Node = NodeType::GetNode( obj );

    Node->Remove(obj, true);
}

template<typename object>
inline bool OctreeScene<object>::Update( object & obj )
{
    return NodeType::Update(obj);
}


template<typename object>
inline typename OctreeScene<object>::NodeType*  OctreeScene<object>::GetRootNode(const glm::ivec3 & id)
{
    auto it = m_RootNodes.find(id);

    if( it != m_RootNodes.end() )
    {
        return it->second;
    }


    //std::cout << "Root Create: " << id << std::endl;
    NodeType    * N  = new NodeType( (float)RootNodeSize );
    N->m_parent      = (void*)this;
    N->m_IsRootNode  = true;
    N->m_aabb        = N->m_aabb * 1.2f;
    N->m_aabb       += vec_type((float)RootNodeSize*2.0f) * vec_type(id);

    m_RootNodes[ id ] = N;

    return N;
}


template<typename object>
inline typename OctreeScene<object>::NodeType*  OctreeScene<object>::GetRootNode(const vec_type & position)
{

    auto p = vec_type( position / (float(RootNodeSize) ) );

    glm::ivec3   id;

    for(int i=0;i<3;i++)
    {
        bool neg = p[i] > 0;

        id[i] = std::round( p[i] / 2) ;

    }

    return GetRootNode(id);

}



template<typename object>
OctreeScene<object>::OctreeScene()
{

    ///GetRootNode( glm::ivec3(0,0,0) );
    ///GetRootNode( glm::ivec3(0,0,1) );
    ///GetRootNode( glm::ivec3(0,0,2) );
    ///GetRootNode( glm::ivec3(0,0,3) );

}
#endif

}


#endif


