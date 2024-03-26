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
  const std::size_t _id;
  const std::string _name;
};

std::size_t MyNode::ID = 0;

class MyMatrixNode {};
} // namespace xgraph
