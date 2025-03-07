#include "OrderedSet.hpp"
#include <iostream>

int main() {
  OrderedSet bst = {{10, -7, 13, -9, 8, -21, -100, 50, 2, 12}, [](const int& x, const int& y){
    return std::abs(x) < std::abs(y);
  }};

  for (const auto& e: bst) {
    std::cout << e << ' ';
  }
  std::cout << std::endl;
  
  auto l{10}, h{20};
  
  for (auto it = bst.find(10); *it < h;)
    bst.erase(it++);

  for (const auto& e: bst) {
    std::cout << e << ' ';
  }
  std::cout << std::endl;
}
