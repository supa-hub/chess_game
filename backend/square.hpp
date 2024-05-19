#ifndef SQUARE
#define SQUARE
#include <string>
#include <vector>
#include <memory>

#include "chess_piece.hpp"
#include "helper_tools.hpp"

// simplify type declarations
typedef std::string aString;
typedef std::weak_ptr<Piece> weakPiecePtr; 
typedef std::shared_ptr<Piece> sharedPiecePtr; 



class Square 
{
    private:
        aString name;
        coordinates position;
        int id = 0;
        bool under_attack = false;
        std::vector< std::string > colors_attacking;

        sharedPiecePtr container = sharedPiecePtr();


    public:
        // base constructor
        Square()
        {
            this->name = "none";
            colors_attacking.reserve(2);
        }

        Square( aString name, int x, int y )
        {
            this->name = name;
            this->position.x = x;
            this->position.y = y;
            colors_attacking.reserve(2);

            if ( name == "black" ) {
                this->id = 1;
            }

        }

        Square( aString name, int x, int y, int id )
        {
            this->name = name;
            this->position.x = x;
            this->position.y = y;
            this->id = id;
            colors_attacking.reserve(2);

        }

        // add a piece into the square
        sharedPiecePtr add_piece( std::shared_ptr<Piece> a_piece ) noexcept
        {   
            // we do a standard std::move to change ownership
            sharedPiecePtr aux = std::move(container);

            container = a_piece;

            return aux; 
        }


        // we remove a piece from the square
        sharedPiecePtr remove_piece() { return std::move(this->container); }

        // return the squares coordinates
        coordinates coordinates()  { return this->position; }

        // basically check if there's a piece that can move to this square
        bool attacked() { return this->under_attack; }
        
        // change the value of this->under_attack
        void change_attacked_status(const bool& a) noexcept
        { 
            this->under_attack = a; 
            
            // because the square is not attacked, we clear the vector that stores what colors are attacking it.
            if ( !a ) {
                colors_attacking.clear();
            }
        }


        void change_attacked_status(const std::string& a) noexcept
        { 
            this->under_attack = true;
            colors_attacking.push_back(a); 
        }

        // returns true if this square contains a piece, and false if the square's empty
        inline bool has_piece() noexcept
        {
            // we call the bool() operator of std::shared_ptr
            return (this->container) ? true : false;

        }

        // return a std::weak_ptr to the piece that this square contains
        weakPiecePtr get_piece()
        {
            weakPiecePtr aux = this->container;
            return aux;
        }

        // returns a vector that has the color of the pieces that can currently move to the square
        std::vector< std::string > attacking_colors()
        {
            return colors_attacking;
        }

        // check whether 2 given Squares have the same coordinates,
        // if they have, then this operator considers them the same Square
        inline bool operator == ( Square a_square )
        {
            return { this->position.x == a_square.coordinates().x &&  this->position.y == a_square.coordinates().y };
        }
        

};

#endif

