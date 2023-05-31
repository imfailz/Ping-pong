#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

int gridHeight = 25;
int gridWidth = 60;
struct Ball
{
	int init_BallX;
	int init_BallY;
	int BallX;
	int BallY;
	char ball[1][1];
	char n_ball[1][1];
	int ballSpeed;
};
struct Player
{
	int speed;
	int _PaddleSize;
	char* _Paddle;
	int _PaddlePosition;
	int wallDistance;
	int score;
};
void gotoxy(int column, int line); // создание игрового поля
void startPos(Ball&, Player& pl1, Player& pl2); // начальное положение "площадок"
void movement(Player& p, bool flag); //движение "площадок"
void PaddleArr(Player& p, int wallDistance);
void readButtons(Player& pl1, Player& pl2); // нажатие кнопок
void _showGrid();
void createHorizontalLine(int lineNo); //создание горизонтальных линий
void createVerticalLine(int colNo); //создание вертикальных линий
bool checkPaddlePos(int x, int p); //положение площадок во время игры
void bounce(Ball& B, Player pl1, Player pl2, bool& Direction, bool& UP, bool& DOWN, bool&
	paddle, bool& LEFT, bool& RIGHT); // отскакивание шарика от стенок
bool Score(Ball B, Player& pl1, Player& pl2); // подсчет очков
void reset(Ball& B, Player& pl1, Player& pl2, bool& Direction, bool& UP, bool& DOWN, bool& paddle, bool& LEFT, bool& RIGHT); //сброс (возвращение "площадок" и шарика в начальное положение)
char difficulty(); //сложность игры
void aboutGame(); // информация об игре
void wait(int w);
void showScore(Player pl1, Player pl2); // вывод счета на экран
int main()
{
	Player pl1, pl2; // обозначаем игроков
	Ball B; // обозначаем мяч
	system("mode 61, 30");
	aboutGame();
	system("pause >nul");
	system("cls");
	startPos(B, pl1, pl2);
	system("cls");
	showScore(pl1, pl2);
	_showGrid();
	PaddleArr(pl1, pl1.wallDistance);
	PaddleArr(pl2, pl2.wallDistance);
	bool UP = true;
	bool DOWN = false;
	bool Direction = UP;
	bool LEFT = true;
	bool RIGHT = false;
	bool paddle = LEFT;
	while (true)
	{
		gotoxy(B.init_BallX += B.BallX, B.init_BallY += B.BallY);
		cout << B.ball[0][0];
		if (Score(B, pl1, pl2))
		{
			if (pl1.score == 10) // если счет первого игрока достигнул 10 очков
			{
				showScore(pl1, pl2);
				gotoxy(10, gridHeight / 2 - 1);
				cout << "Player 1 wins"; // выводим сообщение о победе
				gotoxy(10, gridHeight / 2);
				cout << "GameOver"; //выводим сообщение об окончании игры
				break;
			}
			else if (pl2.score == 10) // если счет второго игрока достигнул 10 очков
			{
				showScore(pl1, pl2);
				gotoxy(40, gridHeight / 2 - 1);
				cout << "Player 2 wins"; // выводим сообщение о победе
				gotoxy(40, gridHeight / 2);
				cout << "GameOver"; //выводим сообщение об окончании игры
				break;
			}
			else
			{
				system("cls");
				reset(B, pl1, pl2, Direction, UP, DOWN, paddle, LEFT, RIGHT);
				_showGrid();
				PaddleArr(pl2, pl2.wallDistance);
				PaddleArr(pl1, pl1.wallDistance);
				showScore(pl1, pl2);
				system("pause >nul");
			}
		}
		bounce(B, pl1, pl2, Direction, UP, DOWN, paddle, LEFT, RIGHT);
		if (B.init_BallX == gridWidth / 2 - 1)
		{
			for (int y = 0; y <= gridHeight; y++)
			{
				if ((y + 1) % 2 == 0)
				{
					gotoxy(gridWidth / 2 - 1, y);
					cout << char(-37);
				}
			}
		}
		gotoxy(B.init_BallX + B.BallX, B.init_BallY + B.BallY);
		cout << B.n_ball[0][0];
		readButtons(pl1, pl2);
		wait(B.ballSpeed);
	}
	gotoxy(gridWidth - 42, gridHeight - 1);
	return 0;
}
// счетчик очков
void showScore(Player pl1, Player pl2)
{
	gotoxy(1, gridHeight + 2);
	cout << "Player 1 Score: " << pl1.score;
	gotoxy(gridWidth - 16, gridHeight + 2);
	cout << "Player 2 Score: " << pl2.score;
}
void wait(int w)
{
	for (int j = 0; j < w; j++)
		for (int i = 0; i < 9999999; i++);
}
// "отрисовываем" окружение
void gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}
// описываем начальное положение площадок
void startPos(Ball& B, Player& pl1, Player& pl2)
{
	pl1.score = 0;
	pl2.score = 0;
	pl1.speed = 2;
	pl2.speed = 2;
	B.ballSpeed = difficulty() - 48;
	pl1.wallDistance = 2;
	pl2.wallDistance = gridWidth - 2;
	pl1._PaddleSize = 4;
	pl2._PaddleSize = 4;
	pl1._Paddle = new char[gridHeight];
	pl2._Paddle = new char[gridHeight];
	B.init_BallX = gridWidth / 2;
	B.init_BallY = gridHeight / 2;
	B.BallX = 1;
	B.BallY = 1;
	B.ball[0][0] = ' ';
	B.n_ball[0][0] = 'O';
	for (int i = 0; i < gridHeight; i++)
	{
		pl1._Paddle[i] = ' ';
		pl2._Paddle[i] = ' ';
	}
	pl1._PaddlePosition = gridHeight / 2 - (pl1._PaddleSize / 2);
	pl2._PaddlePosition = gridHeight / 2 - (pl2._PaddleSize / 2);
	for (int i = pl1._PaddlePosition; i < pl1._PaddlePosition + pl1._PaddleSize; i++)
		pl1._Paddle[i] = char(-37);
	for (int i = pl2._PaddlePosition; i < pl2._PaddlePosition + pl2._PaddleSize; i++)
		pl2._Paddle[i] = char(-37);
}
//движение площадок
void movement(Player& p, bool flag)
{
	if (flag)
	{
		for (int i = 0; i < p.speed; i++)
		{
			if (p._PaddlePosition >= 2)
			{
				p._PaddlePosition--;
				p._Paddle[p._PaddlePosition + p._PaddleSize] = ' ';
				p._Paddle[p._PaddlePosition] = char(-37);
			}
		}
	}
	else
	{
		for (int i = 0; i < p.speed; i++)
		{
			if (p._PaddlePosition < (gridHeight - p._PaddleSize))
			{
				p._Paddle[(p._PaddlePosition++) + p._PaddleSize] = char(-37);
				p._Paddle[p._PaddlePosition - 1] = ' ';
			}
		}
	}
}
//прорисовка площадок
void PaddleArr(Player& p, int wallDistance)
{
	for (int i = 1; i < gridHeight; i++)
	{
		gotoxy(wallDistance, i);
		cout << p._Paddle[i];
	}
}
void createHorizontalLine(int lineNo)
{
	for (int x = 1; x < gridWidth; x++)
	{
		gotoxy(x, lineNo);
		cout << char(-37);
	}
}
void createVerticalLine(int colNo)
{
	for (int y = 0; y <= gridHeight; y++)
	{
		gotoxy(colNo, y);
		cout << char(-37);
	}
}
void _showGrid()
{
	createHorizontalLine(0);
	createHorizontalLine(gridHeight);
	for (int y = 0; y <= gridHeight; y++)
	{
		if ((y + 1) % 2 == 0)
		{
			gotoxy(gridWidth / 2 - 1, y);
			cout << char(-37);
		}
	}
	createVerticalLine(0);
	createVerticalLine(gridWidth);
}
//задаем наименования кнопкам управления
void readButtons(Player& pl1, Player& pl2)
{
	bool UP = false;
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'w':
			UP = true;
			movement(pl1, UP);
			PaddleArr(pl1, pl1.wallDistance);
			break;
		case 'p':
			UP = true;
			movement(pl2, UP);
			PaddleArr(pl2, pl2.wallDistance);
			break;
		case 's':
			UP = false;
			movement(pl1, UP);
			PaddleArr(pl1, pl1.wallDistance);
			break;
		case 'l':
			UP = false;
			movement(pl2, UP);
			PaddleArr(pl2, pl2.wallDistance);
			break;
		case 27:
			system("cls");
			exit(0);
			break;
		}
	}
}
bool checkPaddlePos(int x, int p)
{
	for (int i = p; i < p + 4; i++)
		if (i == x)
			return true;
	return false;
}
//прописываем условие «отскакивания» шарика от поверхностей
void bounce(Ball& B, Player pl1, Player pl2, bool& Direction, bool& UP, bool
	& DOWN, bool& paddle, bool& LEFT, bool& RIGHT)
{
	if (B.init_BallY >= gridHeight - 1 && paddle == LEFT)
	{
		B.BallX = 1;
		B.BallY = -1;
		Direction = DOWN;
	}
	if (B.init_BallY >= gridHeight - 1 && paddle == RIGHT)
	{
		B.BallX = -1;
		B.BallY = -1;
		Direction = DOWN;
	}
	if (B.init_BallY <= 1 && paddle == RIGHT)
	{
		B.BallX = -1;
		B.BallY = 1;
		Direction = UP;
	}
	if (B.init_BallY <= 1 && paddle == LEFT)
	{
		B.BallX = 1;
		B.BallY = 1;
		Direction = UP;
	}
	if (B.init_BallX <= 3 && checkPaddlePos(B.init_BallY, pl1._PaddlePosition) &&
		Direction == DOWN)
	{
		B.BallX = 1;
		B.BallY = -1;
		paddle = LEFT;
	}
	if (B.init_BallX <= 3 && checkPaddlePos(B.init_BallY, pl1._PaddlePosition) &&
		Direction ==
		UP)
	{
		B.BallX = 1;
		B.BallY = 1;
		paddle = LEFT;
	}
	if (B.init_BallX >= gridWidth - 3 && checkPaddlePos(B.init_BallY, pl2._PaddlePosition)
		&&
		Direction == DOWN)
	{
		B.BallX = -1;
		B.BallY = -1;
		paddle = RIGHT;
	}
	if (B.init_BallX >= gridWidth - 3 && checkPaddlePos(B.init_BallY, pl2._PaddlePosition)
		&&
		Direction == UP)
	{
		B.BallX = -1;
		B.BallY = 1;
		paddle = RIGHT;
	}
}
bool Score(Ball B, Player& pl1, Player& pl2)
{
	if (B.init_BallX >= gridWidth - 2)
	{
		pl1.score++;
		return true;
	}
	else if (B.init_BallX <= 1)
	{
		pl2.score++;
		return true;
	}
	else
		return false;
}
char difficulty()
{
	char c;
	while (1)
	{
		system("cls");
		cout << "\n\n\n\n\n\n\n";
		cout << "\t\t\t Select difficulty";
		cout << "\n\t ";
		gotoxy(gridWidth / 2 - 5, gridHeight / 2);
		cout << "1) Pro";
		gotoxy(gridWidth / 2 - 5, gridHeight / 2 + 1);
		cout << "2) Hard";
		gotoxy(gridWidth / 2 - 6, gridHeight / 2 + 2);
		cout << "3) Medium";
		gotoxy(gridWidth / 2 - 5, gridHeight / 2 + 3);
		cout << "4) Easy";
		gotoxy(gridWidth / 2 - 6, gridHeight / 2 + 5);
		cout << "Press: ";
		c = _getch();
		if (c - 48 == 1 || c - 48 == 2 || c - 48 == 3 || c - 48 == 4)
			return c;
	}
}
void aboutGame()
{
	cout << "\n\n\n\n";
	cout << "\n\t Ping Pong is ";
	cout << "\n\t one of the first cooperative arcades.";
	cout << "\n\t ";
	cout << "\n\t Call a friend to play together.";
	cout << "\n\t ";
	cout << "\n\t Еhe goal of the game is to score 10 points ";
	cout << "\n\t faster than your opponent";
	cout << "\n\t ";
	cout << "\n\t Good luck";
	cout << "\n\n\t Controls :-";
	cout << "\n\t Player 1: ";
	cout << "\n\t UP -> w";
	cout << "\n\t DOWN -> s";
	cout << "\n\t Pleayer 2: ";
	cout << "\n\t UP -> p";
	cout << "\n\t DOWN -> l";
}
void reset(Ball& B, Player& pl1, Player& pl2, bool& Direction, bool& UP, bool& DOWN, bool
	& paddle, bool& LEFT, bool& RIGHT)
{
	UP = true;
	DOWN = false;
	Direction = UP;
	LEFT = true;
	RIGHT = false;
	paddle = LEFT;
	B.init_BallX = gridWidth / 2;
	B.init_BallY = gridHeight / 2;
	B.BallX = 1;
	B.BallY = 1;
	pl1._PaddlePosition = gridHeight / 2 - (pl1._PaddleSize / 2);
	pl2._PaddlePosition = gridHeight / 2 - (pl2._PaddleSize / 2);
	for (int i = 0; i < gridHeight; i++)
	{
		pl1._Paddle[i] = ' ';
		pl2._Paddle[i] = ' ';
	}
	for (int i = pl1._PaddlePosition; i < pl1._PaddlePosition + pl1._PaddleSize; i++)
		pl1._Paddle[i] = char(-37);
	for (int i = pl2._PaddlePosition; i < pl2._PaddlePosition + pl2._PaddleSize; i++)
		pl2._Paddle[i] = char(-37);
}
