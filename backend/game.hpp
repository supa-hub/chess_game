#ifndef CHESS_GAME
#define CHESS_GAME

#include <cstdint>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <iostream>

#include "chess_piece.hpp"
#include "square.hpp"
#include "helper_tools.hpp"
#include "board.hpp"


using helper::coordinates;

/*
 We'll use this class more in the future to manage multiple 
 windows with chess games at once.
*/
class Game 
{
    private:
        std::vector< std::shared_ptr<Board> > all_games;
        std::vector< std::shared_ptr< std::deque<std::string> >> moves_history; // the deques store the moves history for each game in the all_games vector at the coresponding index.
        std::shared_ptr<Board> current_board; // this will contain the board that we are currently modifying. We need this variable because we can have multiple boards.
        std::shared_ptr< std::deque<std::string> > current_history;
        std::string return_str = "";

        inline size_t captured_len( const int& color_id ) 
        {
            return current_board->captured_pieces( color_id ).size(); 
        }


    public:
    
        size_t active_games_count() noexcept { return all_games.size(); }
        std::weak_ptr< std::deque<std::string> > get_moves() { return current_history; }

        // Create a new Board and stores it in memory with the other Boards.
        std::shared_ptr<Board> new_game()
        {
            all_games.push_back( std::make_shared<Board>() );
            moves_history.push_back( std::make_shared<std::deque<std::string>>() );

            // if this is the first game that wwe created, then we'll add this as the current board that we'll modify
            if ( all_games.size() == 1 ) {
                current_board = all_games.back();
                current_history = moves_history.back();
            }
            

            return all_games.back();
        }

        // Returns the Board which is at the given index in the std::vector
        std::weak_ptr<Board> get_game( size_t game_index ) noexcept
        {
            if ( game_index < active_games_count() ) {
                return all_games[game_index];
            }

            else {
                return std::weak_ptr<Board>();
            }
        }

        // Delete a board which game has ended and remove it from our std::vector.
        bool end_game( const size_t& game_index ) noexcept
        {   
            
            if ( game_index < active_games_count() ) {
                // we check if the current_board member points to the same object, if it doesnt, erase it too
                if ( current_board == all_games[game_index] ) {
                    current_board.reset();
                    current_history.reset();
                }
            
                all_games.erase(all_games.begin() + static_cast<int64_t>(game_index));
                return true;
            }

            else {
                return false;
            }
        }

        void reset_text() 
        {
            current_history->clear();
        }
        
        // Set which board we are modifying from our array of Boards ( more specifically std::vector of std::shared_ptr's ).
        bool set_current( const size_t& game_index ) noexcept
        {   
            if (  all_games.empty() ) return false;

            current_board = all_games[ helper::clamp<uint32_t>(game_index, 0, all_games.size()-1) ];
            current_history = moves_history[ helper::clamp<uint32_t>(game_index, 0, all_games.size()-1) ];

            if ( !current_board || !current_history ) return false;

            return true;
        }

        // Returns the board that is the currently handled and modified state, in other words its held by the current_board member variable.
        const std::shared_ptr<Board> current() noexcept
        {
            return this->current_board;
        }

        

        /**
         * @brief This method abstracts away alot of the stuff that the code needs to do so the Board classes move_piece() method works smoothly and the gui can work normally,
         * otherwise this would have to be directly implemented in the gui code.
         * 
         * @param orig 
         * @param target 
         * @param click_coords 
         * @param width 
         * @param height 
         * @return std::string 
         */
        bool move_piece( std::weak_ptr<Square> orig, std::weak_ptr<Square> target, helper::coordinates<int64_t>& click_coords, int32_t& width, int32_t& height ) noexcept
        {   
            std::shared_ptr<Piece> clicked_piece;
            std::vector< helper::coordinates<int64_t> > can_go;
            helper::coordinates<int64_t> move_vec; // well use this to check if the piece can move to a new location
            size_t original_len = 0; // we'll use this to check if we've captured a piece
            bool return_val = false;
            

            if ( current_board->is_finished() ) return false;

            // with this for loop we check if the square that we clicked on can be moved to by our piece,
            // so basically if the square is in the possible moves.
            clicked_piece = orig.lock()->get_piece().lock();
            can_go = current_board->doesnt_get_in_check( clicked_piece, orig.lock()->coordinates() );


            original_len = captured_len( clicked_piece->tell_color_id() );
            
            // this is the difference between the clicked_square and a_square.
            move_vec = current_board->convert_pos(click_coords.x, click_coords.y, width, height) - orig.lock()->coordinates();

            for (  const helper::coordinates<int64_t>& a_move : can_go ) {

                if ( move_vec  == a_move ) {
                    
                    // this if-statement is for castling
                    if ( clicked_piece->tell_id() == KING*pow(10, clicked_piece->tell_color_id()) && 
                        (a_move.x == 2 || a_move.x == -2)) {
                            
                            if ( current_board->king_rook_move( orig, target, a_move.x ) ) {
                                current_history->push_back( ( a_move.x == -2 ) ? "0-0-0\n" : "0-0\n" );
                                return_val = true;
                                break;
                            }
                    }


                    else if ( current_board->move_piece(orig, target ) ) {

                        if ( original_len < captured_len( clicked_piece->tell_color_id() ) ) {
                            current_history->push_back( clicked_piece->tell_name() + "x" + target.lock()->coordinates().toChesstring() + "\n" );
                        }
                        else {
                            current_history->push_back( clicked_piece->tell_name() + target.lock()->coordinates().toChesstring() + "\n" );
                        }
                        return_val = true;
                        break;
                    }
                }
            }

            // we tell the game object that we've executed a move on the board and now it should check whether the king is in check
            if ( !current_board->checkmated().empty() ) {
                current_board->end_game();
                for ( const aString& color : current_board->checkmated() ) {
                    current_history->push_back( "The color: " + color + " got checkmated.\n" );
                }
                
            }

            return return_val;
        }



        


};

#endif