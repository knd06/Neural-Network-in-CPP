// main.cpp

// don't forget to include out neural network
#include <fstream> 
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>


#include "NeuralNetwork.hpp"


void ReadCSV(std::string filename, std::vector<RowVector*>& data)
{
	data.clear();
	std::ifstream file(filename);
	std::string line, word;
	// determine number of columns in file
	getline(file, line, '\n');
	std::stringstream ss(line);
	std::vector<Scalar> parsed_vec;
	while (getline(ss, word, ',')) {
		parsed_vec.push_back(Scalar(std::stof(&word[0])));
	}
	uint cols = parsed_vec.size();
	data.push_back(new RowVector(cols));
	for (uint i = 0; i < cols; i++) {
		data.back()->coeffRef(i) = parsed_vec[i];
	}

	// read the file
	if (file.is_open()) {
		while (getline(file, line, '\n')) {
			std::stringstream ss(line);
			data.push_back(new RowVector(1, cols));
			uint i = 0;
			while (getline(ss, word, ',')) {
				data.back()->coeffRef(i) = Scalar(std::stof(&word[0]));
				i++;
			}
		}
	}
}


//... data generator code here
void genData(std::string filename, uint feat_len)
{
	std::ofstream file1(filename + "-in");
	std::ofstream file2(filename + "-out");
	for (uint r = 0; r < 100000; r++) {
		Scalar y = 10;
		for (int i = 0; i < feat_len; i++) {
			Scalar feat_i = rand() / Scalar(RAND_MAX);
			if (i != feat_len - 1) {
				file1 << feat_i << ",";
			} else {
				file1 << feat_i << std::endl;
			}
			y += 8 * feat_i;
		}
		// suppose the distribution is y = x1 + x1 + x2 + 10
		file2 << y << std::endl;
	}
	file1.close();
	file2.close();
}


typedef std::vector<RowVector*> data;
int main()
{
	uint batch_size = 5;
	std::vector<uint> topology;
	topology.push_back(12);
	topology.push_back(128);
	topology.push_back(64);
	topology.push_back(1);
    NeuralNetwork n(topology);
	data in_dat, out_dat;
	genData("an", 12);
	ReadCSV("an-in", in_dat);
	ReadCSV("an-out", out_dat);
	n.train(in_dat, out_dat, batch_size);
	return 0;
}
