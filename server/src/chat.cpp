#include "../include/chat.h"
#include <utility>
#include "../include/server.h"
#include "message.h"
#include "messages_type.h"

namespace war_of_ages::server {

chat::chat(std::string chat_id) : m_id(std::move(chat_id)){};

void chat::add_message(const std::string &handle, const std::string &content) {
    if (m_members.size() == 1) {
        return;
    }

    message<messages_type> msg;
    msg.header.id = messages_type::CHAT_NEW_MESSAGE;
    msg << m_id << handle << content;

    for (const auto &member_handle : m_members) {
        if (member_handle == handle) {
            continue;
        }

        server::instance().send_message(member_handle, msg);
    }
}

void chat::add_system_message(const std::string &system_name, const std::string &content) {
    message<messages_type> msg;
    msg.header.id = messages_type::CHAT_NEW_MESSAGE;
    msg << m_id << system_name << content;

    for (const auto &member_handle : m_members) {
        server::instance().send_message(member_handle, msg);
    }
}

void chat::add_member(const std::string &handle) {
    m_members.push_back(handle);
}

void chat::remove_member(const std::string &handle) {
    m_members.erase(std::find(m_members.begin(), m_members.end(), handle));
}

}  // namespace war_of_ages::server
