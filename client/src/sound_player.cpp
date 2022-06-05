#include "../include/sound_player.h"
#include <filesystem>

namespace war_of_ages::client {
sound_player &sound_player::instance() noexcept {
    static sound_player inst;
    return inst;
}

sound_player::sound_player() {
    for (int i = 0; i < static_cast<int>(sound_type::ENUM_END); i++) {
        auto cur_sound = static_cast<sound_type>(i);
        namespace fs = std::filesystem;
        auto it = fs::directory_iterator("../client/resources/sounds/music/");
        auto sound_file =
            std::find_if(fs::begin(it), fs::end(it), [cur_sound](const fs::directory_entry &entry) {
                return entry.path().filename().replace_extension().string() ==
                       to_string(cur_sound) + "_music";
            });
        if (sound_file == fs::end(it) || !sounds[cur_sound].openFromFile(sound_file->path().string())) {
            // TODO: handle this case somehow
        }
        sounds[cur_sound].setVolume(30);
        if (cur_sound == sound_type::BATTLE || cur_sound == sound_type::LOBBY)
            sounds[cur_sound].setLoop(true);
    }
    sounds[sound_type::LOBBY].play();
}

sf::SoundSource::Status sound_player::status(sound_player::sound_type type) {
    return sounds.at(type).getStatus();
}

void sound_player::change(sound_player::sound_type from, sound_player::sound_type to) {
    sounds.at(from).stop();
    sounds.at(to).play();
}

void sound_player::play(sound_type type) {
    sounds.at(type).play();
}

void sound_player::set_volume(sound_player::sound_type type, float new_volume) {
    sounds.at(type).setVolume(new_volume);
}

float sound_player::get_volume(sound_player::sound_type type) {
    return sounds.at(type).getVolume();
}

std::string sound_player::to_string(sound_type type) {
    switch (type) {
        case sound_type::BATTLE:
            return "battle";
        case sound_type::LOBBY:
            return "lobby";
        default:
            return "";
    }
}
}  // namespace war_of_ages::client
