#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <condition_variable>
#include <format>
#include <future>
#include <latch>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "xgraph"

using xgraph::XEdge;
using xgraph::XNode;

class ConcurrentCounter {
public:
  ConcurrentCounter() : cnt(0), max_active(0) {}

  void Add() {
    std::lock_guard lock(mtx);
    ++cnt;
    if (cnt > max_active) {
      max_active = cnt;
    }
  }

  void Sub() {
    std::lock_guard lock(mtx);
    --cnt;
  }

  [[nodiscard]] auto MaxActive() const { return max_active; }

private:
  std::size_t cnt;
  std::size_t max_active;
  std::mutex mtx;
};

class Task {
public:
  Task() : x(0), y(0), latch_ptr(nullptr) {}

  Task(const std::size_t x, const std::size_t y)
      : x(x), y(y), latch_ptr(nullptr) {}

  void Run(ConcurrentCounter& counter) const {
    counter.Add();
    std::this_thread::sleep_for(std::chrono::milliseconds(x + y));
    counter.Sub();
  }

  auto& Latch() { return latch_ptr; }

private:
  std::size_t x;
  std::size_t y;
  std::shared_ptr<std::latch> latch_ptr;
};

enum class DependencyStatus : int {
  INACTIVE = -1,
  ACTIVE = 0,
  DONE = 1,
};

class Dependency {
public:
  Dependency() : status(DependencyStatus::INACTIVE) {}

  explicit Dependency(const DependencyStatus s) : status(s) {}

  [[nodiscard]] auto Status() const { return status; }

  auto& Status() { return status; }

private:
  DependencyStatus status;
};

/*!
 * @brief ThreadPool (based on https://github.com/progschj/ThreadPool)
 */
class ThreadPool {
public:
  /*!
   * @brief The constructor just launches some amount of workers
   */
  explicit ThreadPool(const size_t threads) {
    for (size_t i = 0; i < threads; ++i)
      workers.emplace_back([this](const auto& st) {
        for (;;) {
          std::function<void()> task{};
          {
            std::unique_lock lock(queue_mutex);
            condition.wait(lock, st, [&] { return !tasks.empty(); });
            if (st.stop_requested() && tasks.empty()) {
              return;
            }
            task = std::move(tasks.front());
            tasks.pop();
          }
          task();
        }
      });
  }

  /*!
   * @brief Add new work item to the pool
   */
  template <class F, class... Args>
  auto enqueue(F&& f, Args&&... args)
      -> std::future<std::invoke_result_t<F, Args...>> {
    using return_type = std::invoke_result_t<F, Args...>;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        [func = std::forward<F>(f),
         ... captured_args = std::forward<Args>(args)] mutable -> return_type {
          return std::invoke(func, std::move(captured_args)...);
        });

    std::future<return_type> res = task->get_future();
    {
      std::lock_guard lock(queue_mutex);
      tasks.emplace([task]() mutable { (*task)(); });
    }
    condition.notify_one();
    return res;
  }

private:
  // synchronization
  std::mutex queue_mutex;
  std::condition_variable_any condition;

  // task queue
  std::queue<std::function<void()>> tasks;

  // worker threads
  std::vector<std::jthread> workers;
};

TEST_CASE("Wavefront Parallelism", "DiGraph") {
  /*
   * Given a 3*3 grid, the wavefront propagates task dependencies from the
   * top-left block all the way to the bottom-right block. Each task precedes
   * two tasks, one to the right and another below.
   *
   *  (0,0) -> (0,1) -> (0,2)
   *    |        |        |
   *    v        v        v
   *  (1,0) -> (1,1) -> (1,2)
   *    |        |        |
   *    v        v        v
   *  (2,0) -> (2,1) -> (2,2)
   *
   */
  const auto taskflow = std::make_shared<
      xgraph::DiGraph<XNode<Task>, XEdge<XNode<Task>, Dependency>>>();

  // Add tasks
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      taskflow->AddNode(std::format("({}, {})", i, j), Task(i, j));
    }
  }

  // Add dependencies
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      // Right
      if (j < 2) {
        taskflow->AddEdge(std::format("({}, {})", i, j),
                          std::format("({}, {})", i, j + 1), 1.0, Dependency());
      }
      // Down
      if (i < 2) {
        taskflow->AddEdge(std::format("({}, {})", i, j),
                          std::format("({}, {})", i + 1, j), 1.0, Dependency());
      }
    }
  }

  // Get number of threads
  auto num_thread = std::thread::hardware_concurrency();
  if (num_thread == 0) {
    num_thread = 2;
  }

  // Run
  auto counter = ConcurrentCounter();
  {
    auto pool = ThreadPool(num_thread);
    const std::optional<xgraph::NodePtrVisitor_t<XNode<Task>>> visitor =
        [&taskflow, &pool, &counter](const std::shared_ptr<XNode<Task>>& node) {
          node->Data().Latch() =
              std::make_shared<std::latch>(taskflow->InEdgeSize(node->Id()));

          pool.enqueue([&taskflow, node, &counter] {
            node->Data().Latch()->wait();

            for (const auto& out_edge : taskflow->OutEdges(node->Id())) {
              out_edge->Data().Status() = DependencyStatus::ACTIVE;
            }

            node->Data().Run(counter);

            for (const auto& in_edge : taskflow->InEdges(node->Id())) {
              in_edge->Data().Status() = DependencyStatus::DONE;
            }
            for (const auto& out_edge : taskflow->OutEdges(node->Id())) {
              out_edge->Target()->Data().Latch()->count_down();
            }
          });
        };
    xgraph::algorithm::TopologicalSort(*taskflow, visitor);
  }

  // Release latch
  for (const auto& node : taskflow->Nodes()) {
    node->Data().Latch() = nullptr;
  }

  // Check status
  for (const auto& edge : taskflow->Edges()) {
    REQUIRE(edge->Data().Status() == DependencyStatus::DONE);
  }

  // Check parallelism
  if (num_thread > 2) {
    REQUIRE(counter.MaxActive() == 3);
  } else {
    REQUIRE(counter.MaxActive() > 1);
  }
}
