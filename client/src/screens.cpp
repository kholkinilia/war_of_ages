#include "../include/screens.h"
#include "../include/client.h"
#include "../include/screen_defines.h"
#include "../include/sprite_supplier.h"

namespace war_of_ages {

void update_widgets(tgui::Gui &gui, const client_state &state) {
    switch (state.get_cur_screen()) {
        case screen::TOURNAMENT_MAIN: {
            state.get_cur_tournament()->update_grid(gui.get(screen_id.at(screen::TOURNAMENT_MAIN))
                                                        ->cast<tgui::Group>()
                                                        ->get("tournament_grid")
                                                        ->cast<tgui::Grid>());
        }
        case screen::GAME_SCREEN: {
            if (state.get_cur_game_state() == nullptr)
                return;
            auto units_group = gui.get(screen_id.at(screen::GAME_SCREEN))
                                   ->cast<tgui::Group>()
                                   ->get("panel")
                                   ->cast<tgui::Panel>()
                                   ->get("panel_group")
                                   ->cast<tgui::Group>()
                                   ->get("units_group")
                                   ->cast<tgui::Group>();
            units_group->removeAllWidgets();
            state.get_cur_game_state()->update({}, {}, 1 /*???*/);
            auto [p1, p2] = state.get_cur_game_state()->snapshot_players();
            tgui::Picture::Ptr unit_picture;
            for (auto unit : p1.units) {
                unit_picture = tgui::Picture::create(
                    *(sprite_supplier::get_instance()
                          .get_unit_sprite(unit.type(), sprite_supplier::player_side::LEFT)
                          .getTexture()));
                unit_picture->setPosition(unit.position(), 100);
                units_group->add(unit_picture);
            }
            for (auto unit : p2.units) {
                unit_picture = tgui::Picture::create(
                    *(sprite_supplier::get_instance()
                          .get_unit_sprite(unit.type(), sprite_supplier::player_side::RIGHT)
                          .getTexture()));
                unit_picture->setPosition(unit.position(), 100);
                units_group->add(unit_picture);
            }
        }
        default:
            break;
    }
}

}  // namespace war_of_ages
