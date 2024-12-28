#pragma once

#include <string>

namespace xgraph {
/*!
 * @brief Default node class
 */
class MyNode {
public:
    /*!
     * @brief Explicit constructor of `MyNode`
     * @param name Node name (source of ID)
     */
    explicit MyNode(std::string name)
        : _id(std::hash<std::string>{}(name)),
          _name(std::move(name)) {
    }

    /*!
     * @brief Deleted copy constructor
     * @param other Other node to be copied
     */
    MyNode(const MyNode &other) = delete;

    /*!
     * @brief Deleted copy assignment constructor
     * @param other Other node to be copied
     * @return
     */
    MyNode &operator=(const MyNode &other) = delete;

    /*!
     * @brief Default destructor
     */
    ~MyNode() = default;

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
     * @brief Two nodes are equal iff names are equal
     * @param other Other node
     * @return Boolean
     */
    bool operator==(const MyNode &other) const {
        return _name == other.Name();
    }

private:
    //! \brief Node identification (come from `name`)
    const std::size_t _id;

    //! \brief Node name
    const std::string _name;

    // You can add your own data here.
};
} // namespace xgraph
