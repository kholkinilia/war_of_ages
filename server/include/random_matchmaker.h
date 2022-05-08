#ifndef WAR_OF_AGES_RANDOM_MATCHMAKER_H
#define WAR_OF_AGES_RANDOM_MATCHMAKER_H

#include <cstdint>
#include <string>
#include <vector>

namespace war_of_ages::server {
struct random_matchmaker {
    static random_matchmaker &instance();
    ~random_matchmaker() = default;
    random_matchmaker(const random_matchmaker &other) = delete;
    random_matchmaker(random_matchmaker &&other) = delete;
    random_matchmaker &operator=(const random_matchmaker &other) = delete;
    random_matchmaker &operator=(random_matchmaker &&other) = delete;

    bool add_user(std::string handle);
    bool remove_user(const std::string &handle);

private:
    [[nodiscard]] bool user_exists(const std::string &handle) const noexcept;

    random_matchmaker() = default;
    std::vector<std::string> m_users;
};
}  // namespace war_of_ages::server

#endif  // WAR_OF_AGES_RANDOM_MATCHMAKER_H
