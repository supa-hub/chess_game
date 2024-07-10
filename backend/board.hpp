#ifndef CHESSBOARD
#define CHESSBOARD
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <random>
#include <functional>
#include <unordered_set>
#include <chrono> 

#include "chess_piece.hpp"
#include "square.hpp"
#include "helper_tools.hpp"

// because our namespace members are fairly unique, there wont be any namespace errors when doing this
using helper::chess_letters;
using helper::coordinates;



// simplify type declarations
typedef std::string aString;
typedef std::weak_ptr<Piece> weakPiecePtr;
typedef std::shared_ptr<Piece> sharedPiecePtr;
typedef std::unique_ptr< helper::coordinates<int64_t> > coordinate_ptr;



// Base class that handles the semantics of a chessboard in the backend.
class Board 
{
    private:
        // add some base initialisation values
        int32_t player_turn = WHITE;
        int32_t amount_of_players = 2;

        // we keep count of each players score
        int32_t score1 = 0;
        int32_t score2 = 0;


        std::unordered_set< aString > kings_in_check;
        std::unordered_set< aString > kings_in_checkmate;
        bool finished = false;


        std::unordered_set<uint32_t> already_used; // well use this for when we shuffle around pieces (it's a special gamemode)

        // we will create a 8x8 board into this container ( NOTE: you can specify a custom board size, but the pieces will be in the 1 to 8 squares )
        std::vector< std::vector< std::shared_ptr<Square> >> all_squares;

        uint32_t board_length = 0; // we will use this member in the future if we want to create a game with different board sizes

        std::vector< std::vector<std::string> > all_captured_pieces; // this will hold all the capured pieces names separated by their color_id

    public:
    
        std::unordered_set<aString> checked_kings() { return this->kings_in_check; }
        std::unordered_set<aString> checkmated() { return this->kings_in_checkmate; }
        bool is_finished() { return this->finished; }

        void end_game()
        {
            if ( kings_in_checkmate.empty() ) return;

            for ( const aString& color : kings_in_checkmate ) {
                if ( color == "w" ) { 
                    score2++; 
                    return;
                }

                else if ( color == "b" ) {
                    score1++;
                    return;
                }
            }

            finished = true;
            return;
        }


        Board() : all_squares(8, std::vector< std::shared_ptr<Square>>(8) ), board_length(8), all_captured_pieces(2)
        {
            this->create_board();
            this->already_used.reserve(32);
        }

        // for custom size board
        Board(uint32_t size) : all_squares(size, std::vector< std::shared_ptr<Square>>(size) ), board_length(size), all_captured_pieces(2)
        {
            this->create_board();
            this->already_used.reserve(32);
        }


        // we add all the pieces onto the board
        void add_pieces()
        {


            std::shared_ptr<Square> a_square;

            // add all the pieces in their places onto the board
            // I dont use switch-statement to make the code clearer
            for ( size_t i = 0; i < 8; i++ ) {
                for ( size_t j = 0; j < 8; j++ ) {
                    a_square = all_squares[i][j];
                    a_square->remove_piece();


                    if ( j == 1 )  a_square->add_piece(std::make_shared<Pawn>("P", "w", WHITE));
                    else if ( j == 6 ) a_square->add_piece(std::make_shared<Pawn>("P", "b", BLACK));

                    else if ( (i == 0 || i == 7 ) && j == 0 )  a_square->add_piece(std::make_shared<Rook>("R", "w", WHITE));
                    else if ( (i == 0 || i == 7 ) && j == 7 )  a_square->add_piece(std::make_shared<Rook>("R", "b", BLACK));

                    else if ( (i == 1 || i == 6 ) && j == 0 )  a_square->add_piece(std::make_shared<Knight>("K", "w", WHITE));
                    else if ( (i == 1 || i == 6 ) && j == 7 )  a_square->add_piece(std::make_shared<Knight>("K", "b", BLACK));

                    else if ( (i == 2 || i == 5 ) && j == 0 )  a_square->add_piece(std::make_shared<Bishop>("B", "w", WHITE));
                    else if ( (i == 2 || i == 5 ) && j == 7 )  a_square->add_piece(std::make_shared<Bishop>("B", "b", BLACK));

                    else if ( i == 3 && j == 0 )  a_square->add_piece(std::make_shared<Queen>("Q", "w", WHITE));
                    else if ( i == 3 && j == 7 )  a_square->add_piece(std::make_shared<Queen>("Q", "b", BLACK));

                    else if ( i == 4 && j == 0 )  a_square->add_piece(std::make_shared<King>("K", "w", WHITE));
                    else if ( i == 4 && j == 7 )  a_square->add_piece(std::make_shared<King>("K", "b", BLACK));
                }
            }


            return;
        }



        // this method add the chess pieces into random starting positions, except the pawns.
        void add_shuffled_pieces()
        {
            std::shared_ptr<Square> a_square;


            // add all the pieces in their places onto the board
            // I dont use switch-statement to make the code clearer
            for ( size_t i = 0; i < 8; i++ ) {
                for ( size_t j = 0; j < 8; j++ ) {
                    a_square = all_squares[i][j];
                    a_square->remove_piece();
                }
            }
            


            // add all the pieces in their places onto the board
            for ( size_t i = 0; i < 8; i++ ) {
                a_square = all_squares[i][1];
                
                a_square->add_piece(std::make_shared<Pawn>("P", "w", WHITE));
            }


            for ( size_t i = 0; i < 8; i++ ) {
                a_square = all_squares[i][6];
                
                a_square->add_piece(std::make_shared<Pawn>("P", "b", BLACK));
            }



            // add the white rooks
            add_specific_piece(std::make_shared<Rook>("R", "w", WHITE), 2, {0, 0}, {board_length, 0});

            // add the white knights
            add_specific_piece(std::make_shared<Knight>("K", "w", WHITE), 2, {0, 0}, {board_length, 0});

            // add the white bishops
            add_specific_piece(std::make_shared<Bishop>("B", "w", WHITE), 2, {0, 0}, {board_length, 0});

            // add the white queen
            add_specific_piece(std::make_shared<Queen>("Q", "w", WHITE), 1, {0, 0}, {board_length, 0});

            // add the white king
            add_specific_piece(std::make_shared<King>("K", "w", WHITE), 1, {0, 0}, {board_length, 0});

            already_used.clear();
            already_used.reserve(16);

            // add the black rooks
            add_specific_piece(std::make_shared<Rook>("R", "b", BLACK), 2, {0, 7}, {board_length, 7});

            // add the black knights
            add_specific_piece(std::make_shared<Knight>("K", "b", BLACK), 2, {0, 7}, {board_length, 7});

            // add the black bishops
            add_specific_piece(std::make_shared<Bishop>("B", "b", BLACK), 2, {0, 7}, {board_length, 7});

            // add the black queen
            add_specific_piece(std::make_shared<Queen>("Q", "b", BLACK), 1, {0, 7}, {board_length, 7});

            // add the black king
            add_specific_piece(std::make_shared<King>("K", "b", BLACK), 1, {0, 7}, {board_length, 7});

            already_used.clear();
            already_used.reserve(16);
            
            player_turn = WHITE;


            return;
        }


        // to simplify the add_shuffled_pieces() method, I created a new method that add a specific piece, 
        // otherwise I would manually have to do this to every piece
        void add_specific_piece(sharedPiecePtr a_piece, const uint32_t& amount, const helper::coordinates<uint32_t>& start_range, const helper::coordinates<uint32_t>& end_range)
        {
            uint32_t i = 0;
            uint32_t chosen = 0;
            std::shared_ptr<Square> a_square;

            // we ensure that the given range is in the given range of the board
            uint32_t x0 = helper::clamp<uint32_t>(start_range.x, 0, board_length-1);
            //int32_t y0 = clamp<int32_t>(start_range.y, 0, board_length-1);
            uint32_t x1 = helper::clamp<uint32_t>(end_range.x, 0, board_length-1);
            uint32_t y1 = helper::clamp<uint32_t>(end_range.y, 0, board_length-1);

            std::default_random_engine generator;
            generator.seed(std::chrono::system_clock::now().time_since_epoch().count()); // give our generator a random seed

            std::uniform_int_distribution<uint32_t> distribution(x0,x1);

            auto random_num = std::bind( distribution, generator );  // bind the generator to the distribution object

            

            while ( i < amount ) {
                chosen = random_num();
                a_square = all_squares[chosen][y1]; // get square

                // check that the chosen square is not already used by a different piece
                if ( already_used.count(chosen) == 0 ) {
                    a_square->add_piece(a_piece);
                    already_used.insert(chosen);
                    i++;
                }   
            }



            return;
        }


        
        // create a new board
        void create_board() 
        {
            
            this->score1 = 0;
            this->score2 = 0;


            // we dont know how many pieces each player will capture, but we already reserve enough space for all the pieces
            // if black captures a white piece, then the white pieces name goes into blacks captured array
            for ( std::vector<std::string> a_color_arr : all_captured_pieces ) {
                a_color_arr.clear();
                a_color_arr.reserve(16);
            }
           

            int cycle = 0;

            // with this nested loop we create all the squares
            for ( size_t i = 0; i < this->board_length; i++ ) {
                for ( size_t j = 0; j < this->board_length; j++ ) {

                    // here we use normal initialisation because if we use std::make_shared the objects
                    // wont be deleted until all the weak pointers go out of scope.
                    // Because I use std::weak_ptr's in square, I cannot use std::make_shared
                    if ( cycle%2 ) {
                        all_squares[i][j] = std::shared_ptr<Square>( new Square("black", (i), (j)) );

                    }

                    else {
                        all_squares[i][j] = std::shared_ptr<Square>( new Square("white", (i), (j)) );
                    }
                    cycle++;
                }
            }

            
            return;
        }




        // here are some basic class functions to return some values.
        std::vector< std::shared_ptr<Square> > get_neighbors(std::vector< coordinate_ptr >& attacking_moves, helper::coordinates<int64_t>& location)
        {
            helper::coordinates<int64_t> new_location;
            std::vector< std::shared_ptr<Square> > possible_locations;

            for ( coordinate_ptr& an_attack : attacking_moves ) {
                new_location = location + (*an_attack);
                possible_locations.push_back( this->all_squares[new_location.x][new_location.y] );
            }

            return possible_locations;
        }

        
        std::weak_ptr<Piece> get_piece( std::shared_ptr<Square>& square ) 
        { 
            return square->get_piece(); 
        }


        std::weak_ptr<Square> get_square( helper::coordinates<int64_t> location )
        {
            return all_squares[static_cast<size_t>( location.x )][static_cast<size_t>( location.y )];
        }

        std::weak_ptr<Square> get_square( int64_t x, int64_t y )
        {
            return all_squares[static_cast<size_t>( x )][static_cast<size_t>( y )];
        }
        
        // converts window helper::coordinates<int64_t> into a squares helper::coordinates<int64_t>, these helper::coordinates<int64_t> can then be used
        // to get the corresponding square
        helper::coordinates<int64_t> convert_pos( const int& x, const int& y, const int64_t& screen_width, const int64_t& screen_height, bool use_clamp = true ) noexcept
        {
            int square_width = screen_width/8;
            int square_height = screen_height/8;

            int x1 = x/square_width;
            int y1 = y/square_height;

            if ( use_clamp ) {
                x1 = helper::clamp<int32_t>(x1, 0, 7);
                y1 = helper::clamp<int32_t>(y1, 0, 7);
            }


            //std::cout << x1 << " ";
            //std::cout << y1 << std::endl;

            

            return helper::coordinates<int64_t>{x1, y1};
        }

        



        // this method calls true, if the piece could be moved, and false if the piece couldn't be moved
        bool move_piece( std::weak_ptr<Square> orig, std::weak_ptr<Square> target ) noexcept
        {
            if ( orig.expired() || target.expired() ) return false;

            if ( this->player_turn != orig.lock()->get_piece().lock()->tell_color_id() ) {
                return false;
            }


            orig.lock()->get_piece().lock()->moved();

            // in the same line we remove a chess piece from the old square and add it in the new one.
            sharedPiecePtr removed_piece = target.lock()->add_piece( orig.lock()->remove_piece() );


            if ( removed_piece ) {
                all_captured_pieces[ target.lock()->get_piece().lock()->tell_color_id() ].push_back( removed_piece->tell_name() );
            }


            update_attacked_squares();
            update_check();
            update_checkmate();
            


            if ( ++this->player_turn == amount_of_players ) {
                this->player_turn = WHITE;
            }

            return true;
        }


        /**
         * @brief this is a special case of Board::move_piece in which the king and castle change
         * places. This method trusts that the Board::find_possible_tiles_to_move already validated the castling
         * and it doesnt to the check again.
         */
        bool king_rook_move( std::weak_ptr<Square> orig, std::weak_ptr<Square> target, int64_t direction ) 
        {
            if ( orig.expired() || target.expired() ) return false;

            if ( this->player_turn != orig.lock()->get_piece().lock()->tell_color_id() ) {
                return false;
            }

            sharedPiecePtr orig_piece = orig.lock()->get_piece().lock();
            uint16_t color_id = orig_piece->tell_color_id();

            std::shared_ptr<Square> target_square; // the square that contains the rook that we'll move

            if ( orig_piece->tell_id() != KING*pow(10, color_id ) ) {
                return false;
            }

            if ( direction == 2 ) {
                target_square = get_square( 7, orig.lock()->coordinates().y ).lock();
            }

            else if ( direction == -2 ) {
                target_square = get_square( 0, orig.lock()->coordinates().y ).lock();
            }


            if ( !target_square ) {
                return false;
            }



            orig.lock()->get_piece().lock()->moved();



            sharedPiecePtr removed_piece = target.lock()->add_piece( orig.lock()->remove_piece() );
            helper::coordinates<int64_t> king_coords = target.lock()->coordinates(); // the kings coords

            // if the rook is at x = 7, well move it to king direction - 1, if its at x = 0, then to king_dir + 1
            int64_t rook_dir = ( direction < 0 ) ? 1 : -1;

            // now we move the rook
            return move_piece( target_square, get_square( king_coords.x + rook_dir, king_coords.y ) );

            //return true;
        }



        // We use this with Board::doesnt_get_in_check() instead of the normal move_piece()
        // to prevent circular method calls.
        void base_move( std::weak_ptr<Square> orig, std::weak_ptr<Square> target )
        {
            if ( orig.expired() || target.expired() ) return;

            target.lock()->add_piece( orig.lock()->remove_piece() );

            update_attacked_squares();

            return;
        }




        // This method finds the kings on the board and returns their positions.
        std::vector<coordinate_ptr> find_kings()
        {
            std::weak_ptr<Square> a_square_ptr;
            std::vector<coordinate_ptr> king_pos;
            king_pos.reserve(2); // usually theres 2 kings, so we optimise,
            // if theres more kings, the push_back method will increase the size.

            for ( size_t i = 0; i < 8; i++ ) {
                for ( size_t j = 0; j < 8; j++ ) {
                    a_square_ptr = all_squares[i][j];

                    // first we check that the square contains a piece
                    if ( a_square_ptr.lock()->get_piece().expired() ) {
                        continue;
                    }



                    if ( a_square_ptr.lock()->get_piece().lock()->tell_id() == KING*pow(10, a_square_ptr.lock()->get_piece().lock()->tell_color_id()) ) {
                            king_pos.push_back( std::make_unique< helper::coordinates<int64_t> >(a_square_ptr.lock()->coordinates()) );
                    }
                }
            }

            return king_pos;

        }

        /**
         * @brief Checks whether the square of the given helper::coordinates<int64_t> contains a chess piece.
         * The method returns true, if it contains a piece, and false if the square is empty (or if coords are out of bounds).
         */
        bool has_piece( const helper::coordinates<int64_t>& a ) noexcept
        {   
            if ( a.x >= 0 && a.x < 7 && a.y >= 0 && a.y < 7 ) {
                return all_squares[static_cast<size_t>( a.x )][static_cast<size_t>( a.y )]->has_piece();
            }

            else {
                return false;
            }
            
        }

        /**
         * @brief Validates the different squares in the given range to check whether the piece can move to them
         * @param current the current position of our piece
         * @param a_piece the given piece
         * @return std::vector< helper::coordinates<int64_t> > works as an array of vectors, which are applied to the pieces current position
         */
        std::vector< helper::coordinates<int64_t> > find_possible_tiles_to_move_to(const helper::coordinates<int64_t>& current, sharedPiecePtr a_piece) noexcept
        {   
            if ( !a_piece ) return std::vector< helper::coordinates<int64_t> >();

            
            const std::vector< coordinate_ptr >& moves = a_piece->possible_moves();
            uint32_t piece_id = a_piece->tell_id(); 
            std::string color = a_piece->tell_color();
            uint16_t color_id = a_piece->tell_color_id();


            helper::coordinates<int64_t> aux0 = current;
            helper::coordinates<int64_t> aux;
            bool is_same_direction = false; // we'll use this to figure out if a piece is blocking the way.
            helper::coordinates<int64_t> vector;
            std::vector < helper::coordinates<int64_t> > castling_moves;

            std::vector< helper::coordinates<int64_t> > directions_cannot_go;
            std::vector< helper::coordinates<int64_t> > can_go;


            if ( piece_id == PAWN*pow(10, color_id)) {
                return pawn_moves(current, a_piece);
            }

            
            if ( piece_id == KING*pow(10, color_id) ) {
                castling_moves = king_castling( current, a_piece );
                can_go.insert( can_go.end(), castling_moves.begin(), castling_moves.end() );
            }
            

            for ( const coordinate_ptr& a_move : moves ) {
                vector = *a_move;
                aux = aux0 + vector;

                // if the move is not out of bounds, we add it.
                if ( aux.x < 0 || aux.x >= this->board_length || aux.y < 0 || aux.y >= this->board_length ) {
                    directions_cannot_go.push_back( vector );
                    continue; // this ensures that we skip the parts from below so we dont have to use helper::clamp
                }


                // because the king cannot move to a tile that is attacked, we have to do this check
                // we also use clamp in these places to protect against segfault.
                if ( piece_id == KING*pow(10, color_id) && get_square( aux.x, aux.y ).lock()->attacked() ) {
                    for ( aString a_color : get_square( aux.x, aux.y ).lock()->attacking_colors() ) {
                        if ( color != a_color) {
                            directions_cannot_go.push_back( vector );
                            break;
                        }
                    }
                }

               

                // check if a square has a piece, so we cannot go to the next square behind it.
                if ( get_square( aux.x, aux.y ).lock()->has_piece() ) {
                    
                    // we check if the piece is of similar color, or if our piece is a pawn, because a pawn cannot take the 
                    // piece in front of it.
                    if ( color == get_square( aux.x, aux.y ).lock()->get_piece().lock()->tell_color() || piece_id == PAWN) {
                        directions_cannot_go.push_back( vector );
                     }
                    
                    else {
                        vector.increment();
                        directions_cannot_go.push_back( vector );
                    }

                }
            }


            for ( const coordinate_ptr& a_move : moves ) {
                vector = *a_move;
                aux = aux0 + vector;
                is_same_direction = false;
                
                // if the move is not out of bounds, we add it.
                if ( aux.x < 0 || aux.x >= this->board_length || aux.y < 0 || aux.y >= this->board_length ) {
                    continue; // this ensures that we skip the parts from below so we dont have to use helper::clamp
                }


                for ( helper::coordinates<int64_t> a_vector : directions_cannot_go ) {
                    if ( helper::same_direction(a_vector, vector) )  {
                        is_same_direction = true;
                        break;
                    }
                }



                if ( !is_same_direction ) {
                    can_go.push_back(vector);
                }
                
            }


            return can_go;
        }


        // 
        /**
         * @brief we create an own distinct method for pawns moves because they have weird movement mechanics 
         * compared to other pieces.
         * @param current the current position of our pawn
         * @param a_piece a shared_ptr to our pawn to get some basic info
         * @return std::vector< helper::coordinates<int64_t> > 
         */
        std::vector< helper::coordinates<int64_t> > pawn_moves(const helper::coordinates<int64_t>& current, sharedPiecePtr a_piece) noexcept 
        {
            helper::coordinates<int64_t> aux0 = current;
            helper::coordinates<int64_t> aux;
            std::string color = a_piece->tell_color();
            uint16_t color_id = a_piece->tell_color_id();

            std::shared_ptr<Square> a_square;
            std::shared_ptr<Square> attacked_square;
            std::vector< helper::coordinates<int64_t> > can_go;

            std::vector<helper::coordinates<int64_t>> basic_moves = { helper::coordinates<int64_t>{0, 1}, helper::coordinates<int64_t>{0, 2} };
            

            // we also manually check for the 2 pawn moves that are only possible if theres a piece of opposite color.
            std::vector<helper::coordinates<int64_t>> attacking_moves = { helper::coordinates<int64_t>{1, 1}, helper::coordinates<int64_t>{-1, 1} };
            
            if ( color_id == BLACK ) {
                basic_moves = { helper::coordinates<int64_t>{0, -1}, helper::coordinates<int64_t>{0, -2} };
                attacking_moves = { helper::coordinates<int64_t>{1, -1}, helper::coordinates<int64_t>{-1, -1} };
            }


            if ( a_piece->has_moved() ) basic_moves.pop_back();

            // we check for the normal pawn moves if they're possible.
            for ( helper::coordinates<int64_t>& basic : basic_moves ) {
                aux = aux0 + basic;

                a_square = get_square( helper::clamp<int>(aux.x, 0, 7), helper::clamp<int>(aux.y, 0, 7) ).lock();

                if ( a_square->has_piece() ) {
                    break;
                }

                else {
                    can_go.push_back( basic );
                }
            }


            for ( helper::coordinates<int64_t>& attack : attacking_moves) {
                aux = aux0 + attack;

                attacked_square = get_square( helper::clamp<int64_t>(aux.x, 0, 7), helper::clamp<int64_t>(aux.y, 0, 7) ).lock();

                if ( attacked_square->has_piece() ) {
                    if ( color != attacked_square->get_piece().lock()->tell_color()) {
                        can_go.push_back( attack );
                    }

                }
            }


            return can_go;
        
        }


        /*
         this methods updates every squares variable that
         we will use in the is_check method to check whether the king is in check
        */
        void update_attacked_squares()
        {   
            Square a_square;
            helper::coordinates<int64_t> aux;

            for ( size_t i = 0; i < 8; i++ ) {
                for ( size_t j = 0; j < 8; j++ ) {
                    a_square = *all_squares[i][j];
                    aux = a_square.coordinates();
                    this->get_square(aux).lock()->change_attacked_status(false);
                }
            }


            for ( size_t i = 0; i < 8; i++ ) {
                for ( size_t j = 0; j < 8; j++ ) {
                    a_square = *all_squares[i][j];

                    // first we check that the square contains a piece
                    if ( a_square.get_piece().expired() ) {
                        continue;
                    }
                    
                    sharedPiecePtr a_piece = a_square.get_piece().lock();

                    /*
                     because the king cannot technically "attack" a square the same way other pieces
                     can, we skip it.
                     The reason for this is that this method is used to check for check or checkmate.
                    */
                    if ( a_piece->tell_id() == KING*pow(10, a_piece->tell_color_id()) ) {
                            continue;
                    }

                    aString color = a_piece->tell_color();


                    // because the pawn doesnt attack with all of its moves, we have to create a special case for it.
                    if ( a_piece->tell_id() == PAWN*pow(10, a_piece->tell_color_id()) ) {
                            const std::vector< coordinate_ptr >& moves = a_piece->attack();

                            for ( const coordinate_ptr& move : moves ) {
                                aux = a_square.coordinates();
                                aux = aux + *move;

                                // we have to clamp these into the acceptable range
                                if ( aux.x >= 0 && aux.x <= 7 && aux.y >= 0 && aux.y <= 7 ) {
                                    this->get_square(aux).lock()->change_attacked_status(color);
                                }

                            }

                            continue;
                    }
                    

                    // we remove the locations that the piece cannot move to.
                    const std::vector<helper::coordinates<int64_t>>& moves = find_possible_tiles_to_move_to( 
                                                                a_square.coordinates(),
                                                                a_piece
                                                                );
                    
                    
                    

                    for ( const helper::coordinates<int64_t>& move : moves ) {
                        aux = a_square.coordinates();
                        aux = aux + move;

                        // we have to clamp these into the acceptable range
                        if ( aux.x >= 0 && aux.x <= 7 && aux.y >= 0 && aux.y <= 7 ) {
                            this->get_square(aux).lock()->change_attacked_status(color);
                        }

                    }
                }
            }
            

            return;
        }


        // returns the names of captured pieces
        std::vector<aString> captured_pieces( const uint16_t& color_id ) { 
            return all_captured_pieces[ helper::clamp<size_t>( static_cast<size_t>(color_id), 0, all_captured_pieces.size()-1 ) ]; 
        }


        // these  methods will tell us if the king is in check.
        void update_check();
        void update_checkmate();

        // The below 2 methods look for check and ckeckmate for a specific color.
        bool is_check( const std::string& color_to_check );
        bool is_checkmate( const aString& color_to_check );

        // This method will be the main way the code filters out the places the the piece cannot 
        // go to at that moment.
        std::vector< helper::coordinates<int64_t> > doesnt_get_in_check( weakPiecePtr a_piece, helper::coordinates<int64_t> current);

    private:
        // with this method we'll check if the king can castle
        template<typename T>
        inline std::vector< helper::coordinates<T> > king_castling( helper::coordinates<T> current, sharedPiecePtr a_piece );
        template<typename T>
        inline bool square_is_protected( helper::coordinates<T> current, sharedPiecePtr king );
    
 
};




inline void Board::update_check()
{   
    std::vector< coordinate_ptr > king_coords = this->find_kings();
    this->kings_in_check.clear();

    if ( king_coords.empty() ) {
        return;
    }

    /*
     We check the helper::coordinates<int64_t> where kings are located and check if 
     a piece of different color attacks it.
    */
    for ( coordinate_ptr& coords : king_coords ) {
        std::string king_color = get_square(*coords).lock()->get_piece().lock()->tell_color();
        
        // loop through the piece colors that attack the square
        for ( aString a_color : get_square(*coords).lock()->attacking_colors() ) {

            
            // We check if the square that the king is on is attacked by an opposite color piece
            if ( a_color != king_color ) {
                kings_in_check.insert( king_color );
            }
        }
           
    }

    return;
    
}

/*
 We check if a king of certain color is in check. 
 We need this for example when white tries to move a pawn but the white king is in check.
*/
bool Board::is_check( const aString& color_to_check )
{   
    update_check();
    return this->kings_in_check.count( color_to_check ) != 0;
}




inline void Board::update_checkmate()
{
    std::vector< coordinate_ptr > king_coords = this->find_kings();
    this->kings_in_checkmate.clear();

    if ( king_coords.empty()) {
        return;
    }

    for ( const coordinate_ptr& coords : king_coords ) {
        sharedPiecePtr king = get_square(*coords).lock()->get_piece().lock();

        for ( aString a_color : get_square(*coords).lock()->attacking_colors() ) {

            
            /*
             in this if statement we check if the king is in check, and also
             if theres nowhere the king can go. I both of these are true,
             then the king is in checkmate.
            */
            if ( 
                a_color != king->tell_color()  
                &&  
                find_possible_tiles_to_move_to( 
                    *coords,
                    king
                ).empty()
            ) {
                
            }
            // this is for the special case in which the piece checking the king is right next to the it
            else if ( this->square_is_protected( *coords, king ) ) {
                this->kings_in_checkmate.insert( king->tell_color() );
            }
                
        }
           
    }


    return;
}


/*
 We check if the game ends because a king is in checkmate.
 This method returns the color that checkmated the king, e.g the opponents color.
*/
bool Board::is_checkmate( const aString& color_to_check )
{
    update_checkmate();
    return this->kings_in_checkmate.count( color_to_check ) != 0;
}



/** @brief this method loops through the pieces moves and checks if the king gets in check
 * when the piece moves.
 * This method only returns the moves that don't get the pieces king in check.
*/
std::vector< helper::coordinates<int64_t> > Board::doesnt_get_in_check(weakPiecePtr a_piece, helper::coordinates<int64_t> current_pos)
{   
    
    std::vector< helper::coordinates<int64_t> > possible_moves;
    std::vector< helper::coordinates<int64_t> > filtered_moves; // these are the squares the the piece can move to.
    sharedPiecePtr removed_piece;
    if ( a_piece.expired() ) return possible_moves;

    aString color_to_check = a_piece.lock()->tell_color();
    

    // filter out the places that the piece cannot go to
    filtered_moves = find_possible_tiles_to_move_to(current_pos, a_piece.lock());

    for ( const helper::coordinates<int64_t>& vector : filtered_moves ) {
        helper::coordinates<int64_t> a_move = current_pos + vector;
        removed_piece = get_square(a_move).lock()->get_piece().lock();

        base_move(get_square(current_pos), get_square(a_move));

        update_attacked_squares();
        // if the king is not in check anymore, then its a possible move.
        if ( !is_check(color_to_check) && !is_checkmate(color_to_check) ) {
            possible_moves.push_back(vector);
        }


        base_move(get_square(a_move), get_square(current_pos));

        // because a removed piece doesnt retain his old position, 
        // we cant use base_move or move_piece methods to add it back,
        // we'll also call update_attacked_squared() after we've added the piece back.
        if ( removed_piece ) { 
            get_square(a_move).lock()->add_piece(removed_piece);
            update_attacked_squares();
        }

    }

    return possible_moves;
    
}

// checks whether a piece that is checking the king right next to the king is protected by another piece
template<typename T>
inline bool Board::square_is_protected( helper::coordinates<T> current, sharedPiecePtr king )
{
    bool return_val = false;
    sharedPiecePtr removed_piece;

    for ( helper::coordinates<int64_t>& vec : find_possible_tiles_to_move_to( current, king ) ) {
        removed_piece = get_square(current + vec).lock()->get_piece().lock();
        base_move( get_square(current), get_square(current + vec) );

        if ( is_check( king->tell_color() ) ) {
            return_val = true;
        }

        base_move(get_square(current + vec), get_square(current));

        // because a removed piece doesnt retain his old position, 
        // we cant use base_move or move_piece methods to add it back,
        // we'll also call update_attacked_squared() after we've added the piece back.
        if ( removed_piece ) { 
            get_square(current + vec).lock()->add_piece(removed_piece);
            update_attacked_squares();
        }

        if ( return_val ) {
            break;
        }
    }

    return return_val;
}



template<typename T>
inline std::vector< helper::coordinates<T> > Board::king_castling( helper::coordinates<T> current, sharedPiecePtr a_piece )
{
    if ( !a_piece ) return std::vector< helper::coordinates<int64_t> >();

    if ( current.x < 0 || current.x > this->board_length || current.y < 0 || current.y > this->board_length ) {
        return std::vector< coordinates<int64_t> >{};
    }

    uint32_t piece_id = a_piece->tell_id(); 
    std::string_view color{ a_piece->tell_color() };
    uint16_t color_id = a_piece->tell_color_id();

    helper::coordinates<int64_t> aux;

    std::vector< helper::coordinates<int64_t> > castling_moves;
    castling_moves.reserve(2); // we know that this will only contains at most 2 moves

    std::vector< helper::coordinates<int64_t> > can_go;
    can_go.reserve(2); // we also know that this will at most contains 2 moves

    std::vector< helper::coordinates<int64_t> > directions_cannot_go;
    directions_cannot_go.reserve(2);

    
    // check if the king can castle
    // first we check if the king is in his starting position,
    // and then we check if his castles are in are in their starting positions.
    if ( piece_id == KING*pow(10, color_id) && !a_piece->has_moved() && ( this->kings_in_check.count( a_piece->tell_color() ) == 0 ) ) {

        // first check if there is a piece, then if the piece is a rook
        if ( !( get_square( 0, current.y ).lock()->get_piece().expired() ) ) { 
            if ( get_square( 0, current.y ).lock()->get_piece().lock()->tell_id() == ROOK*pow(10, color_id) &&
                get_square( 0, current.y ).lock()->get_piece().lock()->tell_color_id() == color_id ) {
                    castling_moves.push_back( helper::coordinates<int64_t>{ -2, 0 } );
            }
        }
        
        if ( !( get_square( 7, current.y).lock()->get_piece().expired() ) ) {
            if ( get_square( 7, current.y).lock()->get_piece().lock()->tell_id() == ROOK*pow(10, color_id) &&
                get_square( 7, current.y).lock()->get_piece().lock()->tell_color_id() == color_id ) {
                    castling_moves.push_back( helper::coordinates<int64_t>{ 2, 0 } );
            }
        }
    }

    else {
        return std::vector< coordinates<int64_t> >{};
    }


    for ( helper::coordinates<int64_t>& move : castling_moves ) {
        aux = current + move;

        // because the king cannot move to a tile that is attacked, we have to do this check
        // we also use clamp in these places to protect against segfault.
        if ( piece_id == KING*pow(10, color_id) && get_square( aux.x, aux.y ).lock()->attacked() ) {
            for ( aString a_color : get_square( aux.x, aux.y ).lock()->attacking_colors() ) {
                if ( color != a_color) {
                    directions_cannot_go.push_back( move );
                    break;
                }
            }
        }

        

        // check if a square has a piece, so we cannot go to the next square behind it.
        if ( get_square( aux.x, aux.y ).lock()->has_piece() ) {
            directions_cannot_go.push_back( move );
        }
    }

    
    // this will at the most do only 4 iterations, because caslting_moves and directions_cannot_go 
    // will both have at most 2 moves
    // this std::copy_if only copies the move sinto can_go if they're not in directions_cannot_go
    // so basically filter out the moves to which he king cannot go
    std::copy_if( castling_moves.begin(), castling_moves.end(), std::back_inserter(can_go), [&directions_cannot_go]( helper::coordinates<int64_t> i ) -> bool { return std::find( directions_cannot_go.begin(), directions_cannot_go.end(), i ) == directions_cannot_go.end(); });
    
    
    
    return can_go;
}



#endif