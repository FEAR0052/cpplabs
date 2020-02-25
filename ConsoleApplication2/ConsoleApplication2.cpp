#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <Windows.h>

#define UP "\n|-------------------------------------------------------------ПОДПИСКА--------------------------------------------------------------|"
#define DUP "\n|----------------------------------Исходные данные------------------------------|------------------Результаты-----------------------|"
#define SHAPKA "\n|Наим.газ.или журн.|...Тип...|Число номеров|Стоим.подп.|Стоим. дост.|Число подп.|Общ.стоим.подп.|Общ.стоим.дост.|Общ.стоим. почт.усл|"
#define LINE "\n|------------------|---------|-------------|-----------|------------|-----------|---------------|---------------|-------------------|"

struct podpiska {
	int name;
	char type[12];
	int cnomerov;
	int stpodp;
	int stdost;
	int chpodpis;
	int obshstpodp;
	int obshstdost;
	int obshstpu;

	friend std::istream& operator>>(std::istream& is, podpiska& dan);

	friend std::ostream& operator<<(std::ostream& os, podpiska& dan);

	friend std::ofstream& operator<<(std::ofstream& ofs, podpiska& dan);
} dan;

std::istream& operator>>(std::istream& is, podpiska& dan) {
	std::cout << "\n Наименование газеты или журнала: ";
	is >> dan.name;
	std::cout << "\n Тип издания(газета или журнал): ";
	is >> dan.type;
	std::cout << "\n Число номеров: ";
	is >> dan.cnomerov;
	std::cout << "\n Стоимость подписки: ";
	is >> dan.stpodp;
	std::cout << "\n Стоимость доставки на один месяц: ";
	is >> dan.stdost;
	std::cout << "\n Число подписчиков издания: ";
	is >> dan.chpodpis;
	dan.obshstpodp = dan.stpodp * dan.chpodpis;
	dan.obshstdost = dan.stdost * dan.chpodpis;
	dan.obshstpu = dan.obshstpodp + dan.obshstdost;
	return is;
}

std::ostream& operator<<(std::ostream& os, podpiska& dan) {
	char str[160];
	sprintf(str, "\n|%18d|%9.9s|%13d|%11.2d|%12.2d|%11.2d|%15.1d|%15.2d|%19.2d|", dan.name, dan.type, dan.cnomerov,
		dan.stpodp, dan.stdost, dan.chpodpis, dan.obshstpodp, dan.obshstdost, dan.obshstpu);
	str[159] = '\0';
	os << str;
	return os;
}

std::ofstream& operator<<(std::ofstream& ofs, podpiska& dan) {
	char str[160];
	sprintf(str, "\n|%18d|%9.9s|%13d|%11.2d|%12.2d|%11.2d|%15.1d|%15.2d|%19.2d|", dan.name, dan.type, dan.cnomerov,
		dan.stpodp, dan.stdost, dan.chpodpis, dan.obshstpodp, dan.obshstdost, dan.obshstpu);
	str[159] = '\0';
	ofs << str;
	return ofs;
}

struct list_EL
{
	struct podpiska m;
	struct list_EL* next;
};
struct list_EL* head = 0, * ref = 0, * last = 0;
struct podpiska z;
char dbFileName[16] = "FBase.bd";
char printFileName[16] = "Print.bd";

void del()
{
	if (head != 0)
	{
		while (head->next != 0)
		{
			ref = head->next;
			free((struct list_EL*)head);
			head = ref;
		}
		free((list_EL*)head);
		head = 0; last = 0; ref = 0;
	} return;
}

void use()
{
	char c;
	std::cout << "\n Использовать стандартный файл FBase.bd? y/n ";
	std::cin >> c;
	if (c == 'n')
	{
		std::cout << "\n Введите свое имя для BD: ";
		std::cin >> dbFileName;
	}
	else
		strcpy(dbFileName, "FBase.bd");
	return;
}

void savetofile()
{
	std::ofstream fout;
	fout.open(dbFileName, std::ios::out);
	if (!fout)
	{
		std::cerr << "\n Файл не открыт!";
		return;
	}
	for (ref = head; ref != 0; ref = ref->next)
		fout.write((char*)&(ref->m), sizeof(struct podpiska));
	fout.close(); return;
}

int add()
{
	ref = last;
	last = (list_EL*)calloc(1, sizeof(list_EL));
	if (last == 0)
	{
		std::cerr << "\n Нет памяти!";
		return 0;
	}
	else
		last->next = 0;
	if (head != 0)
	{
		ref->next = last; ref = last;
	}
	else
	{
		head = last; ref = last;
	}
	return 1;
}

void append()
{
	char c;
	do
	{
		if (add() == 0) return;
		std::cin >> ref->m;
		std::cout << "\n Продолжить ввод? y/n ";
		std::cin >> c;
	} while (c == 'y');
	savetofile(); return;
}

void create()
{
	char c;
	use();
	std::cout << "\n Ввести данные? y/n ";
	std::cin >> c;
	if (c == 'y')
		append();
	return;
}

void readfromfile()
{
	del();
	std::ifstream fin;
	fin.open(dbFileName, std::ios::in);
	if (!fin)
	{
		std::cerr << "\n Файла " << dbFileName << " не существует!"; return;
	}
	else
		std::cout << "\n Файл " << dbFileName << " открыт.";
	while (!fin.read((char*)&z, sizeof(podpiska)).eof())
	{
		if (add() == 0) return;
		else
			ref->m = z;
	}
	fin.close(); return;
}

void printab()
{
	std::ofstream fout;
	fout.open(printFileName, std::ios::out);
	if (!fout)
	{
		std::cerr << "\n Файла " << dbFileName << " не существует!"; return;
	}
	std::cout << UP << DUP << SHAPKA << LINE; fout << UP << DUP << SHAPKA << LINE;
	for (ref = head; ref != 0; ref = ref->next)
	{
		std::cout << ref->m << LINE; fout << ref->m << LINE;
	}
	fout.close(); return;
}

void otbor() {
	std::ifstream fin;
	fin.open(dbFileName, std::ios::in);
	if (!fin) {
		std::cout << "\n Файл не открыт";
		return;
	}
	std::ofstream fout;
	fout.open(printFileName, std::ios::out);
	if (!fout) {
		std::cout << "\n Файл не открыт";
		return;
	}
	std::cout << UP << DUP << SHAPKA << LINE;
	do {
		fin.read((char*)&dan, sizeof(struct podpiska));
		if (!fin.eof())
			if (dan.cnomerov >= 20) {
				std::cout << dan << LINE;
				fout << dan << LINE;
			}
	} while (!fin.eof());
	fin.close();
	fout.close();
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int key = 0;
	do
	{
		std::cout << "\n "
			<< "\n|----------МЕНЮ---------|"
			<< "\n|1.Создать файл         |"
			<< "\n|2.открыть файл         |"
			<< "\n|3.Добавить в файл      |"
			<< "\n|4.Напечатать таблицу   |"
			<< "\n|5.Отбор                |"
			<< "\n|6.Выход                |"
			<< "\n|-----------------------|" << std::endl;
		std::cout << "|Введите номер: ";
		std::cin >> key;
		switch (key)
		{
		case 1: create(); break;
		case 2: use(); readfromfile(); break;
		case 3: append(); break;
		case 4: printab(); break;
		case 5: otbor(); break;
		case 6: break;
		default: std::cerr << "\n Неверный номер!";
		}
	} while (key != 6);
	std::cout << "|Завершение..." << "\n";
}