#ifndef WAR_OF_AGES_SOUND_PLAYER_H
#define WAR_OF_AGES_SOUND_PLAYER_H

#include <SFML/Audio/Music.hpp>
#include <string>
#include <unordered_map>

namespace war_of_ages::client {

struct sound_player {
    enum class sound_type { BATTLE, LOBBY, ENUM_END };

public:
    [[nodiscard]] static sound_player &instance() noexcept;

    ~sound_player() = default;
    sound_player(const sound_player &other) = delete;
    sound_player(sound_player &&other) = delete;
    sound_player &operator=(const sound_player &other) = delete;
    sound_player &operator=(sound_player &&other) = delete;

    void change(sound_type from, sound_type to);
    void play(sound_type type);
    void set_volume(sound_type to, float new_volume);
    [[nodiscard]] float get_volume(sound_type to);
    [[nodiscard]] sf::SoundSource::Status status(sound_type type);

private:
    sound_player();

    std::unordered_map<sound_type, sf::Music> sounds;
    [[nodiscard]] static std::string to_string(sound_type type);
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_SOUND_PLAYER_H
