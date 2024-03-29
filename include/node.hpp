#pragma once

#include <string>

namespace xgraph {
class MyNode {
 public:
  explicit MyNode(std::string name) : _id(ID++), _name(std::move(name)) {}

  ~MyNode() { ID--; }

  [[nodiscard]] std::size_t Id() const { return _id; }

  [[nodiscard]] std::string Name() const { return _name; }

  bool operator==(const MyNode &other) const { return _id == other.Id(); }

 protected:
  static std::size_t ID;

 private:
  /*!
   * @brief
   *
   */
  const std::size_t _id;

  /*!
   * @brief
   *
   */
  const std::string _name;

  // You can add your own data here.
};

std::size_t MyNode::ID = 0;
} // namespace xgraph
