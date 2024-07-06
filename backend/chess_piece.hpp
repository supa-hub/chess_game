#ifndef CHESS_PIECE
#define CHESS_PIECE
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include "helper_tools.hpp"
#include <utility>
#include <cstdint>


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
        uint16_t id = 0;
        uint16_t value = 0;
        uint16_t color_id = 0;
        bool first_move = true; // this will help us check if its the first pawn move or rook move, 
                                // because there are certain chess rules that require that info.


        int32_t position; // will be changed into the square when its done.

        std::vector< coordinate_ptr > moves;
        std::vector< coordinate_ptr > attacking_moves;


        //std::weak_ptr<Square> current_square;

        // this method will add the pieces moves into our vectors
        virtual void add_moves() {}

    public:
        // these methods return the base values of the piece
        aString tell_name() { return this->name; }
        aString tell_color() { return this->color; }
        uint16_t tell_color_id() { return this->color_id; }
        uint16_t tell_id() { return this->id; }
        uint16_t tell_value() { return this->value; }
        bool has_moved() { return !this->first_move; }

        void moved() { this->first_move = false; }
        

        // The base constructor of Piece.
        Piece() : name("none"), color("none") { }

        // We construct a standard piece and give it its standard values.
        Piece(aString name0, aString color0) : name(name0), color(color0) { }

        Piece(aString name0, aString color0, int color_id0) : name(name0), color(color0), color_id(color_id0) { }

        Piece(aString name0, aString color0, int id0, int value0) : name(name0), color(color0), id(id0), value(value0) { }

        Piece(aString name0, aString color0, int id0, int value0, int color_id0) : name(name0), color(color0), id(id0), value(value0), color_id(color_id0) 
        { 
            this->id = this->id*(pow(10, color_id0));
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
    protected:
        inline void add_moves() override
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

    public:
        Pawn() : Piece()
        {
            this->id = PAWN;
            this->value = 1;

            add_moves();
        }

        Pawn(aString name0, aString color0) : Piece(name0, color0, PAWN, 1)
        {   
            add_moves();
        }

        Pawn(aString name0, aString color0, int color_id0) : Piece(name0, color0, PAWN, 1, color_id0)
        {
            add_moves();
        }




         
};


class Rook : public virtual Piece
{
    protected:
        inline void add_moves() override
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

    public:
        Rook() : Piece()
        {
            this->id = ROOK;
            this->value = 5;

            add_moves();
        }


        Rook(aString name0, aString color0) : Piece(name0, color0, ROOK, 5)
        {   
            add_moves();
        }

        Rook(aString name0, aString color0, int color_id0) : Piece(name0, color0, ROOK, 5, color_id0)
        {
            add_moves();
        }

        
};


class Bishop : public virtual Piece
{
    protected:
        inline void add_moves() override
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

    public:
        Bishop() : Piece()
        {
            this->id = BISHOP;
            this->value = 3;

            add_moves();
            
        }


        Bishop(aString name0, aString color0) : Piece(name0, color0, BISHOP, 3)
        {   
            add_moves();
        }

        Bishop(aString name0, aString color0, int color_id0) : Piece(name0, color0, BISHOP, 3, color_id0)
        {
            add_moves();
        }



};





class Knight : public virtual Piece
{
    protected:

        inline void add_moves() override
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

    public:
        Knight() : Piece()
        {
            this->id = KNIGHT;
            this->value = 3;

            add_moves();
        }


        Knight(aString name0, aString color0) : Piece(name0, color0, KNIGHT, 3)
        {   
            add_moves();
        }

        Knight(aString name0, aString color0, int color_id0) : Piece(name0, color0, KNIGHT, 3, color_id0)
        {
            add_moves();
        }


};


// for Queen, we inherit both Rook and Bishop to get both of their add_pieces() methods.
class Queen : public Rook, public Bishop
{
    protected:
        inline void add_moves() override
        {
            Rook::add_moves();
            Bishop::add_moves();
        }
    public:
        Queen()
        {
            this->name = "none";
            this->color = "none";
            this->id = QUEEN;
            this->value = 8;

            this->add_moves();
        }

        Queen(aString name0, aString color0)
        {
            this->name = name0;
            this->color = color0;
            this->id = QUEEN;
            this->value = 8;


            this->add_moves();
        }

        Queen(aString name0, aString color0, int color_id0)
        {
            this->name = name0;
            this->color = color0;
            this->id = QUEEN;
            this->value = 8;
            this->color_id = color_id0;

            this->id = this->id*(pow(10, color_id0));


            this->add_moves();
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

        King(aString name0, aString color0) : Piece(name0, color0, KING, 8)
        {
            add_moves();
        }


        King(aString name0, aString color0, int color_id0) : Piece(name0, color0, KING, 8, color_id0)
        {
            add_moves();
        }


        inline void add_moves() override
        {
            for (int64_t i = -1; i < 2; i++) {
                for ( int64_t j = -1; j < 2; j++){
                    if ( i == 0  && j == 0 ) { continue; }

                    this->moves.push_back( std::make_unique< helper::coordinates<int64_t> >(i, j) );
                }
            }
        }
};


#endif
