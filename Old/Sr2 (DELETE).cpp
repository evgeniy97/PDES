// Sr.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
//#include "iostream"
#include <fstream>
#include <string>
#include <cmath>
#include "stdio.h"
#include "stdlib.h"

using namespace std;

struct value {
	size_t N;
	double minimum = 0;
	double max = 0;
	double average = 0;
	double width = 0;
	double utilization = 0;
public:
	value& operator=(const value& mat);

	friend std::istream& operator>>(std::istream& os, value& oqject);
};

int main()
{
	ofstream main_out("Result min.txt", ios_base::trunc);
	//ofstream main_out_latex("Result_Latex.txt", ios_base::trunc);
	//main_out_latex << "\]begin{table}[h!]\n \]begin{ center } \n\\begin{ tabular }{ | c | c | }\n \hline \nq&среднее\\\ ";
	for (double q = 0.1; q < 1.05; q += 0.01) {
		value this_one, pr_one;
		ifstream from("result" + to_string(q) + ".txt", ios_base::in);
		if (from.is_open()) {
			ofstream out("newResult" + to_string(q) + ".txt", ios_base::trunc);
			from >> pr_one >> this_one;
			out << this_one.minimum - pr_one.minimum << endl;
			for (size_t i = 2; i < 10000; i++)
			{
				pr_one = this_one;
				from >> this_one;
				out << this_one.minimum - pr_one.minimum << endl;
			}
			from.close();
			out.close();
			ifstream from2("newResult" + to_string(q) + ".txt", ios_base::in);
			double sr = 0;
			double now = 0;
			for (size_t i = 0; i < 500; i++)
			{
				from2 >> now;
			}
			for (size_t i = 500; i < 10000 - 1; i++)
			{
				from2 >> now;
				sr = sr + now;
			}
			from2.close();
			sr = sr / (10000 - 1 - 500);
			main_out << q << " " << sr;
			ifstream from3("newResult" + to_string(q) + ".txt", ios_base::in);
			// считаем дисперсию
			double sr_erroe = 0;
			double now_ = 0;
			for (size_t i = 0; i < 500; i++)
			{
				from3 >> now;
			}
			for (size_t i = 500; i < 10000 - 1; i++)
			{
				from3 >> now_;
				now = (sr - now_) *(sr - now_);
				sr_erroe = sr_erroe + now;
			}
			from3.close();
			sr_erroe = sr_erroe / (10000 - 1 - 500);
			main_out << " " << sr_erroe << endl;
			//main_out_latex << "+-" << sr_erroe << " \\\ " << endl;
		}
	}
	return 0;
}

value & value::operator=(const value & mat)
{
	this->N = mat.N;
	this->minimum = mat.minimum;
	this->max = mat.max;
	this->average = mat.average;
	this->width = mat.width;
	this->utilization = mat.utilization;
	return *this;
}

std::istream & operator>>(std::istream & os, value & oqject)
{
	double sr_;
	os >> oqject.N >> oqject.minimum >> sr_ >> oqject.max >> sr_ >> oqject.average >> sr_ >> oqject.width >> sr_ >> oqject.utilization >> sr_;
	return os;
}
