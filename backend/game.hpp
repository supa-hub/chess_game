#ifndef CHESS_GAME
#define CHESS_GAME

#include <stdint.h>
#include <vector>
#include <memory>
#include <map>

#include "chess_piece.hpp"
#include "square.hpp"
#include "helper_tools.hpp"
#include "board.hpp"

/*
 We'll use this class more in the future to manage multiple 
 windows with chess games at once.
*/


class Game 
{
    private:
        std::vector< std::shared_ptr<Board> > all_games;

    public:
    
        size_t active_games_count() noexcept { return all_games.size(); }


        std::weak_ptr<Board> new_game() noexcept
        {
            all_games.push_back( std::make_shared<Board>() );

            return all_games.back();
        }


        std::weak_ptr<Board> get_game( size_t game_index ) noexcept
        {
            if ( game_index < active_games_count() && game_index >= 0 ) {
                return all_games[game_index];
            }

            else {
                return std::weak_ptr<Board>();
            }
        }


        bool end_game( size_t game_index ) noexcept
        {
            if ( game_index < active_games_count() && game_index >= 0 ) {
                all_games.erase(all_games.begin() + game_index);
                return true;
            }

            else {
                return false;
            }
        }

        


};

#endif