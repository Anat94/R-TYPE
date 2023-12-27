# Welcome to our Rtype project !

This project is part of our third year at EPITECH.
The goal is to create an ECS (entity component system) from scratch and add network to it, so that it assemble into a multiplayer game.

## Before starting

Make sure to have the necessary tools installed in order to build the project.

TODO: list of tools (linux / windows)

# How to launch

### Linux build

```shell
cmake .
cmake --build .
```

### Windows build

```powershell
cmake .
cmake --build ..
```

## Launching the server

### Linux

```shell
./server <port>
```

### Windows

```powershell
./Debug/server.exe <port>
```

## Launching the client

### Linux

```shell
./client <ip_addr> <port>
```

### Windows

```powershell
./Debug/client.exe <ip_addr> <port>
```

## Start unit tests


```shell
cmake --build . --target my_tests
```