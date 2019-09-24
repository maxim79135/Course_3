//
//  main.cpp
//  FirstSFML
//
//  Created by Максим Седов on 23.09.2019.
//  Copyright © 2019 Максим Седов. All rights reserved.
//

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(int argc, const char * argv[]) {
    
    RenderWindow *window = new RenderWindow(VideoMode(100, 100), "Hello");
    
    while (window->isOpen()) {
        window->clear();
        window->display();
    }
    return 0;
}
