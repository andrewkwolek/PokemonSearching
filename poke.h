// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <iomanip>
#include <getopt.h>

using namespace std;

class poke {
private:
	int pokemon;
	bool mode = false;
	bool help = false;
	char mode_type;
	double curr_best;
	double curr_dist = 0;
	int curr_path_size = 0;
	struct node {
		int x;
		int y;
		bool used = false;
		double min_weight = numeric_limits<double>::infinity();
	};
	vector<node> points;
	vector<int> best_path;
	vector<int> curr_path;


public:
	poke()
		:pokemon(), mode(false), help(false), mode_type(), curr_best(), curr_dist(0),
		curr_path_size(0), points(), best_path() { }

	~poke() { }

	void setArguments(int argc, char* argv[]) {
		opterr = true;
		int choice;
		int option_index = 0;
		option long_options[] =
		{
			{"mode", required_argument, nullptr, 'm'},
			{"help", no_argument, nullptr, 'h'},
			{nullptr, 0, nullptr, '\0'}
		};

		while ((choice = getopt_long(argc, argv, "m:h",
			long_options, &option_index)) != -1) {
			switch (choice) {
			case 'm':
				mode = true;
				mode_type = optarg[0];
				break;
			case 'h':
				help = true;
				cout << "This program takes in coordinates of pokemon and depending on the mode\n";
				cout << "selected, MST, FASTTSP, and OPTTSP, will create a graph between the points\n";
				cout << "finding the shortest possible distance\n";
				exit(0);
				break;
			}
		}
	}

	void readInput() {
		bool coast = false;
		bool land = false;
		bool sea = false;
		points.reserve(pokemon);

		cin >> pokemon;

		for (int i = 0; i < pokemon; ++i) {
			node temp;
			cin >> temp.x;
			cin >> temp.y;
			if (mode_type == 'M') {
				if ((temp.x <= 0 && temp.y == 0) || (temp.y <= 0 && temp.x == 0)) {
					coast = true;
				}
				else if (temp.x < 0 && temp.y < 0) {
					sea = true;
				}
				else {
					land = true;
				}
			}
			points.push_back(temp);
		}

		if (mode_type == 'M' && coast == false && sea == true && land == true) {
			cerr << "Cannot construct MST" << endl;
			exit(1);
		}
	}

	void MST() {
		vector<int> predecessor;
		predecessor.resize(pokemon);
		points[0].min_weight = 0;
		predecessor[0] = -1;
		double weight = 0;

		cout << std::setprecision(2); //Always show 2 decimal places
		cout << std::fixed; //Disable scientific notation for large numbers
		
		for (int i = 0; i < pokemon; ++i) {
			double min = numeric_limits<double>::infinity();
			int min_ind = -1;
			for (int j = 0; j < pokemon; ++j) {
				if (!points[j].used && points[j].min_weight < min) {
					min = points[j].min_weight;
					min_ind = j;
				}
			}

			points[min_ind].used = true;


			for (int j = 0; j < pokemon; ++j) {
				if (points[j].used == false) {
					if (distance(points[min_ind], points[j]) < points[j].min_weight) {
						points[j].min_weight = distance(points[min_ind], points[j]);
						predecessor[j] = min_ind;
					}
				}
			}
		}

		for (int i = 0; i < pokemon; ++i) {
			if (points[i].min_weight != numeric_limits<double>::infinity()) {
				weight += points[i].min_weight;
			}
		}

		cout << weight << "\n";

		for (int i = 0; i < pokemon; ++i) {
			if (predecessor[i] != -1) {
				cout << min(i, predecessor[i]) << " " << max(i, predecessor[i]) << "\n";
			}
		}
	}

	double FASTTSP() {
		vector<int> path;
		double dist = 0;
		path.reserve(pokemon);
		points[0].min_weight = distance(points[0], points[0]);
		points[0].used = true;
		path.push_back(0);
		points[0].min_weight = distance(points[0], points[1]);
		points[1].min_weight = distance(points[0], points[1]);
		points[1].used = true;
		path.push_back(1);
		path.push_back(0);

		cout << std::setprecision(2); //Always show 2 decimal places
		cout << std::fixed; //Disable scientific notation for large numbers

		for (int i = 2; i < pokemon; ++i) {
			std::vector<int>::iterator insertion_point = path.end();
			int inserter = 0;
			double min = numeric_limits<double>::infinity();
			int k = 0;
			for (std::vector<int>::iterator j = path.begin(); j < path.end() - 1; ++j, ++k) {
				double diff = distance(points[i], points[*j]) + distance(points[i], points[*(j + 1)]) - points[*j].min_weight;
				if (diff < min) {
					min = diff;
					insertion_point = j + 1;
					inserter = k + 1;
				}
			}
			if (insertion_point != path.end()) {
				points[i].min_weight = distance(points[i], points[path[inserter]]);
				points[path[inserter - 1]].min_weight = distance(points[i], points[path[inserter - 1]]);
				path.insert(insertion_point, i);
			}
		}

		for (size_t i = 0; i < path.size() - 1; ++i) {
			dist += points[path[i]].min_weight;
		}

		if (mode_type == 'F') {
			cout << dist << "\n";

			for (size_t i = 0; i < path.size() - 1; ++i) {
				cout << path[i] << " ";
			}
		}

		best_path = path;

		return dist;

	}

	void OPTTSP() {
		curr_best = FASTTSP();
		best_path.pop_back();
		curr_path = best_path;

		genPerms(curr_path, 1);
		

		cout << curr_best << "\n";
		for (size_t i = 0; i < best_path.size(); ++i) {
			cout << best_path[i] << " ";
		}
	}

	template <typename T>
	void genPerms(std::vector<T>& path, size_t permLength) {
		if (path.size() == permLength) {
			double dist = curr_dist + distance(points[path[0]], points[path[path.size() - 1]]);
			if (dist < curr_best) {
				curr_best = dist;
				best_path = path;
			}
			return;
		}
		if (!promising(path, permLength))
			return;
		for (size_t i = permLength; i < path.size(); ++i) {
			std::swap(path[permLength], path[i]);
			curr_dist += distance(points[path[permLength]], points[path[permLength - 1]]);
			genPerms(path, permLength + 1);
			curr_dist -= distance(points[path[permLength]], points[path[permLength - 1]]);
			std::swap(path[permLength], path[i]);
		}
	}

	template <typename T>
	double lowerbound(std::vector<T>& path, size_t permLength) {
		for (int i = (int)permLength; i < pokemon; ++i) {
			points[i].used = false;
			points[i].min_weight = numeric_limits<double>::infinity();
		}

		points[permLength].min_weight = 0;
		double weight = 0;
		double min_start = numeric_limits<double>::infinity();
		double min_end = numeric_limits<double>::infinity();

		cout << std::setprecision(2); //Always show 2 decimal places
		cout << std::fixed; //Disable scientific notation for large numbers

		

		for (int i = (int)permLength; i < pokemon; ++i) {
			double min = numeric_limits<double>::infinity();
			int min_ind = -1;
			for (int j = (int)permLength; j < pokemon; ++j) {
				if (!points[j].used && points[j].min_weight < min) {
					min = points[j].min_weight;
					min_ind = j;
				}
			}

			points[min_ind].used = true;
			weight += points[min_ind].min_weight;

			for (int j = (int)permLength; j < pokemon; ++j) {
				if (points[j].used == false) {
					if (distance(points[path[min_ind]], points[path[j]]) < points[j].min_weight) {
						points[j].min_weight = distance(points[path[min_ind]], points[path[j]]);
					}
				}
			}

			if (distance(points[path[i]], points[path[0]]) < min_start) {
				min_start = distance(points[path[i]], points[path[0]]);
			}
			if (distance(points[path[i]], points[path[permLength - 1]]) < min_end) {
				min_end = distance(points[path[i]], points[path[permLength - 1]]);
			}
		}

		weight += min_start;
		weight += min_end;

		return weight;
	}

	template <typename T>
	bool promising(std::vector<T>& path, size_t permLength) {
		if (lowerbound(path, permLength) + curr_dist < curr_best) {
			return true;
		}
		else {
			return false;
		}
	}

	//Use piazza page for reference, helpful @1028
	double distance(node& one, node& two) {
		if (mode_type == 'M') {
			if ((one.x < 0 && one.y < 0) && (two.x > 0 || two.y > 0)) {
				return numeric_limits<double>::infinity();
			}
			if ((two.x < 0 && two.y < 0) && (one.x > 0 || one.y > 0)) {
				return numeric_limits<double>::infinity();
			}
		}

		double diff1 = two.x - one.x;
		double diff2 = two.y - one.y;
		double result = sqrt(diff1 * diff1 + diff2 * diff2);
		return result;
	}

	char getMode() {
		return mode_type;
	}

};