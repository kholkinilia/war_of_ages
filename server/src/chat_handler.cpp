#include "chat_handler.h"
#include <iostream>

// "A lot of copypaste gives u extra lines in GitHub stats" (c) Sun Tzu "The Art of War"

namespace war_of_ages::server {

void chat_handler::add_member(const std::string &chat_id, const std::string &handle) {
    std::unique_lock lock(m_mutex);
    if (m_chat.find(chat_id) == m_chat.end()) {
        std::cerr << "No such chat, moron" << std::endl;
        return;
    }
    m_chat.at(chat_id).add_member(handle);
}

void chat_handler::add_message(const std::string &chat_id,
                               const std::string &handle,
                               const std::string &content) {
    std::unique_lock lock(m_mutex);
    if (m_chat.find(chat_id) == m_chat.end()) {
        std::cerr << "No such chat, moron" << std::endl;
        return;
    }
    m_chat.at(chat_id).add_message(handle, content);
}

void chat_handler::add_system_message(const std::string &chat_id,
                                      const std::string &system_name,
                                      const std::string &content) {
    std::unique_lock lock(m_mutex);
    if (m_chat.find(chat_id) == m_chat.end()) {
        std::cerr << "No such chat, moron" << std::endl;
        return;
    }
    m_chat.at(chat_id).add_system_message(system_name, content);
}

void chat_handler::remove_member(const std::string &chat_id, const std::string &handle) {
    std::unique_lock lock(m_mutex);
    if (m_chat.find(chat_id) == m_chat.end()) {
        std::cerr << "No such chat, moron" << std::endl;
        return;
    }
    m_chat.at(chat_id).remove_member(handle);
}

void chat_handler::create_chat(const std::string &chat_id) {
    std::unique_lock lock(m_mutex);

    m_chat.insert({chat_id, chat(chat_id)});
}

chat_handler &chat_handler::instance() {
    static chat_handler handler;
    return handler;
}

}  // namespace war_of_ages::server