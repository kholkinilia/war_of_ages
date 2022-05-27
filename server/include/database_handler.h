#ifndef SANDBOX_DATABASE_HANDLER_H
#define SANDBOX_DATABASE_HANDLER_H

#include "sqlite3.h"
#include <string>
#include <utility>
#include <vector>

const int INITIAL_RATING = 100;

struct database_handler {
public:
    enum class Result {
        LOSE, WIN
    };

    struct Statistics_game {
        std::string opponent_handle;
        int opponent_rating, rating;
        Result result;

        Statistics_game(std::string handle, int rating_, int opponent_rating_, Result res) : opponent_handle(
                std::move(handle)), opponent_rating(opponent_rating_), rating(rating_), result(res) {};
    };

    bool sign_in(const std::string& handle, const std::string& password);

    bool registration(const std::string& handle, const std::string& password);

    void add_game(Result result, const std::string &handle1, const std::string &handle2);

    int get_rating(const std::string &handle);

    std::vector<database_handler::Statistics_game> get_statistics(const std::string &handle);

    [[nodiscard]] static database_handler &get_instance();

    database_handler(const database_handler &) = delete;

    database_handler(database_handler &&) = default;

    database_handler &operator=(const database_handler &) = delete;

    database_handler &operator=(database_handler &&) = delete;

    ~database_handler() = default;

private:
    const std::string database_name = "database.db";
    const int SALT_LENGTH = 50;


    database_handler();

    void update_rating(const std::string &handle, Result res);

    static std::size_t hash_with_salt(const std::string& password, const std::string &salt);

    std::string generate_salt() const;

    static void create_database(sqlite3 *database, const std::string &query);

    sqlite3 *open();
};


#endif //SANDBOX_DATABASE_HANDLER_H
