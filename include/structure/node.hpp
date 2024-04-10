#pragma once

#include <string>

namespace xgraph {
class MyNode {
 public:
  explicit MyNode(std::string name) : _id(ID++), _name(std::move(name)) {}

  MyNode(const MyNode& other) : _id(ID++), _name(other._name) {}

  MyNode& operator=(const MyNode& other) = delete;

  ~MyNode() { ID--; }

  [[nodiscard]] std::size_t Id() const { return _id; }

  [[nodiscard]] std::string Name() const { return _name; }

  bool operator==(const MyNode &other) const { return _id == other.Id(); }

 protected:
  inline static std::size_t ID = 0;

 private:
  /*!
   * @brief Node's identification.
   *
   */
  const std::size_t _id;

  /*!
   * @brief Node's name.
   *
   */
  const std::string _name;

  // You can add your own data here.
};
}  // namespace xgraph
