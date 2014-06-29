#ifndef GRAPH_H
#define GRAPH_H

// Boost.Graph stuff
#include <boost/random.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/core_numbers.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/graph/kamada_kawai_spring_layout.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/topology.hpp>

// Local node and edge types
#include "nodeitem.h"
#include "edgeitem.h"


// General typedefs
typedef boost::rand48 RandomNumberGenerator;
typedef boost::rectangle_topology<RandomNumberGenerator> Topology;
typedef Topology::point TopologicalPoint;

// Node meta-data
struct NodeProperties
{
    TopologicalPoint pos;
    NodeItem*   item;

    NodeProperties(NodeItem* item_ = NULL)
        : item(item_)
    {
        // Initialise the point
        pos[0] = 0.0f;
        pos[1] = 0.0f;
    }
};

// Edge meta-data
struct EdgeProperties
{
    float       weight;
    EdgeItem*   item;

    EdgeProperties(EdgeItem* item_ = NULL, float weight_ = 0.0f)
        : weight(weight_)
        , item(item_)
    {
    }
};


// Graph types
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    NodeProperties, EdgeProperties, boost::no_property, boost::listS>
        Graph;
typedef Graph::vertex_descriptor GraphVertex;
typedef Graph::vertex_iterator   GraphVertexIterator;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    NodeProperties, EdgeProperties, boost::no_property, boost::listS>
        Digraph;
typedef Digraph::vertex_descriptor DigraphVertex;
typedef Digraph::vertex_iterator   DigraphVertexIterator;

typedef boost::erdos_renyi_iterator<RandomNumberGenerator, Graph> RandomIterator;


#endif  /* !GRAPH_H */
