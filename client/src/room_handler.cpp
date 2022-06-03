#include "../include/room_handler.h"
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include "../include/client.h"
#include "../include/screen_handler.h"

namespace war_of_ages::client {

room_handler &room_handler::instance() {
    static room_handler inst;
    return inst;
}

void room_handler::update_enemy(std::optional<player_info> enemy) {
    auto &theme = screen_handler::instance().get_theme_buttons();
    if (enemy.has_value()) {
        if (!players[1].has_value()) {
            screen_handler::instance()
                .get_gui()
                .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
                ->get<tgui::Group>("enemy_group")
                ->setVisible(true);
            screen_handler::instance()
                .get_gui()
                .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
                ->get<tgui::Label>("enemy_handle")
                ->setText(enemy->handle);
        }
        screen_handler::instance()
            .get_gui()
            .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
            ->get<tgui::Picture>("enemy_readiness")
            ->setRenderer(theme.getRenderer(enemy->status == player_status::READY ? "ready" : "not_ready"));
        screen_handler::instance()
            .get_gui()
            .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
            ->get<tgui::Label>("enemy_rate_value")
            ->setText(std::to_string(enemy->rate));
    } else {
        screen_handler::instance()
            .get_gui()
            .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
            ->get<tgui::Group>("enemy_group")
            ->setVisible(false);
    }
    players[1] = std::move(enemy);
}

void room_handler::update_me(player_info me) {
    auto &theme = screen_handler::instance().get_theme_buttons();
    screen_handler::instance()
        .get_gui()
        .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
        ->get<tgui::Button>("my_readiness")
        ->setRenderer(theme.getRenderer(me.status == player_status::READY ? "ready" : "not_ready"));
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
    auto &theme = screen_handler::instance().get_theme_buttons();
    if (!players[1].has_value()) {
        return;
    }
    players[1]->status = status;
    screen_handler::instance()
        .get_gui()
        .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
        ->get<tgui::Picture>("enemy_readiness")
        ->setRenderer(theme.getRenderer(status == player_status::READY ? "ready" : "not_ready"));
}

void room_handler::change_my_status(player_status status) {
    auto &theme = screen_handler::instance().get_theme_buttons();
    players[0]->status = status;
    screen_handler::instance()
        .get_gui()
        .get<tgui::Group>(screen_handler::screen_id.at(screen_handler::screen_type::ROOM_SCREEN))
        ->get<tgui::Button>("my_readiness")
        ->setRenderer(theme.getRenderer(status == player_status::READY ? "ready" : "not_ready"));
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