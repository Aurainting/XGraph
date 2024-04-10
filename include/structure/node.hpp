#pragma once

#include <string>

namespace xgraph {
class MyNode {
 public:
  explicit MyNode(std::string name)
      : _id(std::hash<std::string>{}(name)),
        _name(std::move(name)) {}

  MyNode(const MyNode& other) : _id(other.Id()), _name(other.Name()) {}

  MyNode& operator=(const MyNode& other) = delete;

  ~MyNode() = default;

  [[nodiscard]] std::size_t Id() const { return _id; }

  [[nodiscard]] std::string Name() const { return _name; }

  bool operator==(const MyNode& other) const {
    return _name == other.Name();
  }

 private:
  /*!
   * @brief Node's identification (come from `name`).
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
