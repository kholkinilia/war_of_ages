#include "../include/room_handler.h"
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include "../include/client.h"
#include "../include/screen_handler.h"

namespace war_of_ages::client {

static tgui::Texture ready_pic;
static tgui::Texture not_ready_pic;

room_handler::room_handler() noexcept {
    ready_pic.load("../client/resources/pictures/ready.png");
    not_ready_pic.load("../client/resources/pictures/not_ready.png");
}

room_handler &room_handler::instance() {
    static room_handler inst;
    return inst;
}

void room_handler::update_enemy(std::optional<player_info> enemy) {
    screen_handler::instance()
        .get_gui()
        .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
        ->get<tgui::Picture>("enemy_readiness")
        ->getRenderer()
        ->setTexture(enemy.has_value() && enemy->status == player_status::READY ? ready_pic : not_ready_pic);
    if (!players[1].has_value()) {
        screen_handler::instance()
            .get_gui()
            .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
            ->get<tgui::Label>("enemy_handle")
            ->setText(enemy.has_value() ? enemy->handle : "");
    }
    screen_handler::instance()
        .get_gui()
        .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
        ->get<tgui::Label>("enemy_rate_value")
        ->setText(enemy.has_value() ? std::to_string(enemy->rate) : "");
    players[1] = std::move(enemy);
}

void room_handler::update_me(player_info me) {
    screen_handler::instance()
        .get_gui()
        .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
        ->get<tgui::Picture>("my_readiness")
        ->getRenderer()
        ->setTexture(me.status == player_status::READY ? ready_pic : not_ready_pic);
    screen_handler::instance()
        .get_gui()
        .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
        ->get<tgui::Label>("my_handle")
        ->setText(me.handle);
    screen_handler::instance()
        .get_gui()
        .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
        ->get<tgui::Label>("my_rate_value")
        ->setText(std::to_string(me.rate));
    players[0] = std::move(me);
}

void room_handler::change_enemy_status(player_status status) {
    if (!players[1].has_value()) {
        return;
    }
    players[1]->status = status;
    screen_handler::instance()
        .get_gui()
        .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
        ->get<tgui::Picture>("enemy_readiness")
        ->getRenderer()
        ->setTexture(status == player_status::READY ? ready_pic : not_ready_pic);
}

void room_handler::change_my_status(player_status status) {
    players[0]->status = status;
    screen_handler::instance()
        .get_gui()
        .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
        ->get<tgui::Picture>("enemy_readiness")
        ->getRenderer()
        ->setTexture(status == player_status::READY ? ready_pic : not_ready_pic);
}

void room_handler::switch_enemy_status() {
    if (!players[1].has_value()) {
        return;
    }
    change_enemy_status(players[1]->status == player_status::READY ? player_status::NOT_READY
                                                                   : player_status::READY);
}

void room_handler::switch_my_status() {
    change_my_status(players[0]->status == player_status::READY ? player_status::NOT_READY
                                                                : player_status::READY);
}

}  // namespace war_of_ages::client