cmake_minimum_required(VERSION 3.16)
project(t1 C)

set(CMAKE_C_STANDARD 99)

add_executable(t1 src/register.c src/register.h src/main.c src/binarioNaTela.c src/binarioNaTela.h)