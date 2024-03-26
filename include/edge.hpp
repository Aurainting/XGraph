#pragma once

#include "node.hpp"
#include "type_concepts.hpp"
#include <memory>

namespace xgraph {
template <NodeType Node = MyNode> class MyEdge {
public:
  MyEdge(const std::shared_ptr<Node> &source,
         const std::shared_ptr<Node> &target)
      : _source(std::weak_ptr<Node>(source)),
        _target(std::weak_ptr<Node>(target)), _weight(1.0) {
    // Nothing to do here.
  }

  MyEdge(const std::shared_ptr<Node> &source,
         const std::shared_ptr<Node> &target, double weight)
      : _source(std::weak_ptr<Node>(source)),
        _target(std::weak_ptr<Node>(target)), _weight(weight) {
    // Nothing to do here.
  }

  std::shared_ptr<Node> Source() const { return _source.lock(); }

  std::shared_ptr<Node> Target() const { return _target.lock(); }

  [[nodiscard]] double Weight() const { return _weight; }

  bool operator==(const MyEdge &other) const {
    return _source.lock() == other.Source() && _target.lock() == other.Target();
  }

private:
  /*!
   * @brief
   *
   */
  const std::weak_ptr<Node> _source;

  /*!
   * @brief
   *
   */
  const std::weak_ptr<Node> _target;

  /*!
   * @brief
   *
   */
  double _weight;

  // You can add your own data here.
};

class MyMatrixEdge {};
} // namespace xgraph
