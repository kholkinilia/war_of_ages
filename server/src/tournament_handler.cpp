#include "../include/tournament_handler.h"
#include <random>
#include "../../common/include/messages_type.h"
#include "../../common/network/include/message.h"
#include "../include/server.h"

namespace war_of_ages::server {

void tournament_handler::create(const std::string &handle, const std::string &tournament_name) {
    std::unique_lock lock(m_mutex);
    std::string key = gen_key();

    server_tournament &t = m_tournament[key];
    m_key_by_handle[handle] = key;
    t.set_key(key);
    t.set_name(tournament_name);
    t.add_participant(handle);
}

void tournament_handler::join(const std::string &handle, const std::string &key) {
    std::unique_lock lock(m_mutex);
    if (m_tournament.count(key)) {
        m_tournament[key].add_participant(handle);
        m_key_by_handle[handle] = key;
    }
}

void tournament_handler::leave(const std::string &handle) {
    std::unique_lock lock(m_mutex);
    if (m_key_by_handle.count(handle) == 0) {
        return;
    }
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
//    std::cerr << "Adding the result: (winner: "<< winner << ", loser: " << loser << ")" << std::endl;
//    std::cerr << "Kay of " << winner << ": " << m_key_by_handle[winner] << std::endl;
    m_tournament[m_key_by_handle[winner]].add_result(winner, loser);
}

}  // namespace war_of_ages::server
