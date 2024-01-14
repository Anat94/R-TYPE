![rtype_image](https://fs-prod-cdn.nintendo-europe.com/media/images/10_share_images/games_15/virtual_console_wii_u_7/H2x1_WiiUVC_RType.jpg)

# Welcome to our Rtype project !

This project is a third-year school endeavor aimed at creating a professional-grade Entity Component System (ECS) framework for multiplayer game development. It's built using C++ and designed to be cross-compiled across Linux, macOS, and Windows using CMake.

## Features

- ECS Architecture: Implements a robust Entity Component System for managing game entities, behaviors, and attributes.

- Multiplayer Support: Incorporates networking capabilities to enable multiplayer game development.

- Cross-Platform Compatibility: Designed to compile seamlessly across Linux, macOS, and Windows environments using CMake.

- Modular Structure: Divided into three major parts - server, ECS framework, and client - ensuring a clean, modular, and scalable codebase.

- Professional Standards: Aimed at adhering to professional coding practices and industry standards.

## Components

### Server

**Description**: Manages game sessions, networking, and communication between clients.

**Key Features**:
- Session management
- Networking protocols
- Multiplayer synchronization

### ECS framework

**Description**: Provides the core ECS architecture for entity management, component-based design, and game logic implementation.

**Key Features**:
- Entity-Component-System architecture
- Component-based game entities
- System logic for behaviors

### Client

**Description**: Represents the front-end application where players interact with the game.

**Key Features**:
- User Interface
- Client-side game logic
- Rendering and interaction

# Build and Compilation

**CMake**: Use CMake for cross-platform compilation and build process.

**Dependencies**: Specify any external libraries or tools required for compilation.

**Compilation Steps**: Provide detailed instructions for building the server, ECS framework, and client across different operating systems.

# Usage

## Compilation

### Linux build

```shell
cmake .
cmake --build .
```

### Windows build

```powershell
cmake .
cmake --build .
```

## Execution

For the server:
### Linux

```shell
./server <port>
```

### Windows

```powershell
./Debug/server.exe <port>
```

For the client:

### Linux

```shell
./client <ip_addr> <port>
```

### Windows

```powershell
./Debug/client.exe <ip_addr> <port>
```

# Further explanations of the project

The goal of the project is to re-create a game called R-Type. It was created by [Irem](https://en.wikipedia.org/wiki/Irem) in 1987. In this game, the player controls a star ship in its efforts to destroy the Bydo, a powerful alien race bent on wiping out all of mankind.

Because the goal is not just to make a video game, our school spiced it up by asking us to create an ECS (Entity Component System) ffrom scratch. An ECS is a very common thing to use in video game development. It allows you to store entities very efficiently and easily.

## How does the ECS works

An ECS is made to store entities and the components related to them.
Like its name, there are 3 parts in an ECS:

- *Entity*:

    An entity is a game object that represent a logiclal "thing" within the game world. It doesn't contain any data or behavior but serves as a container for components.

- *Components*:

    Components represent the data or attributes of an entity. Each component holds specific information about an entity, such as its position, health, rendering information, etc.

- *Systems*:

    Systems handle the behavior and logic of entities by operating on their associated components. Each system is responsible for specific functionality or behavior in the game. For instance, a Physics System might handle collision detection and response, while a Rendering System might handle rendering entities on the screen. Systems process entities by selecting entities with specific components and applying operations to those entities' components.

## Network implementation

### UDP connection

**UDP Communication**: Our ECS multiplayer game project leverages the User Datagram Protocol (UDP) for network communication between game entities. UDP is chosen for its lightweight, connectionless nature, making it ideal for real-time, latency-sensitive applications like multiplayer games. Its low overhead and reduced latency compared to TCP (Transmission Control Protocol) are well-suited for the rapid exchange of game data packets. While UDP doesn't guarantee packet delivery or ordering like TCP, it offers faster transmission, making it optimal for scenarios where speed and responsiveness are paramount, essential qualities for delivering an immersive multiplayer gaming experience. Our implementation carefully handles data integrity and synchronization within the ECS framework, ensuring smooth, near-instantaneous communication between game clients and the server, crucial for seamless multiplayer interactions.

## Start unit tests


```shell
cmake --build . --target my_tests
```
