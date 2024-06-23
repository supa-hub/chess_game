#ifndef CHESSBOARD
#define CHESSBOARD
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <stdint.h>
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
typedef std::string_view constString;
typedef std::weak_ptr<Piece> weakPiecePtr; 
typedef std::shared_ptr<Piece> sharedPiecePtr;
typedef std::unique_ptr< coordinates > coordinate_ptr;



// Base class that handles the semantics of a chessboard in the backend.
class Board 
{
    private:
        // add some base initialisation values
        int32_t player_turn = WHITE;
        int32_t amount_of_players = 2;

        // we keep count of each players score
        int score1 = 0;
        int score2 = 0;
        std::vector<constString> captured_pieces_white; // the pieces that white has captured
        std::vector<constString> captured_pieces_black; // the pieces that black has captured

        std::unordered_set<int32_t> already_used; // well use this for when we shuffle around pieces (it's a special gamemode)

        // we will create a 8x8 board into this container ( NOTE: you can specify a custom board size, but the pieces will be in the 1 to 8 squares )
        std::vector< std::vector< std::shared_ptr<Square> >> all_squares;

        int32_t board_length = 0; // we will use this member in the future if we want to create a game with different board sizes


    public:

        Board() : all_squares(8, std::vector< std::shared_ptr<Square>>(8) ), board_length(8)
        {
            this->create_board();
            this->already_used.reserve(32);
        }

        // for custom size board
        Board(uint32_t size) : all_squares(size, std::vector< std::shared_ptr<Square>>(size) ), board_length(size)
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
            for ( int i = 0; i < 8; i++ ) {
                for ( int j = 0; j < 8; j++ ) {
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
            for ( int i = 0; i < 8; i++ ) {
                for ( int j = 0; j < 8; j++ ) {
                    a_square = all_squares[i][j];
                    a_square->remove_piece();
                }
            }


            // add all the pieces in their places onto the board
            for ( int i = 0; i < 8; i++ ) {
                a_square = all_squares[i][1];
                a_square->remove_piece();
                
                a_square->add_piece(std::make_shared<Pawn>("P", "w", WHITE));
            }


            for ( int i = 0; i < 8; i++ ) {
                a_square = all_squares[i][6];
                a_square->remove_piece();
                
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
        void add_specific_piece(sharedPiecePtr a_piece, const int32_t& amount, const coordinates& start_range, const coordinates& end_range)
        {
            int32_t i = 0;
            int32_t chosen = 0;
            std::shared_ptr<Square> a_square;

            // we ensure that the given range is in the given range of the board
            int32_t x0 = helper::clamp<int32_t>(start_range.x, 0, board_length-1);
            //int32_t y0 = clamp<int32_t>(start_range.y, 0, board_length-1);
            int32_t x1 = helper::clamp<int32_t>(end_range.x, 0, board_length-1);
            int32_t y1 = helper::clamp<int32_t>(end_range.y, 0, board_length-1);

            std::default_random_engine generator;
            generator.seed(std::chrono::system_clock::now().time_since_epoch().count()); // give our generator a random seed

            std::uniform_int_distribution<int> distribution(x0,x1);

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

            captured_pieces_white.clear();
            captured_pieces_black.clear();

            // we dont know how many pieces each player will capture, but we already reserve enough space for all the pieces
            captured_pieces_white.reserve(16); 
            captured_pieces_black.reserve(16);

            int cycle = 0;

            // with this nested loop we create all the squares
            for ( int i = this->board_length-1; i > -1; i-- ) {
                for ( int j = this->board_length-1; j > -1; j-- ) {

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
        std::vector< std::shared_ptr<Square> > get_neighbors(std::vector< coordinate_ptr >& attacking_moves, coordinates& location)
        {
            coordinates new_location;
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


        std::weak_ptr<Square> get_square( coordinates location )
        {
            std::weak_ptr<Square> square;
            square = all_squares[location.x][location.y];

            return all_squares[location.x][location.y];
        }

        std::weak_ptr<Square> get_square( int x, int y )
        {
            std::weak_ptr<Square> square;
            square = all_squares[x][y];

            return all_squares[x][y];
        }
        
        // converts window coordinates into a squares coordinates, these coordinates can then be used
        // to get the corresponding square
        coordinates convert_pos( const int& x, const int& y, const int64_t& screen_width, const int64_t& screen_height, bool use_clamp = true ) noexcept
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

            

            return coordinates{x1, y1};
        }

        


        void move_piece( coordinate_ptr& direction )
        {
            int x = direction->x;
            int y = direction->y;

            // in the same line we remove a chess piece from the old square and add it in the new one
            std::weak_ptr removed_piece = (this->all_squares)[x][y]->add_piece( (this->all_squares)[x][y]->remove_piece() );

            if ( !(removed_piece.expired()) ) {
                this->captured_pieces_white.push_back( (removed_piece.lock())->tell_name() );
            }

            update_attacked_squares();
            
            if ( is_check() ) { 
                    std::cout << "king is in check" << "\n"; 
                }
        }

        // this method calls true, if the piece could be moved, and false if the piece couldn't be moved
        bool move_piece( const std::weak_ptr<Square> orig, const std::weak_ptr<Square> target ) noexcept
        {
            if ( orig.expired() || target.expired() ) return false;

            if ( this->player_turn != orig.lock()->get_piece().lock()->tell_color_id() ) {
                return false;
            }

            orig.lock()->get_piece().lock()->moved();
            // in the same line we remove a chess piece from the old square and add it in the new one.
            sharedPiecePtr removed_piece = target.lock()->add_piece( orig.lock()->remove_piece() );

            if ( removed_piece ) {
                this->captured_pieces_white.push_back( removed_piece->tell_name() );
            }


            update_attacked_squares();
            
            

            if ( is_check() ) { 
                    std::cout << "king is in check " << this->player_turn << "\n"; 
                }

            if ( is_checkmate() != "" ) {
                std::cout << "the opposing king is checkmated" << "\n"; 
                std::cout << "the color " << is_checkmate() << " won." << "\n"; 
            }


            if ( ++this->player_turn == amount_of_players ) {
                this->player_turn = WHITE;
            }

            return true;
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

            for ( int i = 0; i < 8; i++ ) {
                for ( int j = 0; j < 8; j++ ) {
                    a_square_ptr = all_squares[i][j];

                    // first we check that the square contains a piece
                    if ( a_square_ptr.lock()->get_piece().expired() ) {
                        continue;
                    }



                    if ( a_square_ptr.lock()->get_piece().lock()->tell_id() == KING*pow(10, a_square_ptr.lock()->get_piece().lock()->tell_color_id()) ) {
                            king_pos.push_back( std::make_unique<coordinates>(a_square_ptr.lock()->coordinates()) );
                    }
                }
            }

            return king_pos;

        }

        /*
         Checks whether the square of the given coordinates contains a chess piece.
         The method returns true, if it contains a piece, and false if the square is empty.
        */
        bool has_piece( const coordinates& a ) noexcept
        {   
            if ( a.x >= 0 && a.x < 7 && a.y >= 0 && a.y < 7 ) {
                return all_squares[a.x][a.y]->has_piece();
            }

            else {
                return false;
            }
            
        }

        std::vector< coordinates > find_possible_tiles_to_move_to(const coordinates& current, sharedPiecePtr a_piece) noexcept
        {   
            if ( !a_piece ) return std::vector< coordinates >();

            
            const std::vector< coordinate_ptr >& moves = a_piece->possible_moves();
            int32_t piece_id = a_piece->tell_id(); 
            constString color{ a_piece->tell_color() };
            int32_t color_id = a_piece->tell_color_id();


            coordinates aux0 = current;
            coordinates aux;
            bool is_same_direction = false; // we'll use this to figure out if a piece is blocking the way.
            coordinates vector;

            std::vector< coordinates > directions_cannot_go;
            std::vector< coordinates > can_go;


            if ( piece_id == PAWN*pow(10, color_id)) {
                return pawn_moves(current, a_piece);
            }

            for ( const coordinate_ptr& a_move : moves ) {
                vector = *a_move;
                aux = aux0 + vector;

                // if the move is not out of bounds, we add it.
                if ( aux.x < 0 || aux.x > 7 || aux.y < 0 || aux.y > 7 ) {
                    directions_cannot_go.push_back( vector );
                }


                // because the king cannot move to a tile that is attacked, we have to do this check
                // we also use clamp in these places to protect against segfault.
                if ( piece_id == KING*pow(10, color_id) && get_square( helper::clamp<int>(aux.x, 0, 7), helper::clamp<int>(aux.y, 0, 7)).lock()->attacked() ) {
                    for ( aString a_color : get_square( helper::clamp<int>(aux.x, 0, 7), helper::clamp<int>(aux.y, 0, 7)).lock()->attacking_colors()) {
                        if ( color != a_color) {
                            directions_cannot_go.push_back( vector );
                            break;
                        }
                    }
                }
               


                // check if a square has a piece, so we cannot go to the next square behind it.
                if ( get_square( helper::clamp<int>(aux.x, 0, 7), helper::clamp<int>(aux.y, 0, 7)).lock()->has_piece() ) {
                    
                    // we check if the piece is of similar color, or if our piece is a pawn, because a pawn cannot take the 
                    // piece in front of it.
                    if ( color == get_square( helper::clamp<int>(aux.x, 0, 7), helper::clamp<int>(aux.y, 0, 7) ).lock()->get_piece().lock()->tell_color() || piece_id == PAWN) {
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
                
                
                for ( coordinates a_vector : directions_cannot_go ) {
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


        // we create an own distinct method for pawns moves because they have weird movement mechanics 
        // compared to other pieces.
        std::vector< coordinates > pawn_moves(const coordinates& current, sharedPiecePtr a_piece) noexcept 
        {
            coordinates aux0 = current;
            coordinates aux;
            constString color{ a_piece->tell_color() };
            int32_t color_id = a_piece->tell_color_id();

            std::shared_ptr<Square> a_square;
            std::shared_ptr<Square> attacked_square;
            std::vector< coordinates > can_go;

            std::vector<coordinates> basic_moves = { coordinates{0, 1}, coordinates{0, 2} };
            

            // we also manually check for the 2 pawn moves that are only possible if theres a piece of opposite color.
            std::vector<coordinates> attacking_moves = { coordinates{1, 1}, coordinates{-1, 1} };
            
            if ( color_id == BLACK ) {
                basic_moves = { coordinates{0, -1}, coordinates{0, -2} };
                attacking_moves = { coordinates{1, -1}, coordinates{-1, -1} };
            }


            if ( a_piece->has_moved() ) basic_moves.pop_back();

            // we check for the normal pawn moves if they're possible.
            for ( coordinates& basic : basic_moves ) {
                aux = aux0 + basic;

                a_square = get_square( helper::clamp<int>(aux.x, 0, 7), helper::clamp<int>(aux.y, 0, 7) ).lock();

                if ( a_square->has_piece() ) {
                    break;
                }

                else {
                    can_go.push_back( basic );
                }
            }


            for ( coordinates& attack : attacking_moves) {
                aux = aux0 + attack;

                attacked_square = get_square( helper::clamp<int>(aux.x, 0, 7), helper::clamp<int>(aux.y, 0, 7) ).lock();

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
            coordinates aux;

            for ( int i = 0; i < 8; i++ ) {
                for ( int j = 0; j < 8; j++ ) {
                    a_square = *all_squares[i][j];
                    aux = a_square.coordinates();
                    this->get_square(aux).lock()->change_attacked_status(false);
                }
            }


            for ( int i = 0; i < 8; i++ ) {
                for ( int j = 0; j < 8; j++ ) {
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
                    const std::vector<coordinates>& moves = find_possible_tiles_to_move_to( 
                                                                a_square.coordinates(),
                                                                a_piece
                                                                );
                    
                    
                    

                    for ( const coordinates& move : moves ) {
                        aux = a_square.coordinates();
                        aux = aux + move;

                        // we have to clamp these into the acceptable range
                        if ( aux.x >= 0 && aux.x <= 7 && aux.y >= 0 && aux.y <= 7 ) {
                            this->get_square(aux).lock()->change_attacked_status(color);
                        }

                    }
                }
            }
        }


        // these  methods will tell us if the king is in check.
        bool is_check();
        constString is_checkmate();

        // The below 2 overloads are almost the same as the original methods but they
        // look for check and ckeckmate for a specific color.
        bool is_check(const constString& color_to_check);
        constString is_checkmate(const constString& color_to_check);

        // This method will be the main way the code filters out the places the the piece cannot 
        // go to at that moment.
        std::vector<coordinates> doesnt_get_in_check( weakPiecePtr a_piece, coordinates current);

        // with this method we'll check if the king can castle
        bool king_castling();
        
 
};




bool Board::is_check()
{   


    std::vector< coordinate_ptr > king_coords = this->find_kings();

    if ( king_coords.empty() ) {
        return false;
    }

    /*
     We check the coordinates where kings are located and check if 
     a piece of different color attacks it.
    */
    for ( coordinate_ptr& coords : king_coords ) {
        // loop through the piece colors that attack the square
        for ( aString a_color : get_square(*coords).lock()->attacking_colors() ) {


            // We check if the square that the king is on is attacked by an opposite color piece
            if ( a_color != get_square(*coords).lock()->get_piece().lock()->tell_color() )
                return true;
        }
           
    }

    return false;
    
}

/*
 We check if a king of certain color is in check. 
 We need this for example when white tries to move a pawn but the white king is in check.
*/
bool Board::is_check(const constString& color_to_check)
{   


    std::vector< coordinate_ptr > king_coords = this->find_kings();
    std::string king_color;

    if ( king_coords.empty() ) {
        return false;
    }

    /*
     we check the coordinates where kings are located and check if 
     a piece of different color attacks it.
    */
    for ( coordinate_ptr& coords : king_coords ) {
        king_color = get_square(*coords).lock()->get_piece().lock()->tell_color();

        if ( king_color != color_to_check ) { continue; }

        // loop through the piece colors that attack the square
        for ( aString a_color : get_square(*coords).lock()->attacking_colors() ) {

            //std::cout << "a color: " << a_color << "\n";

            // We check if the square that the king is on is attacked by an opposite color piece
            if ( a_color != king_color )
                return true;
        }
           
    }

    return false;
    
}


/*
 We check if the game ends because a king is in checkmate.
 This method returns the color that checkmated the king, e.g the opponents color.
*/
constString Board::is_checkmate()
{
    std::vector< coordinate_ptr > king_coords = this->find_kings();

    if ( king_coords.empty()) {
        return "";
    }

    for ( coordinate_ptr& coords : king_coords ) {
        for ( constString a_color : get_square(*coords).lock()->attacking_colors() ) {

            sharedPiecePtr king = get_square(*coords).lock()->get_piece().lock();
            
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
                return a_color;
            }
                
        }
           
    }


    return "";
}



// we check if the game ends because a king is in checkmate
constString Board::is_checkmate(const constString& color_to_check)
{
    std::vector< coordinate_ptr > king_coords = this->find_kings();
    constString king_color;

    if ( king_coords.empty()) {
        return "";
    }

    for ( coordinate_ptr& coords : king_coords ) {
        for ( constString a_color : get_square(*coords).lock()->attacking_colors() ) {

            sharedPiecePtr king = get_square(*coords).lock()->get_piece().lock();

            king_color = king->tell_color();

            if ( king_color != color_to_check ) { continue; }
            
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
                return a_color;
            }
                
        }
           
    }


    return "";
}



/*
this method loops through the pieces moves and checks if the king gets in check
when the piece moves.
This method only returns the moves that don't get the king in check.
*/
std::vector<coordinates> Board::doesnt_get_in_check(weakPiecePtr a_piece, coordinates current_pos)
{   
    
    std::vector< coordinates > possible_moves;
    std::vector< coordinates > filtered_moves; // these are the squares the the piece can move to.
    sharedPiecePtr removed_piece;
    if ( a_piece.expired() ) return possible_moves;

    constString color_to_check{ a_piece.lock()->tell_color() };
    

    // filter out the places that the piece cannot go to
    filtered_moves = find_possible_tiles_to_move_to(current_pos, a_piece.lock());

    for ( const coordinates& vector : filtered_moves ) {
        coordinates a_move = current_pos + vector;
        removed_piece = get_square(a_move).lock()->get_piece().lock();

        base_move(get_square(current_pos), get_square(a_move));


        // if the king is not in check anymore, then its a possible move.
        if ( !is_check(color_to_check) &&  is_checkmate(color_to_check) == "" ) {
            possible_moves.push_back(vector);
        }

        base_move(get_square(a_move), get_square(current_pos));

        if (removed_piece) get_square(a_move).lock()->add_piece(removed_piece);


    }

    return possible_moves;
    
}




bool Board::king_castling()
{
    return false;
}



#endif