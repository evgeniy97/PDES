// GetVelocity.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include <cmath>

using namespace std;
const size_t number_of_steps = 100000;

struct value {
	size_t N;
	double minimum = 0;
	double max = 0;
	double average = 0;
	double width = 0;
	double utilization = 0;
public:
	value& operator=(const value& mat);

	friend std::istream& operator >> (std::istream& os, value& oqject);
};

int main()
{
	string P;
	cin >> P;

	ofstream out_min(("Result velocity min p = " + P + ".txt"), ios_base::trunc);
	ofstream out_averange(("Result velocity averange p = " + P + ".txt"), ios_base::trunc);
	ofstream out_max(("Result velocity max p = " + P + ".txt"), ios_base::trunc);

	for (double q = 0.1; q < 1.0; q += 0.01) {
		ifstream from("result" + to_string(q) + ".txt", ios_base::in);
		if (from.is_open()) {
			value this_one, pr_one;
			ofstream out("newResult" + to_string(q) + ".txt", ios_base::trunc);
			from >> pr_one >> this_one;
			out << this_one.minimum - pr_one.minimum << " "
				<< this_one.average - pr_one.average << " "
				<< this_one.max - pr_one.max << endl;
			for (size_t i = 2; i < number_of_steps; i++)
			{
				pr_one = this_one;
				from >> this_one;
				out << this_one.minimum - pr_one.minimum << " " 
					<< this_one.average - pr_one.average << " " 
					<< this_one.max - pr_one.max << endl;
			}
			from.close();
			out.close();

			ifstream from2("newResult" + to_string(q) + ".txt", ios_base::in);

			double sr_min = 0,sr_averange = 0, sr_max = 0;
			double sr_min_squar = 0, sr_averange_squar = 0, sr_max_squar = 0;
			double now_min = 0,now_averange = 0, now_max = 0;

			for (size_t i = 0; i < 500; i++)
			{
				from2 >> now_min >> now_averange >> now_max;
			}

			for (size_t i = 500; i < number_of_steps - 1; i++)
			{
				from2 >> now_min >> now_averange >> now_max;
				sr_min = sr_min + now_min;
				sr_averange = sr_averange + now_averange;
				sr_max = sr_max + now_max;

				sr_min_squar += now_min * now_min;
				sr_averange_squar += now_averange * now_averange;
				sr_max_squar += now_max * now_max;
			}

			from2.close();

			sr_min = sr_min / (number_of_steps - 500 - 1);
			sr_averange = sr_averange / (number_of_steps - 500 - 1);
			sr_max = sr_max / (number_of_steps - 500 - 1);

			sr_min_squar = sr_min_squar / (number_of_steps - 500 - 1);
			sr_averange_squar = sr_averange_squar / (number_of_steps - 500 - 1);
			sr_max_squar = sr_max_squar / (number_of_steps - 500 - 1);

			out_min << q << " " << sr_min;
			out_averange << q << " " << sr_averange;
			out_max << q << " " << sr_max;

			ifstream from3("newResult" + to_string(q) + ".txt", ios_base::in);

			double sr_min_er = 0, sr_averange_er = 0, sr_max_er = 0;

			now_min = 0;
			now_averange = 0;
			now_max = 0;

			for (size_t i = 0; i < 500; i++)
			{
				from3 >> now_min >> now_averange >> now_max;
			}

			for (size_t i = 500; i < number_of_steps - 1; i++)
			{
				from3 >> now_min >> now_averange >> now_max;
				sr_min_er += sr_min_squar - now_min * now_min;
				sr_averange_er += sr_averange_squar - now_averange * now_averange;
				sr_max_er += sr_max_squar - now_max * now_max;
			}

			from3.close();

			sr_min_er = sr_min_er / (number_of_steps - 500 - 1 - 1);
			sr_averange_er = sr_averange_er/ (number_of_steps - 500 - 1 - 1);
			sr_max_er = sr_max_er / (number_of_steps - 500 - 1 - 1);

			sr_min_er = sqrt(abs(sr_min_er));
			sr_averange_er = sqrt(abs(sr_averange_er));
			sr_max_er = sqrt(abs(sr_max_er));

			out_min  << " " << sr_min_er << endl;
			out_averange  << " " << sr_averange_er << endl;
			out_max << " " << sr_max_er << endl;

		}
	}

	for (double q = 0.1; q < 1.00; q += 0.01) remove(("newResult" + to_string(q) + ".txt").c_str());


	out_min.close();
	out_averange.close();
	out_max.close();
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

std::istream & operator >> (std::istream & os, value & oqject)
{
	double sr_;
	os >> oqject.N >> oqject.minimum >> sr_ >> oqject.max >> sr_ >> oqject.average >> sr_ >> oqject.width >> sr_ >> oqject.utilization >> sr_;
	return os;
}

