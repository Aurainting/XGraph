#pragma once

#include <functional>
#include <string>
#include <utility>

#include "type_concepts.hpp"

namespace xgraph {
/*!
 * @brief Empty Object
 */
struct EmptyObject {};

/*!
 * @brief Default node class
 * @tparam NodeData Node data
 */
template <UserDataType NodeData = EmptyObject>
class XNode {
 public:
  /*!
   * @brief Explicit constructor of `XNode`
   * @param id Node identification
   * @param name Node name
   */
  explicit XNode(const std::size_t& id, std::string name)
      : _id(id), _name(std::move(name)), _node_data() {}

  /*!
   * @brief Explicit constructor of `XNode`
   * @param id Node identification
   * @param name Node name
   * @param user_data User node data (copy constructible)
   */
  explicit XNode(const std::size_t& id, std::string name,
                 const NodeData& user_data)
    requires std::is_copy_constructible_v<NodeData>
      : _id(id), _name(std::move(name)), _node_data(user_data) {}

  /*!
   * @brief Explicit constructor of `XNode`
   * @param id Node identification
   * @param name Node name
   * @param user_data User node data (move constructible)
   */
  explicit XNode(const std::size_t& id, std::string name, NodeData&& user_data)
    requires std::is_move_constructible_v<NodeData>
      : _id(id), _name(std::move(name)), _node_data(std::move(user_data)) {}

  /*!
   * @brief Explicit constructor of `XNode`
   * @param id Node identification
   */
  explicit XNode(const std::size_t& id)
      : _id(id), _name(std::to_string(id)), _node_data() {}

  /*!
   * @brief Explicit constructor of `XNode`
   * @param id Node identification
   * @param user_data User node data (copy constructible)
   */
  explicit XNode(const std::size_t& id, const NodeData& user_data)
    requires std::is_copy_constructible_v<NodeData>
      : _id(id), _name(std::to_string(id)), _node_data(user_data) {}

  /*!
   * @brief Explicit constructor of `XNode`
   * @param id Node identification
   * @param user_data User node data (move constructible)
   */
  explicit XNode(const std::size_t& id, NodeData&& user_data)
    requires std::is_move_constructible_v<NodeData>
      : _id(id), _name(std::to_string(id)), _node_data(std::move(user_data)) {}

  /*!
   * @brief Explicit constructor of `XNode`
   * @param name Node name (source of node id)
   */
  explicit XNode(const std::string& name)
      : _id(std::hash<std::string>{}(name)), _name(name), _node_data() {}

  /*!
   * @brief Explicit constructor of `XNode`
   * @param name Node name (source of node id)
   * @param user_data User node data (copy constructible)
   */
  explicit XNode(const std::string& name, const NodeData& user_data)
    requires std::is_copy_constructible_v<NodeData>
      : _id(std::hash<std::string>{}(name)),
        _name(name),
        _node_data(user_data) {}

  /*!
   * @brief Explicit constructor of `XNode`
   * @param name Node name (source of node id)
   * @param user_data User node data (move constructible)
   */
  explicit XNode(const std::string& name, NodeData&& user_data)
    requires std::is_move_constructible_v<NodeData>
      : _id(std::hash<std::string>{}(name)),
        _name(name),
        _node_data(std::move(user_data)) {}

  /*!
   * @brief Deleted copy constructor
   * @param other Other node to be copied
   */
  XNode(const XNode& other) = delete;

  XNode(XNode&& other) = delete;

  /*!
   * @brief Deleted copy assignment constructor
   * @param other Other node to be copied
   * @return
   */
  XNode& operator=(const XNode& other) = delete;

  XNode& operator=(XNode&& other) = delete;

  /*!
   * @brief Default destructor
   */
  ~XNode() = default;

  /*!
   * @brief Get const node id
   * @return Node id
   */
  [[nodiscard]] std::size_t Id() const { return _id; }

  /*!
   * @brief Get const node name
   * @return Node name
   */
  [[nodiscard]] std::string Name() const { return _name; }

  /*!
   * @brief Get const node data
   * @return Node data
   */
  NodeData Data() const { return _node_data; }

  /*!
   * @brief Get reference to node data
   * @return Node data
   */
  NodeData& Data() { return _node_data; }

  /*!
   * @brief Two nodes are equal iff ids are equal
   * @param other Other node
   * @return Boolean
   */
  bool operator==(const XNode& other) const { return _id == other._id; }

 private:
  //! @brief Node identification
  const std::size_t _id;

  //! @brief Node name
  const std::string _name;

  //! @brief Node data
  NodeData _node_data{};
};
}  // namespace xgraph
