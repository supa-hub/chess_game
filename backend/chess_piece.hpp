#ifndef CHESS_PIECE
#define CHESS_PIECE
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include "helper_tools.hpp"
#include <utility>


// simplify type declarations
typedef std::string aString;
typedef std::unique_ptr< helper::coordinates<int64_t>  > coordinate_ptr;


// because our enums are unique, there wont be any namespace errors when doing this
using helper::chess_letters;



// the base class that we'll use to derive the other chess pieces.
class Piece 
{
    protected:
        // base values
        aString name;
        aString color;
        int id = 0;
        int value = 0;
        int color_id = 0;
        bool first_move = true; // this will help us check if its the first pawn move or rook move, 
                                // because there are certain chess rules that require that info.


        int position; // will be changed into the square when its done.

        std::vector< coordinate_ptr > moves;
        std::vector< coordinate_ptr > attacking_moves;


        //std::weak_ptr<Square> current_square;


    public:
        // these methods return the base values of the piece
        aString tell_name() { return this->name; }
        aString tell_color() { return this->color; }
        int tell_color_id() { return this->color_id; }
        int tell_id() { return this->id; }
        int tell_value() { return this->value; }
        bool has_moved() { return !this->first_move; }

        void moved() { this->first_move = false; }
        

        // The base constructor of Piece.
        Piece() : name("none"), color("none") { }

        // We construct a standard piece and give it its standard values.
        Piece(aString name, aString color) : name(name), color(color) { }

        Piece(aString name, aString color, int color_id) : name(name), color(color) { }

        Piece(aString name, aString color, int id, int value) : name(name), color(color), id(id), value(value) { }

        Piece(aString name, aString color, int id, int value,  int color_id) : name(name), color(color), id(id), value(value), color_id(color_id) 
        { 
            this->id = this->id*(pow(10, color_id));
        }

        // Returns all the moves that a piece can do.
        std::vector< coordinate_ptr > const& possible_moves() const { return this->moves; }

        /*
        Returns the attacking moves of a piece. Generally these are the same as the normal moves,
        But this is needed mor for Pawn, which have different attack moves than their usual moves.
        */
        std::vector< coordinate_ptr > const& attack() const { return this->attacking_moves; }

};




// The class for the pawn.
class Pawn : public virtual Piece
{
        
    public:
        Pawn() : Piece()
        {
            this->id = PAWN;
            this->value = 1;

            add_moves();
        }

        Pawn(aString name, aString color) : Piece(name, color, PAWN, 1)
        {   
            add_moves();
        }

        Pawn(aString name, aString color, int color_id) : Piece(name, color, PAWN, 1, color_id)
        {
            add_moves();
        }



        inline void add_moves()
        {   
            // We add all the pawn moves as pointers.
            // I use push_back() instead of adding the values in initialisation to support older C++ versions.
            if ( color_id == WHITE ) {
                this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(0, 2) );
                this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(0, 1) );
                this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(1, 1) );
                this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-1, 1) );
            }

            else if ( color_id == BLACK ) {
                this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(0, -2) );
                this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(0, -1) );
                this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(1, -1) );
                this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-1, -1) );
            }
            
            
        }

         
};


class Rook : public virtual Piece
{
    public:
        Rook() : Piece()
        {
            this->id = ROOK;
            this->value = 5;

            add_moves();
        }


        Rook(aString name, aString color) : Piece(name, color, ROOK, 5)
        {   
            add_moves();
        }

        Rook(aString name, aString color, int color_id) : Piece(name, color, ROOK, 5, color_id)
        {
            add_moves();
        }

        inline void add_moves()
        {
            // we add all the moves as pointers
            for ( int i = -7; i < 8; i++ ) {
                if ( i == 0 ) { continue; }

                this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(i, 0) );
                this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(i, 0) );
            }
            for ( int j = -7; j < 8; j++ ) {
                if ( j == 0 ) { continue; }

                this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(0, j) );
                this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(0, j) );
            }
            return;
        }
};


class Bishop : public virtual Piece
{
    public:
        Bishop() : Piece()
        {
            this->id = BISHOP;
            this->value = 3;

            add_moves();
            
        }


        Bishop(aString name, aString color) : Piece(name, color, BISHOP, 3)
        {   
            add_moves();
        }

        Bishop(aString name, aString color, int color_id) : Piece(name, color, BISHOP, 3, color_id)
        {
            add_moves();
        }



        inline void add_moves()
        {
            // we add all the moves as pointers
            for ( int i = -7; i < 8; i++ ) {
                if ( i == 0 ) { continue; }

                this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(i, i) );
                this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(i, i) );

                //this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-i, i) );
                //this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-i, i) );

                this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(i, -i) );
                this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(i, -i) );

                //this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-i, -i) );
                //this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-i, -i) );
            }

            return;
        }
};





class Knight : public virtual Piece
{
    public:
        Knight() : Piece()
        {
            this->id = KNIGHT;
            this->value = 3;

            add_moves();
        }


        Knight(aString name, aString color) : Piece(name, color, KNIGHT, 3)
        {   
            add_moves();
        }

        Knight(aString name, aString color, int color_id) : Piece(name, color, KNIGHT, 3, color_id)
        {
            add_moves();
        }


        inline void add_moves()
        {
            // we add all the moves as pointers
            // I use push_back() instead of adding the values in initialisation to support older C++ versions.
            this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(1, 2) );
            this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(2, 1) );
            this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(1, 2) );
            this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(2, 1) );

            this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(1, -2) );
            this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(2, -1) );
            this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(1, -2) );
            this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(2, -1) );

            this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-1, -2) );
            this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-2, -1) );
            this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-1, -2) );
            this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-2, -1) );

            this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-1, 2) );
            this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-2, 1) );
            this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-1, 2) );
            this->attacking_moves.push_back( std::make_unique< helper::coordinates<int64_t> >(-2, 1) );
            return;
        }
};


// for Queen, we inherit both Rook and Bishop to get both of their add_pieces() methods.
class Queen : public Rook, public Bishop
{
    public:
        Queen()
        {
            this->name = "none";
            this->color = "none";
            this->id = QUEEN;
            this->value = 8;


            Rook::add_moves();
            Bishop::add_moves();
        }

        Queen(aString name, aString color)
        {
            this->name = name;
            this->color = color;
            this->id = QUEEN;
            this->value = 8;


            Rook::add_moves();
            Bishop::add_moves();
        }

        Queen(aString name, aString color, int color_id)
        {
            this->name = name;
            this->color = color;
            this->id = QUEEN;
            this->value = 8;
            this->color_id = color_id;

            this->id = this->id*(pow(10, color_id));


            Rook::add_moves();
            Bishop::add_moves();
        }


};



class King : public virtual Piece
{
    public:
        King() : Piece()
        {
            this->name = "none";
            this->color = "none";
            this->id = KING;
            this->value = 8;

            add_moves();
        }

        King(aString name, aString color) : Piece(name, color, KING, 8)
        {
            add_moves();
        }


        King(aString name, aString color, int color_id) : Piece(name, color, KING, 8, color_id)
        {
            add_moves();
        }


        inline void add_moves()
        {
            for (int i = -1; i < 2; i++) {
                for ( int j=-1; j < 2; j++){
                    if ( i == 0  && j == 0 ) { continue; }

                    this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(i, j) );
                }
            }
        }
};


#endif
