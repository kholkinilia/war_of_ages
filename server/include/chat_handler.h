#ifndef WAR_OF_AGES_CHAT_HANDLER_H
#define WAR_OF_AGES_CHAT_HANDLER_H

#include <mutex>
#include <string>
#include <unordered_map>
#include "chat.h"

namespace war_of_ages::server {

struct chat_handler {
private:
    std::unordered_map<std::string, chat> m_chat;
    std::mutex m_mutex;

    chat_handler() = default;

public:
    void create_chat(const std::string &chat_id);

    void add_member(const std::string &chat_id, const std::string &handle);
    void add_message(const std::string &chat_id, const std::string &handle, const std::string &content);
    void remove_member(const std::string &chat_id, const std::string &handle);

    static chat_handler &instance();
};

}  // namespace war_of_ages::server
#endif  // WAR_OF_AGES_CHAT_HANDLER_H
