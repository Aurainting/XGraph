#pragma once

#include <memory>

#include "node.hpp"
#include "type_concepts.hpp"

namespace xgraph {
template <NodeType Node = MyNode>
class MyEdge {
 public:
  MyEdge(const std::shared_ptr<Node> &source,
         const std::shared_ptr<Node> &target, const double weight = 1.0)
      : _source(std::weak_ptr<Node>(source)),
        _target(std::weak_ptr<Node>(target)),
        _weight(weight) {
    // Nothing to do here.
  }

  MyEdge(const MyEdge &other) = delete;

  MyEdge &operator=(const MyEdge &other) = delete;

  ~MyEdge() = default;

  std::shared_ptr<Node> Source() const { return _source.lock(); }

  std::shared_ptr<Node> Target() const { return _target.lock(); }

  [[nodiscard]] double Weight() const { return _weight; }

  bool operator==(const MyEdge &other) const {
    return *(_source.lock()) == *(other.Source()) &&
           *(_target.lock()) == *(other.Target()) && _weight == other.Weight();
  }

 private:
  /*!
   * @brief Source node.
   *
   */
  const std::weak_ptr<Node> _source;

  /*!
   * @brief Target node.
   *
   */
  const std::weak_ptr<Node> _target;

  /*!
   * @brief Weight of this edge.
   *
   */
  double _weight;

  // You can add your own data here.
};
}  // namespace xgraph
