#include <string>
#include <memory>
#include "backend/square.hpp"
#include "backend/chess_piece.hpp"
#include "backend/board.hpp"


int wmain()
{
    std::unique_ptr<Board> board_ptr = std::make_unique<Board>();
    board_ptr->add_pieces();

    std::weak_ptr<Square> a_square = (board_ptr->get_square( coordinates{1, 1} ));
    //std::weak_ptr<Piece> a_piece = (a_square.lock())->get_piece();

    //std::cout << a_piece.lock()->tell_name() << std::endl;

    std::shared_ptr<Piece> a_piece = a_square.lock()->remove_piece();

    
    std::cout << a_piece->tell_name() << std::endl;

    return 0;
}