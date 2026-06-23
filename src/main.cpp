#include <cstdlib>
#include <ctime>
#include "CLI.h"

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    CLI cli;
    cli.run();
    return 0;
}
