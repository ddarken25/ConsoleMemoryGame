#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h>
using namespace std;
// Создаём структуру карты. 
struct card {
	int value = 0;  // Значение внутри карты
	bool open = false;   // Закрыта или открыта
	int active = 0;      // 0 - белый цвет(обычная карта), 1 - зелёный цвет (в фокусе пользователя), 2 - желтый (карта угадана)
};
// Первое меню игры
int choiseMenu(const char* title, const char** selects, int size) {
	int key;  // Переменная для считываения значений нажатых клавиш
	int choiseMain = 0;  // Значение выбора пользователя. В конце его и вернём
	do {
		system("cls");
		// Две строки для подсвечивания активного выбора нужным цветом
		const char* green = "\033[38;5;010m";
		const char* white = "\033[38;5;015m";
		// Выводим заголовок
		cout << title;
		// Выводим пункты меню
		for (int i = 0; i < size; i++) {
			if (i == choiseMain) {    // Если выбор пользователя равен пункту меню
				cout << green;        // Красим пункт в зелёный
			}
			cout << "\t\t\t\t" << selects[i] << endl; // Вывод самого пункта
			cout << white;  // Возвращаем белый цвет
		}
		key = _getch();   // Считываем клавишу (если 224, значит не правильно считали, нужно еще раз)
		if (key == 224) key = _getch();
		switch (key)
		{
		case 72:  // Клавиша вверх
			if (choiseMain == 0) continue;   // Если вверх больше некуда, то ничего и не делаем
			choiseMain--;  // Уменьшаем значение выбора (красим нижний пункт меню)
			break;
		case 80: // Клавиша вниз
			if (choiseMain == size - 1) continue;   // Если вниз больше некуда, то ничего и не делаем
			choiseMain++;  // Увеличиваем значение выбора (красим вверхний пункт меню)
			break;
		case 27: // Esc
			choiseMain = size - 1; // Если Esc то уходим
			break;
		}
		// Продолжаем цикл пока не нажата Enter или Esc
	} while (key != 13 && key != 27);
	return choiseMain;
}
// Функция для заставки. Побуквенно меняет цвет
void changeColorAnimation(const char* startTitle, const char* color, const char* backgroundColor) {
	for (int j = 0; j < strlen(startTitle); j++) {
		system("cls");
		cout << "\n\t\t\t\t";
		for (int i = 0; i < strlen(startTitle); i++) {
			if (i <= j) {
				cout << backgroundColor;
				cout << color;
				cout << startTitle[i];
				cout << "\033[0m";
			}
			else {
				cout << startTitle[i];
			}
		}
		Sleep(100);
	}
}
// Создание игрового поля, в зависимости от выбора пользователя
void choiseMapCreate(int& row, int& col, int choise) {
	switch (choise)
	{
	case 0:
		row = 2;
		col = 4;
		break;
	case 1:
		row = 3;
		col = 4;
		break;
	case 2:
		row = 4;
		col = 4;
		break;
	case 3:
		row = 5;
		col = 4;
		break;
	case 4:
		cout << "\t\t\tВведите количество строк: ";
		cin >> row;
		cout << "\t\t\tВведите количество столбцов: ";
		cin >> col;
		while ((row * col > 180) || (row * col) % 2 != 0) {
			cout << "\t\tНет возможности создать карту с заданными параметрами. Попробуйте еще раз: \n";
			cout << "\t\t\tВведите количество строк: ";
			cin >> row;
			cout << "\t\t\tВведите количество столбцов: ";
			cin >> col;
		}
		break;
	}
}
// Вывод на экран игрового поля
void showmap(card** map, int row, int col) {
	system("cls");
	const char* colorActive = "\033[48;5;002m";
	const char* colorNotActive = "\033[48;5;015m";
	const char* colorDone = "\033[48;5;003m";
	for (int i = 0; i < row; i++)
	{
		cout << "\n\t\t\t\t\t";
		for (int j = 0; j < col; j++) {
			// Вывод массива на экран, в зависимости от его значение active
			switch (map[i][j].active)
			{
			case 0:
				// Обычная белая карта
				cout << colorNotActive << "| ";
				break;
			case 1:
				// Зеленая карта, под прицелом пользователя
				cout << colorActive << "| ";
				break;
			case 2:
				// Желтая угаданная карта
				cout << colorDone << "| ";
				break;
			}
			// В зависимости, открытая карта или нет, вывод значения карт, нужным цветом 
			// Если карта открыта, цвет значения будет черным.
			if (map[i][j].open) {
				cout << "\033[38;5;000m" << map[i][j].value << "\033[38;5;000m\033[48; 5; 000m |";
			}
			// Если карта закрыта, то надо проверить, в фокусе карта или нет. В первом случае цвет значения должен быть зелёным, во втором - белым
			else {
				if (map[i][j].active == 1) {
					cout << "\033[38;5;002m" << map[i][j].value << "\033[38;5;000m\033[48; 5; 000m |";
				}
				else {
					cout << "\033[38;5;015m" << map[i][j].value << "\033[38;5;000m\033[48; 5; 000m |";
				}
			}
		}
		cout << "\n\t\t\t\t\t";
		for (int j = 0; j < col; j++) {
			cout << "\033[0m------";
		}
		cout << "\033[0m";
	}
}
// Заполнение массива числами от 10
void putArray(card** map, int row, int col) {
	// Счетчик пар
	int countPair = 0;
	// Первое число
	int pair = 10;
	// Заполняем массив
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++) {
			map[i][j].value = pair;    // Присваем значение первому числу
			countPair++;               // Увеличиваем счётчик пар
			// Если пара создана, увеличиваем начальное число и возвращаем счетчик пар на 0
			if (countPair == 2) {
				pair++;
				countPair = 0;
			}
		}
	}
}
// Перемешивание массива
void shakeArray(card** map, int row, int col) {
	int temp; // Для запоминания
	int x, y; // Для новых случайных координат
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++) {
			// Выбираем случайным элемент массива
			x = rand() % row;
			y = rand() % col;
			// Запоминаем элемент массива
			temp = map[i][j].value;
			// Присваем значение случайного элемента
			map[i][j].value = map[x][y].value;
			// В выбранный случайный элемент присваем значение из первого
			map[x][y].value = temp;
		}
	}
}
// Игровой процесс: передвижение по карте, открытие карт, проверка на совпадение, счётчик побед, ошибок и времени
void game(card** map, int row, int col) {
	// Запоминаем время начала игры
	int startTime = clock();
	int x = 0, y = 0;  // Координаты для передвижения по карте
	int tempX, tempY;  // Для запоминания координат уже открытой первой карты, чтобы в дальнейшем её сравнить со второй
	// Счётчики нажатий, ошибок, готовых пар, и выбора пользователя в меню паузы
	int clickCount = 0, counterError = 0, pairsDone = 0, choiseUserPause = 0;
	int key; // Для считывания нажатых клавиш
	int tempActive; // Для правильного отображение "активной" (зелёной) карты
	// Запускаем цикл, пока пользователь не решит выйти или не соберет все пары
	do {
		tempActive = map[y][x].active; // Запоминаем показатель активности карты, которая находится в фокусе
		map[y][x].active = 1;  // Меняем это значение на 1 (зелёный цвет)
		showmap(map, row, col); // Выводим карту
		map[y][x].active = tempActive;  // Возвращаем первоначальное значение активности карте.
		key = _getch(); // Считываем клавишу
		if (key == 224) key = _getch();
		// В зависимости от нажатых стрелок, двигаем прицел, и проверяем, чтобы не вышел за границы карты
		switch (key)
		{
		case 77:
			if (x + 1 >= col) {
				continue;
			}
			x++;
			break;
		case 75:
			if (x <= 0) {
				continue;
			}
			x--;
			break;
		case 72:
			if (y <= 0) {
				continue;
			}
			y--;
			break;
		case 80:
			if (y + 1 >= row) {
				continue;
			}
			y++;
			break;
		case 13: // При нажатии Enter
			// Проверяем, не открыта ли уже это карта, если открыта, ничего дальше не делаем, возвращаемсчя в начало цикла
			if (map[y][x].open) {
				continue;
			}
			// Открываем карту.
			map[y][x].open = true;
			// Если это была первая открытая карта: 
			if (clickCount == 0) {
				// Запоминаем её координаты, чтобы потом её проверить со следующей открытой картой
				tempX = x;
				tempY = y;
				// Увеличиваем счётчик карт
				clickCount++;
			}
			// Если это вторая открытая карта
			else {
				// Сравниваем значения первой и второй карты
				if (map[tempY][tempX].value == map[y][x].value) {
					// Если значения совпали, присваеваем значение в active 2 (желтый цвет)
					map[y][x].active = 2;
					map[tempY][tempX].active = 2;
					// Проигрываем звук
					Beep(500, 100);
					Beep(550, 100);
					Beep(500, 100);
					Beep(550, 100);
					Beep(500, 100);
					// Увеличиваем счётчик готовых пар
					pairsDone++;
				}
				// Если карта не совпала
				else {
					// Выводим на экран поле с двумя открытыми разными картами
					showmap(map, row, col);
					// Закрываем обе карты
					map[y][x].open = false;
					map[tempY][tempX].open = false;
					// Проигрываем другой звук
					Beep(460, 300);
					Beep(340, 300);
					Beep(420, 300);
					Beep(300, 600);
					// Считаем ошибки
					counterError++;
				}
				// Переводим счётчик нажатий на 0
				clickCount = 0;
			}
			break;
		case 27:  // Esc
		{
			// Выводим меню паузы, там пользователь решит, проджолжить или выйтив меню
			const char* selectPause[2] = { "\tПродолжить","\tВыход" };
			choiseUserPause = choiseMenu("\n\t\t\t\tПауза:\n\n",selectPause , 2);
			break;
		}
		}
		// Проверка, не отгадал ли пользователь все карты, или выбрал выход в меню, если нет, то начинаем цикл снова
	} while (pairsDone != (row * col / 2) && choiseUserPause == 0);

	// Когда цикл отработал, проверяем, угадал ли пользователь все пары (если просто нажал на выход, то ничего не делаем)
	if (pairsDone == (row * col / 2)) {
		// Считаем время игры в секундах
		int Time = (clock() - startTime) / 1000;
		// Выводим мини заставку "Победа"
		const char* backgroundColors[4] = { "\033[48;5;014m", "\033[48;5;013m", "\033[48;5;195m", "\033[48;5;001m" };
		const char* colors[4] = { "\033[38;5;000m", "\033[38;5;014m", "\033[38;5;003m", "\033[38;5;000m" };
		for (int i = 0; i < 4; i++) {
			changeColorAnimation("\t\tПОБЕДА!", colors[i], backgroundColors[i]);
		}
		// Показываем победное поле и небольшую статистику (время количество ошибок)
		showmap(map, row, col);
		cout << "\n\t\t\t\t\t\tПОБЕДА!";
		cout << "\n\t\t\t\t\tВремя: " << Time << " секунд";
		cout << "\n\t\t\t\t\tКоличество ошибок: " << counterError << "\n\n\n";
		system("pause");
	}
}

void rules() {
	system("cls");
	cout << "\t\t\t\tПравила для игры в Память:\n\n";
	cout << "Выберите размер поля. Можно выбрать из стандартных размеров, а можно задать размер самостоятельно.\n";
	cout << "При ручном вводе размера поля, нужно учитывать, что указанные количества строк и столбцов\n";
	cout << "должны позволять сформировать чётное количество пар. А так же они не должны быть равными 0.\n";
	cout << "После выбора размера поля начнётся игра. Вам нужно ходить по полю, используя стрелки, и открывать\n";
	cout << "карточки, нажав Enter.\n";
	cout << "После открытия одной карточки, вам нужно открыть вторую карточку, и, если значение в них совпадут, то\n";
	cout << "карточки не закроются и будут подсвечены. В противном случае карточки закроются, и придётся снова искать\n";
	cout << "правильную пару.\n\n";
	system("pause");
}
void author() {
	system("cls");
	cout << "\n\t\t\t\tАвтор: студент группы СБД113 - Вощакин Григорий\n\n\n\n";
	system("pause");
}
// Начальная заставка
void screen() {
	const char* startTitle = "\tПАМЯТЬ";
	cout << "\n\t\t\t\t";
	for (int i = 0; i < strlen(startTitle); i++) {
		cout << startTitle[i];
		Sleep(100);
	}
	const char* backgroundColors[4] = { "\033[48;5;014m", "\033[48;5;013m", "\033[48;5;195m", "\033[48;5;001m" };
	const char* colors[4] = { "\033[38;5;000m", "\033[38;5;014m", "\033[38;5;003m", "\033[38;5;000m" };
	for (int i = 0; i < 4; i++) {
		changeColorAnimation(startTitle, colors[i], backgroundColors[i]);
	}
	Sleep(1000);
	system("cls");
	cout << "\n\t\t\t\t";
	cout << "\033[48;5;001m" << startTitle << "\033[0m";
	Sleep(1000);
}
int main()
{
	setlocale(0, "");
	srand(time(0));   
	int row = 0, col = 0;  // Количество строк и столбцов, изначально приравниваем к нулю.
	// мини-заставка
	screen();
	// Массив строк для меню
	const char* selectMenu[4] = { "\tИграть", "\tПравила", "\tАвтор","\tВыход" };
	// Переменная для хранения результата выбора
	int choiseUserMenu;
	do {
		// Выводим меню на экран и считываем результат
		choiseUserMenu = choiseMenu("\n\t\t\t\t\tИГРА ПАМЯТЬ\n\n", selectMenu, 4);
		// В зависимости от выбора пункта меню, действуем: 
		switch (choiseUserMenu)
		{
			// Если выбран пункт "Играть"
		case 0: {
			// Пункты для меню выбора поля
			const char* selectMap[6] = { "\t2 х 4", "\t3 х 4", "\t4 х 4","\t5 х 4","Указать свой размер поля", "\tНазад" };
			// Вывод меню выбора поля
			int choiseUserMap = choiseMenu("\n\t\t\t\tВыберите размер поля:\n\n", selectMap, 6);
			// Создаём поле, в зависимости от выбора пользователя
			choiseMapCreate(row, col, choiseUserMap);
			// Проверяем условия: строки и столбцы не должны быть равны нулю, и было выбрано хоть какое-то поле
			if ((row != 0 && col != 0) && choiseUserMap != 5) {
				// Создаём двумерный массив из структуры card
				card** map = new card * [row]; // Выделяем память для количества строк
				for (int i = 0; i < row; i++) {
					map[i] = new card[col];    // В каждой строке выделяем память для столбцов
				}
				// Заполняем массив
				putArray(map, row, col);
				// Перемешиваем массив
				shakeArray(map, row, col);
				// Запускаем игровоц процесс
				game(map, row, col);

				// После игры удаляем память под массив
				for (int i = 0; i < row; i++) {
					delete[] map[i];
				}
				delete[] map;
			}
			break;
		}
		case 1:
			rules();
			break;
		case 2:
			author();
			break;

		}
	} while (choiseUserMenu != 3);
}
