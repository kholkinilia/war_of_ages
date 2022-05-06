#include "../include/tournament_handler.h"
#include <random>
#include "../../common/include/messages_type.h"
#include "../../common/network/include/message.h"

namespace war_of_ages {

void tournament_handler::create(const std::string &handle, const std::string &tournament_name) {
    std::string key = gen_key();

    tournament &t = m_tournament[key];
    m_key_by_handle[handle] = key;
    t.add_participant(handle);
    t.set_name(tournament_name);
    t.set_key(key);

    message<messages_type> msg;
    msg.header.id = messages_type::TOURNAMENT_CREATE;
    msg.insert_container(key);

    // TODO: send message
}

void tournament_handler::join(const std::string &handle, const std::string &key) {
    if (m_tournament.find(key) == m_tournament.end()) {
        return;  // TODO: send respond
    }
    m_tournament[key].add_participant(handle);
}

void tournament_handler::leave(const std::string &handle) {
    if (m_key_by_handle.find(handle) != m_key_by_handle.end()) {
        return;  // TODO: send respond
    }
    m_tournament[m_key_by_handle[handle]].remove_participant(handle);
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

}  // namespace war_of_ages
