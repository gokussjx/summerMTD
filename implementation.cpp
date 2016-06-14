#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <random>

bool first = true;

// Returns RMS value
double rootMeanSquare(double a, double b) {
  double rms = pow(a, 2.0) + pow(b, 2.0);
  rms = rms / 2.0;
  return sqrt(rms);
}

// Uniform real distribution random number generator
double randomize(double a, double b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(a, b + 0.00001);

    auto temp = dis(gen);
    if (first == true) {
      first = false;
      if (temp == a) temp = randomize(a, b);  // We're reducing probability of 'a'
      first = true;
    }
    if (temp > b) temp = b;

    return dis(gen);
}

double assignRalloc_k(double Ravail_j, double Rmax_k) {
	double Rmin_k = randomize(0, 0.5);
	//double Rmax_k = randomize(0.5, 1);
	double p_k = randomize(0, 1.0);
  double Ralloc_k_j = 0;
	if (Ravail_j < Rmin_k)
		Ralloc_k_j = 0;
	else if (p_k * Rmax_k < Rmin_k)
		Ralloc_k_j = Rmin_k;
	else
		Ralloc_k_j = p_k * Rmax_k;
	return Ralloc_k_j;
}

int main() {

  std::vector<int> vmArray{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};    // Holds the ID of every VM
  int bestIndex;                                              // Index of best VM in vmArray

  // SC_snapshotCost_i = timeAtIdeal / timeAtSource;
  double SC_snapshotCost_i = randomize(0, 1);
  
  double Rmax, Ralloc, U_utility;
  std::vector<double> bidArray;
  double B_bid = std::numeric_limits<double>::min();

  //Compute Ralloc value for the application k. We only need one application
  // double C_compute_k = randomize(0, 0.5); // 1 - 0.65 {i.e, 65% CPU is being used}
  // double N_network_k = randomize(0, 0.5); // 1 - 0.50 {i.e, 50% bandwidth being used}
  // double Ralloc_k = rootMeanSquare(C_compute_k, N_network_k);

  //Compute Rmax value for the application k. We only need one application
  double C_max_k = randomize(0.5, 1);
  double N_max_k = randomize(0.5, 1);
  double Rmax_k = rootMeanSquare(C_max_k, N_max_k);
  
  for (int j = 0; j < vmArray.size(); ++j) {
    
    // BEGIN: Deployment Cost calculation
    //Compute R value for the application k. We only need one application
    double C_compute_j = randomize(0, 1); // 1 - 0.65 {i.e, 65% CPU is being used}
    double N_network_j = randomize(0, 1); // 1 - 0.50 {i.e, 50% bandwidth being used}
    double Ravail_j = rootMeanSquare(C_compute_j, N_network_j);
    double Rmax_j = 1;
    double DC_deploymentCost_j = Ravail_j / Rmax_j;
    // END: Deployment Cost calculation
    
    // BEGIN: Migration Cost calculation
    // double NC_networkCost_ij = timeMigrateAtMaxBW / timeMigrateAtActualBW;
    double NC_networkCost_ij = randomize(0, 1);
    double minTempMCPossible = 0.0;
    double maxTempMCPossible = 2.0;
    double tempMC = SC_snapshotCost_i + NC_networkCost_ij;
    double MC_migrationCost_ij = (tempMC - minTempMCPossible) / (maxTempMCPossible - minTempMCPossible);
    // END: Migration Cost calculation

    // BEGIN: Utility calculation
    double r_reputation_j = randomize(0, 1);
  	double Ralloc_k_j = assignRalloc_k(Ravail_j, Rmax_k);
  	double U_utility_j = r_reputation_j * (Ralloc_k_j / Rmax_k);
    // END: Utility calculation

    // BEGIN: Bid calculation
    bidArray.push_back(U_utility_j - MC_migrationCost_ij - DC_deploymentCost_j);
    // END: Bid calculation
  }

  // BEGIN: Bid selection
  // Find the max Bid value and its corresponding array index
  auto bidIterator = std::max_element(std::begin(bidArray), std::end(bidArray));
  B_bid = *bidIterator;
  bestIndex = std::distance(std::begin(bidArray), bidIterator);
  // END: Bid selection

  // Print the best VM's ID and Bid value
  std::cout << "The best VM is VM#" << vmArray[bestIndex] << " with Bid value = " << B_bid << std::endl;
  std::cout << std::endl;
  std::cout << "********" << std::endl;
  std::cout << "Extra stats: " << std::endl;
  // std::cout << "Ralloc_k_j = " << Ralloc_k_j << std::endl;
  // std::cout << "(" << C_compute_k << ", " << N_network_k << ")" << std::endl;
  std::cout << "Rmax_k(Cmax, Nmax) = " << Rmax_k;
  std::cout << "(" << C_max_k << ", " << N_max_k << ")" << std::endl;
}
