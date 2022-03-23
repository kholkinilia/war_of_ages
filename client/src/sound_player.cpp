#include "../include/sound_player.h"

war_of_ages::sound_player::sound_player() {
    for (int i = 0; i < static_cast<int>(sound_type::ENUM_END); i++) {
        auto cur_sound = static_cast<sound_type>(i);
        if (!sounds[cur_sound].openFromFile("../client/resources/sounds/music/" + to_string(cur_sound) +
                                            "_music.wav")) {
            // TODO: handle this case somehow
        }
        sounds[cur_sound].setVolume(50);
        if (cur_sound == sound_type::BATTLE || cur_sound == sound_type::LOBBY)
            sounds[cur_sound].setLoop(true);
    }
    sounds[sound_type::LOBBY].play();
    return;
}

sf::SoundSource::Status war_of_ages::sound_player::status(war_of_ages::sound_player::sound_type type) {
    return sounds.at(type).getStatus();
}

void war_of_ages::sound_player::change(war_of_ages::sound_player::sound_type from,
                                       war_of_ages::sound_player::sound_type to) {
    sounds.at(from).stop();
    sounds.at(to).play();
}

void war_of_ages::sound_player::play(sound_type type) {
    sounds.at(type).play();
}

void war_of_ages::sound_player::set_volume(sound_player::sound_type type, float new_volume) {
    sounds.at(type).setVolume(new_volume);
}

float war_of_ages::sound_player::get_volume(war_of_ages::sound_player::sound_type type) {
    return sounds.at(type).getStatus() == sf::SoundSource::Status::Playing ? sounds.at(type).getVolume() : 50;
}
std::string war_of_ages::sound_player::to_string(sound_type type) {
    switch (type) {
        case sound_type::BATTLE:
            return "battle";
        case sound_type::LOBBY:
            return "lobby";
        default:
            return "";
    }
}
