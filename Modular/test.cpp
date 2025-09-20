#include<bits/stdc++.h>
#include "Mint.h"

using Mint = ModInt<5>;

int main() {
    Mint x;
    std::cin >> x;
    std::cout << x << std::endl;
    std::cout << Mint(2) + Mint(4) << std::endl;
    std::cout << Mint(2) - Mint(4) << std::endl;
    std::cout << Mint(2) * Mint(4) << std::endl;
    std::cout << Mint(2).power(4) << std::endl;
}