#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <string>
#include <iomanip>

using namespace std;

void stav() // распечатывается файл info.txt с именем / фамилией
{
	string x;
	ifstream info;
	info.open("info.txt");
	cout << endl << "_________________________________" << endl;
	cout << "|                                |" << endl;
	while (getline(info, x))
		cout << x << endl;
	cout << "|________________________________|" << endl;
	info.close();
}

void Menu() { //                           
	cout << endl << "____________________________________";
	cout << endl << "|                                   |";
	cout << endl << "| Меню                              |" << endl;
	cout << "| 1. Вывод таблицы для n точек      |" << endl;
	cout << "| 2. Расчет погрешности для входа   |" << endl;
	cout << "| 3. Расчет погрешности для выхода  |" << endl;
	cout << "| 4. Сохранить результаты в файл    |" << endl;
	cout << "| 5. Закрыть                        |";
	cout << endl << "|___________________________________|" << endl;
	cout << "Ввод : ";
}

void Time(int n, double* t) { // Формирование массива времени t

	double tn = 10, tk = 100; // Начальное и конечное время
	double dt = (tk - tn) / (n - 1); // Расчет шага
	for (int i = 0; i < n; i++)
		t[i] = tn + i * dt; // расчет каждого элемента массива времени
}

void inputU(int n, double* t, double* Uin) { // Формирование массива Uin

	double tn = 10, U = 100, tk = 100, t1 = 80; // Начальные данные
	for (int i = 0; i < n; i++)
		if (t[i] <= t1)
			Uin[i] = 1.25 * t[i] - 12.5;
		else
			Uin[i] = -5 * t[i] + 500;
}

void outputU(int n, double* Uin, double* Uout) // Формирование массива Uout
{
	double U1 = 5;
	double U2 = 20;
	double U3 = 100; // Начальные данные
	for (int i = 0; i < n; i++)
		if (Uin[i] <= U1)
			Uout[i] = 0;
		else if (Uin[i] < U2)
			Uout[i] = ((U3 * (pow((Uin[i] - U1), 2))) / (pow((U2 - U1), 2)));
		else
			Uout[i] = U3;
}

void table(int n, double* t, double* Uin, double* Uout) // Вывод данных в виде таблицы
{
	cout << "N	t	Uin		Uout " << endl;
	for (int i = 0; i < n; i++) {
		cout << showpoint;
		cout << setprecision(0) << i + 1 << showpoint << setprecision(3) << "	" << t[i] << "	" << Uin[i] << "		 " << Uout[i] << endl;
	}
}

int tablefile(int n, double* t, double* Uin, double* Uout, double x, double y)
{

	string path = "File.txt";
	ofstream fout;
	fout.open(path);

	if (!fout.is_open())
		cout << "@$Error#!";

	else {
		fout << "N	t	Uin		Uout " << endl;
		for (int i = 0; i < n; i++) {
			cout << showpoint;
			fout << setprecision(0) << i + 1 << showpoint << setprecision(3) << "	" << t[i] << "	" << Uin[i] << "		 " << Uout[i] << endl;
		}

		fout << endl << "tok1: ";
		fout << x << endl;
		fout << endl << "tok2: ";
		fout << y << endl;
	}

	fout.close();
	return 0;
}

double dpulse(int n, double* U) {

	double Umax = U[0], // максимальное значение массива отсчетов сигнала
		dlit = 0; // длительность импульса

	double tn = 10, tk = 100; // начальное и конечное время
	double dt = (tk - tn) / (n - 1); // рассчет шага

	for (int i = 0; i < n; i++)
		if (U[i] > Umax)
			Umax = U[i]; // находим максимальное значение в массиве

	for (int i = 0; i < n; i++)
		if (U[i] >= 0.5 * Umax)
			dlit += dt;

	return dlit;
}

void inaccuracyI() {
	double p = 1, // Текущая погрешность
		eps = 0.01, // Заданная погрешность
		par = 1000000, // начальное значение параметра (очень большое число)
		parl = 0;
	int n = 11;
	double* pt, * pUin, * pUout;
	pt = new double[n];
	pUin = new double[n]; // инициализация динамических массивов с заданным количеством точек
	pUout = new double[n];

	while (p > eps)
	{
		pt = new double[n];
		pUin = new double[n]; // инициализация динамических массивов с заданным количеством точек
		pUout = new double[n];
		Time(n, pt);
		inputU(n, pt, pUin);
		parl = dpulse(n, pUin);
		p = fabs(par - parl) / parl;
		cout << "n = " << n << endl << "Параметр = " << parl << endl << "Погрешность = " << p << endl;
		par = parl;
		n *= 2;
	}

	delete[] pt;
	delete[] pUout;
	delete[] pUin;
}

void inaccuracyO() {
	double p = 1, // Текущая погрешность
		eps = 0.01, // Заданная погрешность
		par = 1000000, // начальное значение параметра (очень большое число)
		parl = 0;
	int n = 11;
	double* pt, * pUin, * pUout;
	pt = new double[n];
	pUin = new double[n]; // инициализация динамических массивов с заданным количеством точек
	pUout = new double[n];


	while (p > eps)
	{
		pt = new double[n];
		pUin = new double[n]; // инициализация динамических массивов с заданным количеством точек
		pUout = new double[n];
		Time(n, pt);
		inputU(n, pt, pUin);
		outputU(n, pUin, pUout);
		parl = dpulse(n, pUout);
		p = fabs(par - parl) / parl;
		cout << "n = " << n << endl << " Параметр = " << parl << endl << "Погрешность = " << p << endl;
		par = parl;
		n *= 2;
	}

	delete[] pt;
	delete[] pUout;
	delete[] pUin;
}

int main()
{
	setlocale(LC_ALL, "RUS");
	double* t, * Uin, * Uout; // Объявление массивов
	int n = -1, ns = 1;
	double x = 0, y = 0;
	double dt = 0;

	stav();

	while (ns < 5 && ns > 0) {
		Menu();
		cin >> ns;
		switch (ns)
		{
		case 1:
			cout << "Введите необходимое количество точек: ";	// Ввод необходимого количества точек
			cin >> n;
			t = new double[n];
			Uin = new double[n]; // инициализация динамических массивов с заданным количеством точек
			Uout = new double[n];
			Time(n, t); // формирование массива t
			inputU(n, t, Uin); // формирование массива Uin
			outputU(n, Uin, Uout); // формирование массива Uout
			table(n, t, Uin, Uout); // Вывод всех массивов
			cout << endl << "inputU: ";
			cout << dpulse(n, Uin) << endl;
			cout << endl << "outputU: ";
			cout << dpulse(n, Uout) << endl;
			delete[] t;
			delete[] Uout;
			delete[] Uin;
			break;
		case 2:
			inaccuracyI();
			break;
		case 3:
			inaccuracyO();
			break;
		case 4:
			cout << "Введите необходимое количество точек: ";	// Ввод необходимого количества точек
			cin >> n;
			t = new double[n];
			Uin = new double[n]; // инициализация динамических массивов с заданным количеством точек
			Uout = new double[n];
			Time(n, t); // формирование массива t
			inputU(n, t, Uin); // формирование массива Uin
			outputU(n, Uin, Uout); // формирование массива Uout
			x = dpulse(n, Uin);
			y = dpulse(n, Uout);

			tablefile(n, t, Uin, Uout, x, y);

			delete[] t;
			delete[] Uout;
			delete[] Uin;
			break;
		default:
			break;
		}
	}

	return 0;
}