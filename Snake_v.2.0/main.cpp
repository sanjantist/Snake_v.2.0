#include "Game.h"

int main() {
	srand(time(NULL));
	setlocale(LC_ALL, "ru");

	Game game(20, 50);
	game.Run();

	return 0;
}