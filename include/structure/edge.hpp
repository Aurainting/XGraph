#pragma once

#include <memory>

#include "node.hpp"
#include "type_concepts.hpp"

namespace xgraph {
/*!
 * @brief Default edge class
 * @tparam Node Use `MyNode` by default
 */
template<NodeType Node = MyNode>
class MyEdge {
public:
    /*!
     * @brief Explicit constructor of `MyEdge`
     * @param source Source node pointer
     * @param target Target node pointer
     * @param weight Edge weight (1.0 by default)
     */
    MyEdge(const std::shared_ptr<Node> &source,
           const std::shared_ptr<Node> &target, const double weight = 1.0)
        : _source(std::weak_ptr<Node>(source)),
          _target(std::weak_ptr<Node>(target)),
          _weight(weight) {
        // Nothing to do here.
    }

    /*!
     * @brief Deleted copy constructor
     * @param other Other edge to be copied
     */
    MyEdge(const MyEdge &other) = delete;

    /*!
     * @brief Deleted copy assignment constructor
     * @param other Other edge to be copied
     * @return
     */
    MyEdge &operator=(const MyEdge &other) = delete;

    /*!
     * @brief Default destructor
     */
    ~MyEdge() = default;

    /*!
     * @brief Get const source node pointer
     * @return Source node pointer
     */
    std::shared_ptr<Node> Source() const { return _source.lock(); }

    /*!
     * @brief Get const target node pointer
     * @return Target node pointer
     */
    std::shared_ptr<Node> Target() const { return _target.lock(); }

    /*!
     * @brief Get edge weight
     * @return Edge weight
     */
    [[nodiscard]] double Weight() const { return _weight; }

    /*!
     * @brief Two edges are equal iff source, target and weight are equal
     * @param other Other edge
     * @return Boolean
     */
    bool operator==(const MyEdge &other) const {
        return *(_source.lock()) == *(other.Source()) &&
               *(_target.lock()) == *(other.Target()) && _weight == other.Weight();
    }

private:
    //! \brief Source node without ownership
    const std::weak_ptr<Node> _source;

    //! \brief Target node without ownership
    const std::weak_ptr<Node> _target;

    //! \brief Weight of the edge
    double _weight;

    // You can add your own data here.
};
} // namespace xgraph
