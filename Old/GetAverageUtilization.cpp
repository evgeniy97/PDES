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
	std::ios_base::sync_with_stdio(false);
	string P;
	cin >> P;

	ofstream out(("Result utilization p = " + P + ".txt"), ios_base::trunc);


	for (double q = 0.1; q < 1.0; q += 0.01) {
		ifstream from("Result utilization p = " + P + "/Result utilization p =" + P + " q = " + to_string(q) + ".txt", ios_base::in);
		if (from.is_open()) {

			double utilization = 0;
			double utilization_squar = 0;
			double now_utilization = 0;
			double utilization_err = 0;

			for (int i = 0; i < number_of_steps; i++) {
				int N = 0;
				from >> N >> now_utilization >> utilization_err;
				utilization = utilization + now_utilization;
			}
			
			utilization = utilization / number_of_steps;
			from.close();

			out << q << " " << utilization;

			ifstream from_("Result utilization p = " + P + "/Result utilization p =" + P + " q = " + to_string(q) + ".txt", ios_base::in);

			for (int i = 0; i < number_of_steps; i++) {
				int N = 0;
				from >> N >> now_utilization >> utilization_err;
				utilization_squar = utilization_squar + (utilization - now_utilization)*(utilization - now_utilization);
			}

			from_.close();

			utilization_squar = utilization_squar/ ( number_of_steps - 1);
			utilization_squar = sqrt(utilization_squar);

			out << " " << utilization_squar << endl;
		}
	}


	out.close();
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
