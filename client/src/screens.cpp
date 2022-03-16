#include "../include/screens.h"
#include "../include/client.h"
#include "../include/game_object_size_constants.h"
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
                                   ->get("units_group")
                                   ->cast<tgui::Group>();
            units_group->removeAllWidgets();
            state.get_cur_game_state()->update({}, {}, 0.1);
            auto [p1, p2] = state.get_cur_game_state()->snapshot_players();
            tgui::Picture::Ptr unit_picture;
            for (auto unit : p1.units) {
                auto unit_hp = tgui::ProgressBar::create();
                auto unit_hp_renderer = tgui::ProgressBarRenderer();
                unit_hp_renderer.setFillColor({0, 255, 0, 255});
                unit_hp->setRenderer(unit_hp_renderer.getData());

                if (unit.type() == unit_type::STONE_TOWER) {
                    unit_picture = tgui::Picture::create(
                        *(sprite_supplier::get_instance()
                              .get_tower_sprite(age_type::STONE, sprite_supplier::player_side::LEFT)
                              .getTexture()));
                    unit_picture->setSize(TOWER_WIDTH, TOWER_HEIGHT);
                    unit_picture->setPosition(unit.position() + TOWER_WIDTH,
                                              BACKGROUND_HEIGHT - TOWER_HEIGHT - 10);
                    unit_hp->setPosition(unit.position() + TOWER_WIDTH * 0.33,
                                         BACKGROUND_HEIGHT - TOWER_HEIGHT - 60);
                    unit_hp->setSize(TOWER_WIDTH * 0.5, 20);
                } else {
                    unit_picture = tgui::Picture::create(
                        *(sprite_supplier::get_instance()
                              .get_unit_sprite(unit.type(), sprite_supplier::player_side::LEFT)
                              .getTexture()));
                    unit_picture->setSize(100, 100);
                    unit_picture->setPosition(unit.position() + TOWER_WIDTH, BACKGROUND_HEIGHT - 200);
                    unit_hp->setPosition(unit.position() + 100 * 0.33, BACKGROUND_HEIGHT - 200 - 60);
                    unit_hp->setSize(100 * 0.5, 20);
                }

                unit_hp->setValue(unit.remaining_hp());

                units_group->add(unit_hp);
                units_group->add(unit_picture);
            }

            for (auto unit : p2.units) {
                auto unit_hp = tgui::ProgressBar::create();
                auto unit_hp_renderer = tgui::ProgressBarRenderer();
                unit_hp_renderer.setFillColor({255, 0, 0, 255});
                unit_hp->setRenderer(unit_hp_renderer.getData());

                if (unit.type() == unit_type::STONE_TOWER) {
                    unit_picture = tgui::Picture::create(
                        *(sprite_supplier::get_instance()
                              .get_tower_sprite(age_type::STONE, sprite_supplier::player_side::RIGHT)
                              .getTexture()));
                    unit_picture->setSize(TOWER_WIDTH, TOWER_HEIGHT);
                    unit_hp->setSize(TOWER_WIDTH / 2, 20);
                    unit_hp->setPosition(ROAD_WIDTH - unit.position() - TOWER_WIDTH * 0.67,
                                         BACKGROUND_HEIGHT - TOWER_HEIGHT - 60);
                } else {
                    unit_picture = tgui::Picture::create(
                        *(sprite_supplier::get_instance()
                              .get_unit_sprite(unit.type(), sprite_supplier::player_side::LEFT)
                              .getTexture()));
                    unit_picture->setSize(100, 100);
                    unit_picture->setPosition(unit.position(), BACKGROUND_HEIGHT - TOWER_HEIGHT - 10);
                    unit_hp->setSize(TOWER_WIDTH / 2, 20);
                    unit_hp->setPosition(ROAD_WIDTH - unit.position() - TOWER_WIDTH * 0.67,
                                         BACKGROUND_HEIGHT - TOWER_HEIGHT - 60);
                }

                unit_picture->setPosition(ROAD_WIDTH - TOWER_WIDTH - unit.position(),
                                          BACKGROUND_HEIGHT - TOWER_HEIGHT - 10);
                unit_hp->setValue(unit.remaining_hp());

                units_group->add(unit_hp);
                units_group->add(unit_picture);
            }
        }
        default:
            break;
    }
}

}  // namespace war_of_ages
