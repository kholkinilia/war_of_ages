#include "server_bullet.h"
#include "game_handler.h"
#include "message.h"
#include "messages_type.h"
#include "server.h"

namespace war_of_ages::server {

void server_bullet::post_collision_action() const {
}

void server_bullet::post_create_action(std::string owner_handle) const {
    std::cerr << "Create bullet: " << (int)m_type << std::endl;
    if (std::find(ult_bullet_types.begin(), ult_bullet_types.end(), m_type) != ult_bullet_types.end()) {
        std::cerr << "This is actually an ult bullet: " << owner_handle << std::endl;
        message<messages_type> msg;
        msg.header.id = messages_type::GAME_ADD_BULLET;
        msg << owner_handle << get_snapshot();
        server::instance().send_message(owner_handle, msg);
        server::instance().send_message(game_handler::instance().get_enemy_handle_lock_held(owner_handle),
                                        msg);
    }
}

server_bullet::server_bullet(bullet_type type, const vec2f &start, const vec2f &target)
    : bullet(type, start, target) {
}

std::shared_ptr<bullet> server_bullet_factory(bullet_type type, const vec2f &start, const vec2f &target) {
    return std::make_shared<server_bullet>(type, start, target);
}

}  // namespace war_of_ages::server