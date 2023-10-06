#include "Game.h"

void gotoxy(int x, int y) {
	COORD coordinates;
	coordinates.X = x;
	coordinates.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

bool operator ==(Point first, Point second) {
	if (first.x == second.x && first.y == second.y) {
		return true;
	}
	return false;
}

Game::Game(int height, int width)
{
	// передача параметров высоты и ширины в поля класса
	height_ = height;
	width_ = width;
	// игра начинается в центре поля
	head_position_.x = width / 2;
	head_position_.y = height / 2;
	// заполнение всех свободных полей вектора
	for (int i = 2; i < width_; ++i) {
		for (int j = 2; j < height_; ++j) {
			Point temp = { i,j };
			fruit_possible_.push_back(temp);
		}
	}
	// фрукт появляется в случайном свободном месте поля
	fruit_position_ = fruit_possible_[rand() % fruit_possible_.size()];
	// остальные базовые параметры
	game_over_ = false;
	score_ = 0;
	dir_ = STOP;
	DrawField();
}

Game::~Game()
{
	gotoxy(0, height_ + 7);
	std::cout << "Game over\n" << "Score: " << score_ << std::endl;
	std::cout << "Для выхода нажмите любую кнопку";
	_getch();
}

void Game::DrawField()
{
	// прорисовка верхней границы
	for (int i = 1; i <= width_; ++i) {
		gotoxy(i, 1);
		std::cout << "#";
	}

	// прорисовка боковых границ
	for (int i = 1; i < height_; ++i) {
		for (int j = 1; j <= width_; ++j) {
			if (j == 1 || j == width_) {
				gotoxy(j, i);
				std::cout << "#";
			}
		}
	}

	// прорисовка нижней границы
	for (int i = 1; i <= width_; ++i) {
		gotoxy(i, height_);
		std::cout << "#";
	}

	// прорисовка фрукта и головы
	gotoxy(fruit_position_.x, fruit_position_.y);
	std::cout << "F";
	gotoxy(head_position_.x, head_position_.y);
	std::cout << "D";

	// Выведение начальной информации на экран
	gotoxy(0, height_ + 2);
	std::cout << "Score: " << score_;
	gotoxy(0, height_ + 3);
	std::cout << "Press 'x' to exit";
}

void Game::Logic()
{
	gotoxy(head_position_.x, head_position_.y);
	std::cout << " ";

	prev_head_ = head_position_;

	if (!tail_.empty()) {
		for (auto part : tail_) {
			gotoxy(part.x, part.y);
			std::cout << " ";
		}
		last_tail_part_ = tail_[tail_.size() - 1];
		if (tail_.size() > 1) {
			for (int i = tail_.size() - 1; i > 0; --i) {
				tail_[i] = tail_[i - 1];
			}
		}
		tail_[0] = head_position_;
	}

	switch (dir_)
	{
	case LEFT:
		--head_position_.x;
		break;
	case RIGHT:
		++head_position_.x;
		break;
	case UP:
		--head_position_.y;
		break;
	case DOWN:
		++head_position_.y;
		break;
	}

	auto it = find(fruit_possible_.begin(), fruit_possible_.end(), head_position_);
	if (it == fruit_possible_.end()) {
		game_over_ = true;
		return;
	}
	else {
		fruit_possible_.erase(it);
	}

	if (!tail_.empty()) {
		for (auto part : tail_) {
			if (head_position_ == part) {
				game_over_ = true;
				return;
			}
		}

		if (head_position_==fruit_position_) {
			EatFruit();
			tail_.push_back(last_tail_part_);
		}
		else {
			fruit_possible_.push_back(last_tail_part_);
		}
	}
	else {
		if (head_position_ == fruit_position_) {
			EatFruit();
			tail_.push_back(prev_head_);
		}
		else {
			fruit_possible_.push_back(prev_head_);
		}
	}

	gotoxy(head_position_.x, head_position_.y);
	std::cout << "D";

	for (auto part : tail_) {
		gotoxy(part.x, part.y);
		std::cout << "o";
	}

	if (CheckLosing()) {
		game_over_ = true;
	}
}

void Game::Input()
{
	if (_kbhit()) {
		switch (_getch())
		{
		case 'x':
			game_over_ = true;
			break;
		case 'a':
			dir_ = LEFT;
			break;
		case 'd':
			dir_ = RIGHT;
			break;
		case 'w':
			dir_ = UP;
			break;
		case 's':
			dir_ = DOWN;
			break;
		case 'X':
			game_over_ = true;
			break;
		case 'A':
			dir_ = LEFT;
			break;
		case 'D':
			dir_ = RIGHT;
			break;
		case 'W':
			dir_ = UP;
			break;
		case 'S':
			dir_ = DOWN;
			break;
		}
	}
}

void Game::EatFruit()
{
	gotoxy(fruit_position_.x, fruit_position_.y);
	std::cout << " ";

	fruit_position_ = fruit_possible_[rand() % fruit_possible_.size()];

	gotoxy(fruit_position_.x, fruit_position_.y);
	std::cout << "F";

	++score_;

	gotoxy(0, height_ + 2);
	std::cout << "Score: " << score_;
}

bool Game::CheckLosing()
{
	for (auto part : tail_) {
		if (head_position_.x == part.x && head_position_.y == part.y) {
			return true;
		}
	}
	return (head_position_.x == 1 || head_position_.x == width_ ||
		head_position_.y == 1 || head_position_.y == height_);
}

void Game::Run()
{
	while (!game_over_) {
		Input();
		Logic();
		Sleep(100);
	}
}
