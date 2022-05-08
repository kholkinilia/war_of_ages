#ifndef WAR_OF_AGES_ACTIONS_RECIEVER_H
#define WAR_OF_AGES_ACTIONS_RECIEVER_H

#include <memory>
#include <vector>
#include "../../common/game_logic/include/game_commands.h"

namespace war_of_ages {

struct actions_supplier {
protected:
    std::mutex m_mutex;

public:
    [[nodiscard]] virtual std::vector<std::unique_ptr<game_command>> pop_actions() = 0;
    virtual void add_action(std::unique_ptr<game_command> cmd) = 0;
    virtual void clear_actions() = 0;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_ACTIONS_RECIEVER_H
