#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <random>

double rootMeanSquare(double a, double b, double c) {
  double rms = pow(a, 2.0) + pow(b, 2.0) + pow(c, 2.0);
  rms = rms / 3.0;
  return rms;
}

int main() {


  std::vector<int> vmArray{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};    // Holds the ID of every VM
  int bestIndex;                                              // Index of best VM in vmArray

  // double DC_deploymentCost;
  
  double timeAtSource = ?;
  double timeAtIdeal = ?; 
  double SC_snapshotCost_i = timeAtIdeal / timeAtSource;
  
  double Rmax, Ralloc, U_utility;
  std::vector<double> bidArray;
  double B_bid = std::numeric_limits<double>::min();

  
  for (int j = 0; j < vmArray.size(); ++j) {
    
    // TODO: Implement DC_deploymentCost
    // RMS value for "vector" R_k
    double C_compute_j = ?;
    double S_storage_j = ?;
    double N_network_j = ?;
    double R_j = rootMeanSquare(C_compute_j, S_storage_j, N_network_j);
    double DC_deploymentCost_j = R_j / Rmax;
    
    // BEGIN: Migration Cost calculation
    double NC_networkCost_ij = timeMigrateAtMaxBW / timeMigrateAtActualBW;
    double minTempMCPossible = 0.0;
    double maxTempMCPossible = 2.0;
    double tempMC = SC_snapshotCost_i + NC_networkCost_ij;
    double MC_migrationCost_ij = (tempMC - minTempMCPossible) / (maxTempMCPossible - minTempMCPossible);

    // BEGIN: Utility calculation
    // **Uniform real distribution random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1.00001);
    double r_reputation_j = dis(gen);
    
    if (r_reputation_j == 0) r_reputation_j = dis(gen);
    if (r_reputation_j > 1) r_reputation_j = 1;
    // **
    
    double Ralloc = ?;
    double Rmax = ?;
    double U_utility_j = r_reputation_j * (Ralloc / Rmax);
    // END: Utility calculation

    // BEGIN: Bid calculation
    bidArray[j] = U_utility_j - MC_migrationCost_ij - DC_deploymentCost_j;
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
}