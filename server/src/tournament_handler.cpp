#include "tournament_handler.h"
#include <random>
#include "chat_handler.h"
#include "message.h"
#include "messages_type.h"
#include "server.h"

namespace war_of_ages::server {

void tournament_handler::create(const std::string &handle, const std::string &tournament_name) {
    std::unique_lock lock(m_mutex);
    std::string key = gen_key();

    chat_handler::instance().create_chat(key);
    chat_handler::instance().add_member(key, handle);

    server_tournament &t = m_tournament[key];
    m_key_by_handle[handle] = key;
    t.set_key(key);
    t.set_name(tournament_name);
    t.add_participant(handle);
}

void tournament_handler::join(const std::string &handle, const std::string &key) {
    std::unique_lock lock(m_mutex);

    if (m_tournament.count(key)) {
        chat_handler::instance().add_member(key, handle);

        m_tournament[key].add_participant(handle);
        m_key_by_handle[handle] = key;
    }
}

void tournament_handler::leave(const std::string &handle) {
    std::unique_lock lock(m_mutex);

    if (m_key_by_handle.count(handle) == 0) {
        return;
    }

    chat_handler::instance().remove_member(m_key_by_handle[handle], handle);
    m_tournament[m_key_by_handle[handle]].remove_participant(handle);

    m_key_by_handle.erase(handle);
}

tournament_handler &tournament_handler::instance() {
    static tournament_handler handler;
    return handler;
}

std::string tournament_handler::gen_key() {
    const static std::size_t KEY_LENGTH = 20;
    static std::mt19937_64 rnd(clock());
    const static std::string characters = "qwertyuiopasdfghjklzxcvbnm1234567890QWERTYUIOPASDFGHJKLZXCVBNM";
    std::string res;
    for (int i = 0; i < KEY_LENGTH; i++) {
        res += characters[rnd() % characters.size()];
    }
    return res;
}

void tournament_handler::add_result(const std::string &winner, const std::string &loser) {
    std::unique_lock lock(m_mutex);
    m_tournament[m_key_by_handle[winner]].add_result(winner, loser);
}

}  // namespace war_of_ages::server
