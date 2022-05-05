#ifndef WAR_OF_AGES_THREAD_SAFE_DEQUE_H
#define WAR_OF_AGES_THREAD_SAFE_DEQUE_H

#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>

namespace war_of_ages {

template <typename T>
struct ts_deque {
private:
    std::deque<T> m_deque;
    mutable std::mutex m_mutex;
    std::condition_variable m_variable_blocking;
    mutable std::mutex m_mutex_blocking;

public:
    ts_deque() = default;

    ts_deque &operator=(const ts_deque &) = delete;
    ts_deque &operator=(ts_deque &&) noexcept = default;
    ts_deque(const ts_deque &) = delete;
    ts_deque(ts_deque &&) noexcept = default;

    ~ts_deque() = default;

    [[nodiscard]] std::size_t size() const {
        std::unique_lock lock(m_mutex);
        return m_deque.size();
    }

    [[nodiscard]] bool empty() const {
        std::unique_lock lock(m_mutex);
        return m_deque.empty();
    }

    [[nodiscard]] const T &front() const {
        std::unique_lock lock(m_mutex);
        return m_deque.front();
    }

    [[nodiscard]] const T &back() const {
        std::unique_lock lock(m_mutex);
        return m_deque.back();
    }

    void push_front(const T &val) & {
        std::unique_lock lock(m_mutex);
        m_deque.push_front(val);
        std::unique_lock b_lock(m_mutex_blocking);
        m_variable_blocking.notify_one();
    }

    void push_front(T &&val) & {
        std::unique_lock lock(m_mutex);
        m_deque.push_back(std::move(val));
        std::unique_lock b_lock(m_mutex_blocking);
        m_variable_blocking.notify_one();
    }

    void push_back(const T &val) & {
        std::unique_lock lock(m_mutex);
        m_deque.push_back(val);
        std::unique_lock b_lock(m_mutex_blocking);
        m_variable_blocking.notify_one();
    }

    void push_back(T &&val) & {
        std::unique_lock lock(m_mutex);
        m_deque.push_back(std::move(val));
        std::unique_lock b_lock(m_mutex_blocking);
        m_variable_blocking.notify_one();
    }

    T pop_front() & {
        std::unique_lock lock(m_mutex);
        auto res = m_deque.front();
        m_deque.pop_front();
        return res;
    }

    T pop_back() & {
        std::unique_lock lock(m_mutex);
        auto res = m_deque.back();
        m_deque.pop_back();
        return res;
    }

    void clear() {
        std::unique_lock lock(m_mutex);
        m_deque.clear();
    }

    void wait() {
        while (empty()) {
            std::unique_lock lock(m_mutex_blocking);
            m_variable_blocking.wait(lock);
        }
    }
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_THREAD_SAFE_DEQUE_H
