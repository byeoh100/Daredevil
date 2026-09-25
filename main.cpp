#include "movegen.hpp"
#include "uci.hpp"

int main() {
    movegen::init_all_pieces();
    uci::loop();

    return 0;
}
