CC=gcc
CD=cd
RM=rm

CFLAGS=-Wall -g -Iinc/ -I. $(shell sdl-config --cflags)
CLIBS= $(shell sdl-config --libs) -lSDL_image -lSDL_ttf -lSDL_mixer -lm

TARGET=voyager

PREFIX=/usr/games
BIN_DIR=$(PREFIX)/bin
TARGET_DIR=$(PREFIX)/$(TARGET)

SRC_DIR=src
BUILD_DIR=build

OBJS=main.o events.o event_stubs.o ships.o bg.o hud.o weapon.o collision.o game.o sound.o config.o
