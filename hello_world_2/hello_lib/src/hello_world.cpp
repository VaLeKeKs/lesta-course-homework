//
// Created by valentin on 03.04.23.
//
#include <iostream>
#include <hello_world.h>

void printHelloWorld() {
    std::cout << "Hello World" << std::endl;
    if(std::cout.fail()) throw std::runtime_error("Error");
}
