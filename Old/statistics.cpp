// statistics++.cpp: определяет точку входа для консольного приложения.
//

//#include "stdafx.h"
//#include "iostream"
#include <fstream>
#include <string>
#include <cmath>
#include "stdio.h"
#include "stdlib.h"
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

// Структура, как для среднего значения, так и для среднего квадратичного
// Используем массив таких структур из number_of_timesteps элементов
struct value {
	double minimum = 0;
	double max = 0;
	double average = 0;
	double width = 0;
	double utilization = 0;
};

// Считаем среднее
void average(value &sr);
// Берем корень из суммы квадратов отклонений на number_of_timesteps;
void root(value &error);
// Функция, которая добавляет значения из файлов:
void add(ifstream &in, value &sr, value &error);

void count(value &sr, value &error);
// Указываем путь к файлу из которого будем читать 
string give_path(const int i);
// Выписываем в файл
void write(ofstream &out, size_t j, value Val, value Val2);

// Количество проведенных тестов
const size_t number_of_simulation = 1000;
// Количество шагов в одной симуляции
const size_t number_of_timesteps = 100000;



int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);
	double r = 1.0;
	double p = atof(argv[1]);
	size_t number_of_LP = 1024;
	mkdir(("Result" + to_string(p)).c_str(), 0777);
	ofstream OUT("OUT" +to_string(p)+".txt", ios_base::trunc);
	for (double q = 0; q < 1.1; q += 0.1) {

		string name = "N";
		name += to_string(number_of_LP);
		name += "M";
		name += to_string(number_of_timesteps);
		name += "r";
		name += to_string(r);
		name += "q";
		name += to_string(q);
		name += "p";
		name += to_string(p);
		name +="/";
OUT << name << std::endl;
		// Массивы средних значений, для каждого шага 
		value sr[number_of_timesteps];
		value error[number_of_timesteps];
		// Добавляем все значения на каждом шаге
		for (int i = 0; i < number_of_simulation; i++) {
			ifstream in(name+give_path(i), ios_base::in);
			for (size_t j = 0; j < number_of_timesteps; j++)
			{
				add(in, sr[j], error[j]);
			}
			in.close();
		}
		// Высчитываем среднее значение на каждом шаге и записываем в файл
		for (size_t j = 0; j < number_of_timesteps; j++)
		{
			// Делим на количество симуляций
			average(sr[j]);
			average(error[j]);
			// Вычитаем из суммы квадратов деленных на K квадрат суммы деленной на К
			count(sr[j], error[j]);
		}
		string ut = "Result" + to_string(p)+"/result";
		ut += to_string(q);
		ut += ".txt";
		ofstream out(ut, ios_base::trunc);
		// Считаем корень из суммы квадратов отклонений на number_of_timesteps;
		// И сразу пишем в файл
		for (size_t j = 0; j < number_of_timesteps; j++)
		{
			// Берем корень из разности суммы квадратов деленной на К и квадраты суммы деленной на К деленную на К - 1
			root(error[j]);
			write(out, j, sr[j], error[j]);
		}
		out.close();
	}
	return 0;
}
string give_path(const int i) {
	string path = "test";
	path += to_string(i);
	path += ".txt";
	return path;
}
void average(value & sr)
{
	sr.minimum = sr.minimum / number_of_simulation;
	sr.max = sr.max / number_of_simulation;
	sr.average = sr.average / number_of_simulation;
	sr.width = sr.width / number_of_simulation;
	sr.utilization = sr.utilization / number_of_simulation;
}
void root(value & error)
{
	error.minimum = sqrt(error.minimum / (number_of_simulation- 1));
	error.max = sqrt(error.max / (number_of_simulation - 1));
	error.average = sqrt(error.average / (number_of_simulation - 1));
	error.width = sqrt(error.width / (number_of_simulation - 1));
	error.utilization = sqrt(error.utilization / (number_of_simulation - 1));

}
void add(ifstream & in, value & sr, value & error)
{
	size_t k;// Переменная, считывающая первую столбец из матрицы, то есть номер шага
	double _minimum = 0;
	double _max = 0;
	double _average = 0;
	double _width = 0;
	double _utilization = 0;
	in >> k >> _minimum >> _max >> _average >> _width >> _utilization;

	sr.minimum += _minimum;
	sr.max += _max;
	sr.average += _average;
	sr.width += _width;
	sr.utilization += _utilization;

	error.minimum += (_minimum)*(_minimum);
	error.max += (_max)*(_max);
	error.average += (_average)* (_average);
	error.width += (_width)*(_width);
	error.utilization += (_utilization)*(_utilization);
}
void count(value & sr, value & error){

	error.minimum -= sr.minimum*sr.minimum;
	error.max -= sr.max*sr.max;
	error.average -= sr.average*sr.average;
	error.width -= sr.width*sr.width;
	error.utilization -= sr.utilization*sr.utilization;
}
void write(ofstream &out, size_t j, value Val, value Val2) {
	// Выводим первые 8 значащих знака после запятой
	out.precision(8);
	out << j << " " << Val.minimum << " " << Val2.minimum << " "
		<< Val.max << " " << Val2.max << " "
		<< Val.average << " " << Val2.average << " "
		<< Val.width << " " << Val2.width << " "
		<< Val.utilization << " " << Val2.utilization << endl;;
}
