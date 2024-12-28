#pragma once

#include <concepts>

namespace xgraph {
/*!
 * @brief Concept specify node type
 * @tparam T
 */
template <typename T>
concept NodeType = requires(T x) {
  { x.Id() } -> std::convertible_to<std::size_t>;
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
concept EdgeType = requires(T x) {
  { x.Source() } -> NodePtrType;
  { x.Target() } -> NodePtrType;
};
}  // namespace xgraph
