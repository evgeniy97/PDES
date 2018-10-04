/************************************************************

The simple model of time evolution in optimistic PDES (parallel
discrete event simulation) algorithm on small-world topology.

Date: 15.09.2017

Authors:         Ziganurova Liliia, Khomutov Evgeniy

Purpose:        Parallel Discrete Event Simulation Research

Compile:        mpic++ -o optimisticSW.out SWv3+mpi+mt19937.cpp -std=c++11 -I../include -L../lib -lrngavx (g++ -o test ./OSW.cpp -std=c++11 -I./include -L./lib -lrngavx)


Usage:          ./optimisticSW.out -N -M -r -q -p

Reference:      I use RNGAVXLIB library to generate random numbers
(http://cpc.cs.qub.ac.uk/summaries/AEIT_v3_0.html)

File formats:   The ouptut files are:  txt
************************************************************/

#include <mpi.h>
//#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include <fstream>
#include "mt19937.h"
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;


//Function CreateOneDimSmallWorldTopolgy returns a graph of N nodes
//with pN random links added on the top of regular 1D topology(ring)

vector< vector<int> > CreateOneDimSmallWorldTopolgy(int N, float p, mt19937_state &state);

//Functiont PrintGraph prints graph : )
void PrintGraph(vector< vector<int> > graph);
/* Function ForwardPropogation updates array of LVT. We assume that all
LPs may update their times.
*/
void ForwardPropogation(double array_of_LVT[], size_t number_of_LP, double r, mt19937_state &state);

//Function Rollback choses a random node index and then compares its time with time of its random neighbour.
//If the time of the neigbour is lower, the time of node index becomes equal to the time of the neigbour.
//This procedure is made K*N times.
void  Rollback(double array_of_LVT[], vector< vector<int> > graph, size_t number_of_LP, double b, mt19937_state &state);
/*==============================================================

Function CreateOneDimSmallWorldTopolgy returns a graph of N nodes
with pN random links added on the top of regular 1D topology (ring)

Functiont PrintGraph prints graph :)

*/

vector< vector<int> > CreateOneDimSmallWorldTopolgy(int N, float p, mt19937_state &state) {

	vector< vector<int> > graph;
	int right_neighbour_ind, left_neighbour_ind;
	int from_node, to_node;

	//First Create a regular topology 
	for (int index = 0; index < N; index++) {

		//Define boundaries 
		if (index == 0) {
			right_neighbour_ind = 1;
			left_neighbour_ind = N - 1;
		}
		else if (index == N - 1) {
			left_neighbour_ind = index - 1;
			right_neighbour_ind = 0;
		}
		else {
			left_neighbour_ind = index - 1;
			right_neighbour_ind = index + 1;
		}

		// Add closest neighbours
		graph.push_back(vector <int>());
		graph[index].push_back(right_neighbour_ind);
		graph[index].push_back(left_neighbour_ind);
	}


	// Add random links
	int k = 0;
	int counter = 0;
	while (k < N*p)
	{
		from_node = mt19937_generate_(&state) % N;
		to_node = mt19937_generate_(&state) % N;

		//Проверка, чтобы не было кратных дуг и петель
		if ((from_node != to_node) &&
			(find(graph[from_node].begin(), graph[from_node].end(), to_node) == graph[from_node].end())) {
			graph[from_node].push_back(to_node);
			graph[to_node].push_back(from_node); //так как граф не ориентированный
			k += 1;
		}
	}
	return(graph);

} //GraphCreate



void PrintGraph(vector< vector<int> > graph) {
	int N = graph.size();

	for (int i = 0; i< N; i++) {
		cout << "Элемент " << i << " соседствует с ";
		for (int k = 0; k < graph[i].size(); k++)
		{
			cout << graph[i][k] << " ";
		}
		cout << "\n";
	}
}

/*==============================================================*/

/* Function ForwardPropogation updates array of LVT. We assume that all
LPs may update their times.
*/

void ForwardPropogation(double array_of_LVT[], size_t number_of_LP, double r, mt19937_state &state) {

	double sluch, J;

	for (int i = 0; i<number_of_LP; i++) {
		// Генерация случайного числа от 0 до 1 (равномерное распределение)
		// Вычисление числа J (пуассоновское распределение с параметром r)

		sluch = mt19937_generate_uniform_float_(&state);
		if (sluch == 0) {
			do { sluch = mt19937_generate_uniform_float_(&state); }
			while (sluch == 0);
			J = -r * log(sluch);
		}
		else {
			J = -r * log(sluch);
		}

		// Прибавление к случайному ЛП числа J
		int index = mt19937_generate_(&state) % number_of_LP;

		//int index = rand() % number_of_LP;
		array_of_LVT[index] = array_of_LVT[index] + J;
	} // Завершение цикла по N_PE


}



/* Function Rollback choses a random node index and then compares its time with time of its random neighbour.
If the time of the neigbour is lower, the time of node index becomes equal to the time of the neigbour.
This procedure is made K*N times.
*/
void  Rollback(double array_of_LVT[], vector< vector<int> > graph, size_t number_of_LP, double b, mt19937_state &state) {

	//Генерация числа К
	double sluch = mt19937_generate_uniform_float_(&state);
	int K;
	int nNeighbours, indNeighbour;
	int index;

	if (sluch == 0) {
		do { sluch = mt19937_generate_uniform_float_(&state); }
		while (sluch == 0);
		K = round(-b * log(sluch));
	}
	else { K = round(-b * log(sluch)); }

	// K*N раз
	for (int i = 0; i < K * number_of_LP; i++) {

		//Выбор случайного ЛП
		index = mt19937_generate_(&state) % number_of_LP;

		nNeighbours = graph[index].size(); // определили количество соседей

		indNeighbour = mt19937_generate_(&state) % nNeighbours;//выбрали случайного

		if (array_of_LVT[index] > array_of_LVT[graph[index][indNeighbour]])
		{
			array_of_LVT[index] = array_of_LVT[graph[index][indNeighbour]];
		}


	} // Завершение цикла по K*N

}
/*==============================================================*/

/* Here is a block of simple function of calculating average, minimum,
width and local minima density of a given array */


struct Observables {
	double minimum;
	double max;
	double average;
	double width;
	double utilization;
public:
	//Observables():minimum(0), max(0), average(0), width(0), utilization(0) {};
	friend ostream& operator<<(ostream& os, const Observables& _this);
};

ostream& operator<<(ostream& os, const Observables& this_observables)
{
	os << this_observables.minimum << " " << this_observables.max << " " << this_observables.average << " " << this_observables.width << " " << this_observables.utilization << endl;
	return os;
}

Observables CalucateObservables(double array_of_LVT[], size_t number_of_LP, vector< vector<int> > graph) {

	double min = array_of_LVT[0];
	double max = array_of_LVT[0]; //!
	double sum = 0.0;
	double sum_square = 0.0;
	int number_of_active_LP = 0;
	int nNeighbours;
	double avg;
	bool Flag = 0;

	for (int i = 0; i < number_of_LP; i++) {


		sum += array_of_LVT[i];

		if (array_of_LVT[i] < min) min = array_of_LVT[i];
		if (array_of_LVT[i] > max) max = array_of_LVT[i];

		nNeighbours = graph[i].size();
		for (int q = 0; q < nNeighbours; q++) {
			if (array_of_LVT[i] > array_of_LVT[graph[i][q]]) {
				Flag = 0;
				break;
			}
			else Flag = 1;

		}
		if (Flag == 1) number_of_active_LP += 1;

	}
	avg = double(sum) / number_of_LP;

	for (int i = 0; i < number_of_LP; i++) {
		sum_square += pow((array_of_LVT[i] - avg), 2);
	}

	return{ min,max, avg , sum_square / number_of_LP, double(number_of_active_LP) / number_of_LP };
}

string give_path(const size_t number_of_LP, const size_t number_of_timesteps, const  float r, const float q, const float p) {
	return "N" + to_string(number_of_LP) + "M" + to_string(number_of_timesteps) + "r" + to_string(r) + "q" + to_string(q) + "p" + to_string(p);
}

int main(int argc, char* argv[]) {

	std::ios_base::sync_with_stdio(false);
	mkdir("result", 0777);

	int Ranks;
	int myRank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &Ranks);

	// Initializing  Generator
	mt19937_state state; // Initializating state. 
	int number_of_LP = atoi(argv[1]); // Number of logical processes
	int number_of_timesteps = atoi(argv[2]); // Number of time steps
	float r = atof(argv[3]); //Percentage of random connections in range [0:1]
	float q = atof(argv[4]);
	float b = 1/q-1;
	float p = atof(argv[5]);
	mt19937_init_sequence_(&state, (unsigned long long)(time(NULL) + myRank));

	for (size_t j = 100 * (myRank); j < 100 * (myRank + 1); j++)
	{

		Observables my_observables;
		double *tau;
		vector< vector<int> >  my_topology;
		my_topology = CreateOneDimSmallWorldTopolgy(number_of_LP, p, state);
		tau = (double*)malloc(number_of_LP*sizeof(double)); //contains local virual times
															// Initialize array of local virual times
		for (int i = 0; i < number_of_LP; i++)  tau[i] = 0.0;

		//Open file for writing 
		string path = "result/";
		path += give_path(number_of_LP, number_of_timesteps, r, q, p);
		mkdir(path.c_str(), 0777);
		path = path + "/test" + to_string(j) + ".txt";
		ofstream out(path, ios_base::trunc);
		out.precision(8);

		// Begin to model the evolution of LVT profile 
		for (int t = 0; t < number_of_timesteps; t++)
		{
			ForwardPropogation(tau, number_of_LP, r, state);
			Rollback(tau, my_topology, number_of_LP, b, state);
			my_observables = CalucateObservables(tau, number_of_LP, my_topology);
			out << t << " " << my_observables;
		}
		out.close();
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	return 0;
}
