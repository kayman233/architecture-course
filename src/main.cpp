#include "Account.h"
#include "Bank.h"
#include "Client.h"
#include <iostream>
#include <set>
#include <string>

int main() {
    Client alice("Alice", "Hamburder");
    Client bob("Bob", "Ross");
    Client carl("Carl", "Wasovsky");

    Bank greenBank("G-R-E-E-N", "Green Bank, Inc", {});
    Bank yellowBank("T-I-N-K-O-F-F", "Yellow Bank, Inc", {});

    uint32_t aliceAtGreen = greenBank.registerClient(alice);
    uint32_t carlAtGreen = greenBank.registerClient(carl);
    uint32_t aliceAtYellow = yellowBank.registerClient(alice);
    uint32_t bobAtYellow = yellowBank.registerClient(bob);

    greenBank.openDebit(aliceAtGreen);

    return 0;
}