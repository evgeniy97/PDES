// GetWidth.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include<direct.h>

using namespace std;

struct value {
	size_t N = 0;
	double width = 0;
	double width_error = 0;
public:
	value& operator=(const value& mat);

	friend std::istream& operator >> (std::istream& os, value& oqject);
};

int main(){
	string P;
	cin >> P;
	mkdir(("Result width p = " + P).c_str());
	for (double q = 0.1; q < 1.05; q += 0.01) {
		value use_me;
		ifstream from("result" + to_string(q) + ".txt", ios_base::in);
		if (from.is_open()) {
			ofstream main_out("Result width p = " + P + "/Result width p ="  + P + " q = " + to_string(q) + ".txt", ios_base::trunc);
			for (size_t i = 0; i < 100000; i++) {
				from >> use_me;
				main_out << use_me.N << " " << use_me.width << " " << use_me.width_error << endl;
			}
			from.close();
			main_out.close();
		}
	}
    return 0;
}

std::istream & operator >> (std::istream & os, value & oqject)
{
	double sr_;
	os >> oqject.N >> sr_ >> sr_ >> sr_ >> sr_ >> sr_ >> sr_ >> oqject.width >> oqject.width_error >> sr_ >> sr_;
	return os;
}
