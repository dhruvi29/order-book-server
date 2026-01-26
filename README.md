# order-book-server

## What is

1. cmake
2. mingw
3. vcpkg
4. drogon


## Installation
1. Install vcpkg - [Installation Guide](https://learn.microsoft.com/en-us/vcpkg/users/platforms/mingw)
2. Install Drogon - [Installation Guide](https://drogonframework.github.io/drogon-docs/#/ENG/ENG-02-Installation)

## How to setup Drogon server

Create Drogon Server

```bash
drogon_ctl create project order-book-server
```
Create HTTP & Web Socket Controller
```bash
cd controllers
drogon_ctl create controller OrderController 
drogon_ctl create controller -w WsMarketController
```
## How to run the project
```bash
cd build
cmake .
cmake --build .
.\order-book-server
```
> UI is a React app made with Vite in /ui directory. Checkout Readme in /ui. 

