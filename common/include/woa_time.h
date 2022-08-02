#ifndef WAR_OF_AGES_WOA_TIME_H
#define WAR_OF_AGES_WOA_TIME_H

#include <chrono>
#include <cstdint>

constexpr std::uint64_t MS_PER_SEC = 1000;

inline std::uint64_t get_ms() {
    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                          std::chrono::system_clock::now().time_since_epoch())
                                          .count());
}

#endif  // WAR_OF_AGES_WOA_TIME_H
