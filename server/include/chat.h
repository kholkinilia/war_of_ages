#ifndef WAR_OF_AGES_CHAT_H
#define WAR_OF_AGES_CHAT_H

#include <string>
#include <vector>

namespace war_of_ages::server {

struct chat {
private:
    std::string m_id;
    std::vector<std::string> m_members;

public:
    explicit chat(std::string chat_id);

    void add_message(const std::string &handle, const std::string &content);
    void add_system_message(const std::string &system_name, const std::string &content);
    void add_member(const std::string &handle);
    void remove_member(const std::string &handle);
};

}  // namespace war_of_ages::server

#endif  // WAR_OF_AGES_CHAT_H
