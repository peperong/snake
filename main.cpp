#include <ctime>
#include <cstdlib>
#include "console.h"

void game();
int main(int argc, char** argv, char** env) {
    time_t time1;
    
    time1 = time(nullptr);
    srand(static_cast<unsigned int>(time1));
    console::init();
    game();
    return 0;
}