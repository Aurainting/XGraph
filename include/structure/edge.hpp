#pragma once

#include <memory>

#include "node.hpp"
#include "type_concepts.hpp"

namespace xgraph {
/*!
 * @brief Default edge class
 * @tparam Node Use `XNode` by default
 * @tparam EdgeData Edge data
 */
template <NodeType Node = XNode<>, UserDataType EdgeData = EmptyObject>
class XEdge {
 public:
  /*!
   * @brief Explicit constructor of `XEdge`
   * @param source Source node pointer
   * @param target Target node pointer
   * @param weight Edge weight (1.0 by default)
   */
  XEdge(const std::weak_ptr<Node>& source, const std::weak_ptr<Node>& target,
        const double weight = 1.0)
      : _source(source), _target(target), _weight(weight), _edge_data() {
    // Nothing to do here.
  }

  /*!
   * @brief Explicit constructor of `XEdge`
   * @param source Source node pointer
   * @param target Target node pointer
   * @param weight Edge weight
   * @param user_data User edge data (copy constructible)
   */
  XEdge(const std::weak_ptr<Node>& source, const std::weak_ptr<Node>& target,
        const double weight, const EdgeData& user_data)
    requires std::is_copy_constructible_v<EdgeData>
      : _source(source),
        _target(target),
        _weight(weight),
        _edge_data(user_data) {
    // Nothing to do here.
  }

  /*!
   * @brief Explicit constructor of `XEdge`
   * @param source Source node pointer
   * @param target Target node pointer
   * @param weight Edge weight
   * @param user_data User edge data (move constructible)
   */
  XEdge(const std::weak_ptr<Node>& source, const std::weak_ptr<Node>& target,
        const double weight, EdgeData&& user_data)
    requires std::is_move_constructible_v<EdgeData>
      : _source(source),
        _target(target),
        _weight(weight),
        _edge_data(std::move(user_data)) {
    // Nothing to do here.
  }

  /*!
   * @brief Deleted copy constructor
   * @param other Other edge to be copied
   */
  XEdge(const XEdge& other) = delete;

  XEdge(XEdge&& other) = delete;

  /*!
   * @brief Deleted copy assignment constructor
   * @param other Other edge to be copied
   * @return
   */
  XEdge& operator=(const XEdge& other) = delete;

  XEdge& operator=(XEdge&& other) = delete;

  /*!
   * @brief Default destructor
   */
  ~XEdge() = default;

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
   * @brief Get const edge weight
   * @return Edge weight
   */
  [[nodiscard]] double Weight() const { return _weight; }

  /*!
   * @brief Get const edge data
   * @return Edge data
   */
  EdgeData Data() const { return _edge_data; }

  /*!
   * @brief Get reference to edge data
   * @return Edge data
   */
  EdgeData& Data() { return _edge_data; }

  /*!
   * @brief Two edges are equal iff source, target and weight are equal
   * @param other Other edge
   * @return Boolean
   */
  bool operator==(const XEdge& other) const {
    return *(_source.lock()) == *(other.Source()) &&
           *(_target.lock()) == *(other.Target()) && _weight == other.Weight();
  }

 private:
  //! @brief Source node without ownership
  const std::weak_ptr<Node> _source;

  //! @brief Target node without ownership
  const std::weak_ptr<Node> _target;

  //! @brief Weight of the edge
  double _weight;

  //! @brief Edge data
  EdgeData _edge_data;
};
}  // namespace xgraph
