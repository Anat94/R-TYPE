// /*
// ** EPITECH PROJECT, 2023
// ** rtype
// ** File description:
// ** main
// */

// #include <iostream>
// #include <boost/asio.hpp>
// #include "../Errors.hpp"
// #include "Client.hpp"
// #include "Menu.hpp"

// using boost::asio::ip::udp;

// int main(int argc, char** argv) {
//     std::string tmp_username;

//     try {
//         if (argc != 3) {
//             throw ArgumentError("./client <client_ip> <client_port>");
//         }
//         enum state _state = MENU;
//         while (_state != END) {
//             if (_state == MENU) {
//                 Menu menu;
//                 _state = menu.run();
//                 tmp_username = menu.getUsername();
//                 std::cout << "1username: " << tmp_username << std::endl;
//             } else if (_state == GAME) {
//                 Client client(argv[1], atoi(argv[2]), tmp_username);
//                 return client.run();
//             }
//             if (_state == SUCCES)
//                 return 0;
//         }
//     } catch (ArgumentError e) {
//         std::cerr << "Usage: " << e.what() << std::endl;
//         return 84;
//     } catch (SFMLError e) {
//         std::cerr << "Sfml error: " << e.what() << std::endl;
//         return 84;
//     }

//     return 0;
// }

#include "raylib.h"

int main(void)
{
    InitWindow(800, 450, "raylib [core] example - basic window");

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}