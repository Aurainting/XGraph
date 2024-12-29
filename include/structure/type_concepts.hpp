#pragma once

#include <concepts>

namespace xgraph {
template <typename T>
concept UserDataType =
    std::is_default_constructible_v<T> &&
    (std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>);

/*!
 * @brief Concept specify node type
 * @tparam T
 */
template <typename T>
concept NodeType = requires(T n) {
  { n.Id() } -> std::convertible_to<std::size_t>;
};

/*!
 * @brief Concept specify node pointer type
 * @tparam T
 */
template <typename T>
concept NodePtrType = requires {
  typename T::element_type;
  requires NodeType<typename T::element_type>;
};

/*!
 * @brief Concept specify edge type
 * @tparam T
 */
template <typename T>
concept EdgeType = requires(T e) {
  { e.Source() } -> NodePtrType;
  { e.Target() } -> NodePtrType;
  { e.Weight() } -> std::convertible_to<double>;
};
}  // namespace xgraph
