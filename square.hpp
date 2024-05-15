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

class Piece;

class Square 
{
    private:
        aString name;
        coordinates position;
        int id = 0;
        bool under_attack = false;
        std::vector< std::string > colors_attacking;

        sharedPiecePtr container = sharedPiecePtr();
        // std::shared_ptr<Board> board;


    public:
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
        sharedPiecePtr add_piece( std::shared_ptr<Piece> a_piece )
        {   
            /*
            if ( container.expired() ) {
                //  if the square is empty, we will return an empty pointer. The emptiness will be checked by the code that calls this method
                container = a_piece;

                return weakPiecePtr(); // we return an empty weak pointer, because the vector doesnt have any piece
            }

            else {
                weakPiecePtr aux = container;
                //aux = container;

                container = a_piece;

                return aux;
            }  */
            sharedPiecePtr aux = std::move(container);
            //aux = container;

            container = a_piece;

            return aux; 
        }

        // we remove a piece from the square
        sharedPiecePtr remove_piece() { return std::move(this->container); }

        coordinates coordinates()  { return this->position; }

        bool attacked() { return this->under_attack; }
        
        void change_attacked_status(const bool& a) 
        { 
            this->under_attack = a; 
            
            // because the square is not attacked, we clear the vector that stores what colors are attacking it.
            if ( !a ) {
                colors_attacking.clear();
            }
        }

        void change_attacked_status(const std::string& a) 
        { 
            this->under_attack = true;
            colors_attacking.push_back(a); 
        }

        inline bool has_piece()
        {
            // we call the bool() operator of std::shared_ptr
            return (this->container) ? true : false;

        }


        weakPiecePtr get_piece()
        {
            weakPiecePtr aux = this->container;
            return aux;
        }

        std::vector< std::string > attacking_colors()
        {
            return colors_attacking;
        }


        inline bool operator == ( Square a_square )
        {
            return { this->position.x == a_square.coordinates().x &&  this->position.y == a_square.coordinates().y };
        }
        

};

#endif

