#ifndef CHESS_GAME
#define CHESS_GAME

#include <stdint.h>
#include <vector>
#include <memory>
#include <map>
#include <string>

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
        std::shared_ptr<Board> current_board; // this will contain the board that we are currently modifying. We need this variable because we can have multiple boards.

    public:
    
        size_t active_games_count() noexcept { return all_games.size(); }

        // Create a new Board and stores it in memory with the other Boards.
        std::shared_ptr<Board> new_game()
        {
            all_games.push_back( std::make_shared<Board>() );

            // if this is the first game that wwe created, then weäll add this as the current board that we'll modify
            if ( all_games.size() == 1 ) current_board = all_games.back();

            return all_games.back();
        }

        // Returns the Board which is at the given index in the std::vector
        std::weak_ptr<Board> get_game( size_t game_index ) noexcept
        {
            if ( game_index < active_games_count() && game_index >= 0 ) {
                return all_games[game_index];
            }

            else {
                return std::weak_ptr<Board>();
            }
        }

        // Delete a board which game has ended and remove it from our std::vector.
        bool end_game( size_t game_index ) noexcept
        {   
            
            if ( game_index < active_games_count() && game_index >= 0 ) {
                // we check if the current_board member points to the same object, if it doest, erase it too
                if ( current_board == all_games[game_index] ) {
                    current_board = std::shared_ptr<Board>();
                }
            
                all_games.erase(all_games.begin() + game_index);
                return true;
            }

            else {
                return false;
            }
        }
        
        // Set which board we are modifying from our array of Boards ( more specifically std::vector of std::shared_ptr's ).
        bool set_current( size_t& game_index ) noexcept
        {   
            if (  all_games.empty() ) return false;

            current_board = all_games[ helper::clamp<uint32_t>(game_index, 0, all_games.size()-1) ];

            if ( !current_board ) return false;

            return true;
        }

        // Returns the board that is the currently handled and modified state, in other words its held by the current_board member variable.
        const std::shared_ptr<Board> current() noexcept
        {
            return this->current_board;
        }


        // This method abstracts away alot of the stuff that the code needs to do so the Board classes move_piece() method works smoothly and the gui can work normally,
        // otherwise this would have to be directly implemented in the gui code.
        std::string move_piece( std::weak_ptr<Square> orig, std::weak_ptr<Square> target, helper::coordinates& click_coords, int32_t& width, int32_t& height ) noexcept
        {   
            std::shared_ptr<Piece> clicked_piece;
            std::vector<coordinates> can_go;
            coordinates move_vec; // well use this to check if the piece can move to a new location


            // with this for loop we check if the square that we clicked on can be moved to by our piece,
            // so basically if the square is in the possible moves.
            clicked_piece = orig.lock()->get_piece().lock();
            can_go = current_board->doesnt_get_in_check( clicked_piece, orig.lock()->coordinates() );

            for (  const coordinates& a_move : can_go ) {

                // this is the difference between the clicked_square and a_square.
                move_vec = current_board->convert_pos(click_coords.x, click_coords.y, width, height) - orig.lock()->coordinates();

                if ( move_vec  == a_move ) {
                    //std::cout << clicked_square.lock()->get_piece().lock()->tell_name() << "\n"; 

                    if ( current_board->move_piece(orig, target ) ) {
                        return target.lock()->get_piece().lock()->tell_name() + target.lock()->coordinates().toChesstring() + "\n";
                    }

                    else {
                        return "";
                    }
                }
            }


            return "";
        }

        


};

#endif