﻿#pragma once
#include <algorithm>
#include <conio.h>
#include <iostream>
#include <vector>
#include <Windows.h>

extern void gotoxy(int x, int y); // функция для переноса курсора в точку (x,y) на экране
struct Point { int x; int y; }; // структура для описания точек с координатаами (x,y)
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN }; // для более удобной работы с направлением движения

/* Класс Game - основной класс, в котором будет реализована логика игры, будут использоваться
необходимые переменные для хранения основных данных */
class Game
{
public:
	Game(int height, int width); /*конструктор, задающий высоту и ширину игрового поля и 
								   устанавливающий все необходимые начальные значения */
	~Game();
	void Run(); // главная функция игры, собирающая в себе логику и управление
private:
	bool game_over_; // отвечает за выход из игры при поражении
	int height_, width_; // ширина и высота игрового поля
	int score_; // счетчик очков
	Point prev_head_, last_tail_part_; /*запоминают предыдущие координаты данных точек,
										 нужны для удлиненния змейки при съедании фрукта*/
	Direction dir_; // направление движения головы
	Point head_position_; // позиция головы на экране
	std::vector<Point> tail_; // вектор с точками элементов хвоста
	std::vector<Point> fruit_possible_; // вектор с точками, где может появиться фрукт
	Point fruit_position_; // позиция фрукта на экране
	void DrawField(); // функция, прорисовывающая границы поля при создании объекта класса Game
	void Logic(); // функция, реализующая логику перемещения и подсчета очков
	void Input(); // функция, реализующая управление игрой с клавиатуры
	void EatFruit(); // функция, реализующая съедание фрукта
	bool CheckLosing(); // функция, реализующая проверку на поражение
};
