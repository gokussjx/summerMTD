#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <random>
#include <time.h>

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
  double C_compute_j[10]; 
  double N_network_j[10];
  double U_utility_j[10];
  double greedy = 0;
  int greedy_index;
  double r_reputation_j[10];
  double leastCost_j = std::numeric_limits<double>::max();
  int leastCostVMIndex;
  double netCostArray[10];
  
  for (unsigned int j = 0; j < vmArray.size(); ++j) {
    
    // BEGIN: Deployment Cost calculation
    //Compute R value for the application k. We only need one application
    C_compute_j[j] = randomize(0, 1); // 1 - 0.65 {i.e, 65% CPU is being used}
    N_network_j[j] = randomize(0, 1); // 1 - 0.50 {i.e, 50% bandwidth being used}
    double Ravail_j = rootMeanSquare(C_compute_j[j], N_network_j[j]);
    double r_reputation_j[10];
    double Rmax_j = 1;
    double DC_deploymentCost_j = Ravail_j / Rmax_j;
    // END: Deployment Cost calculation
    
    //calculating the best R value for further use of greedy approach.
    if (greedy < Ravail_j) {
      greedy = Ravail_j;
      greedy_index = j;
      
    }
    
    // BEGIN: Migration Cost calculation
    // double NC_networkCost_ij = timeMigrateAtMaxBW / timeMigrateAtActualBW;
    double NC_networkCost_ij = randomize(0, 1);
    double minTempMCPossible = 0.0;
    double maxTempMCPossible = 2.0;
    double tempMC = SC_snapshotCost_i + NC_networkCost_ij;
    double MC_migrationCost_ij = (tempMC - minTempMCPossible) / (maxTempMCPossible - minTempMCPossible);
    // END: Migration Cost calculation

    // Least Cost Scheme
    double netCost_j = DC_deploymentCost_j + MC_migrationCost_ij;
    netCostArray[j] = netCost_j;
    if (netCost_j < leastCost_j) {
      leastCost_j = netCost_j;
      leastCostVMIndex = j;
    } 

    // BEGIN: Utility calculation
    r_reputation_j[j] = randomize(0, 1);
    double Ralloc_k_j = assignRalloc_k(Ravail_j, Rmax_k);
    U_utility_j[j] = r_reputation_j[j] * (Ralloc_k_j / Rmax_k);
    // END: Utility calculation

    // BEGIN: Bid calculation
    bidArray.push_back(U_utility_j[j] - MC_migrationCost_ij - DC_deploymentCost_j);
    // END: Bid calculation
  }
  
  //calculate maximum reputation to choose VM in accordance to the reputation.
  double max_reputation = 0;
  int reputation_index;  
  for (unsigned int j = 0; j < vmArray.size(); ++j) {
    if (max_reputation < r_reputation_j[j]) {
      max_reputation = r_reputation_j[j];
      reputation_index = j;
    	
    }
  }
  // BEGIN: Bid selection
  // Find the max Bid value and its corresponding array index
  auto bidIterator = std::max_element(std::begin(bidArray), std::end(bidArray));
  B_bid = *bidIterator;
  bestIndex = std::distance(std::begin(bidArray), bidIterator);
  // END: Bid selection
  
  //print all the VMs' metrics with their respective ID
  for (unsigned int i = 0; i < vmArray.size(); ++i) {
    std::cout << "The details for VM#" << i << std::endl;
    std::cout << "Computation: " << C_compute_j[i] << std::endl;
    std::cout << "Network: " << N_network_j[i] << std::endl;
    std::cout << "Reputation: " << r_reputation_j[i] << std::endl;
    std::cout << "Utility: " << U_utility_j[i] << std::endl;
    std::cout << "Cost: " << netCostArray[i] << std::endl; 
    std::cout << "----------------------------------" << std::endl;
  }  
  
  // Print the best VM's ID and Bid value
  std::cout << "LEAST JOINT (Our approach)" << "\n" << std::endl;
  std::cout << "The best VM is VM#" << vmArray[bestIndex] << " with Bid value = " << B_bid << std::endl;
  std::cout << std::endl;
  std::cout << "********" << std::endl;
  std::cout << "The computation value for this VM is: " << C_compute_j[bestIndex] << std::endl;
  std::cout << "The network value for this VM is: " << N_network_j[bestIndex] << std::endl;
  std::cout << "The reputation for this VM is: " << r_reputation_j[bestIndex] << "." << std::endl;
  std::cout << "The utility value for this VM is: " << U_utility_j[bestIndex] << std::endl;
  std::cout << "The cost for this VM is: " << netCostArray[bestIndex] << std::endl;
  std::cout << "Rmax_k(Cmax, Nmax) = " << Rmax_k;
  std::cout << "(" << C_max_k << ", " << N_max_k << ")" << "\n\n" << std::endl;

  std::cout << "**************************************************************************" << std::endl;
  std::cout << "LEAST LATENCY (Greedy approach)" << std::endl;
  std::cout << "The best VM is VM# " << greedy_index << " with the Resource value of " << greedy << "\n\n" << std::endl;
  std::cout << "The computation value for this VM is: " << C_compute_j[greedy_index] << "." << std::endl;
  std::cout << "The network value for this VM is: " << N_network_j[greedy_index] << "." << std::endl;
  std::cout << "The reputation for this VM is: " << r_reputation_j[greedy_index] << "." << std::endl;
  std::cout << "The utility value for this VM is: " << U_utility_j[greedy_index] << std::endl;
  std::cout << "The cost for this VM is: " << netCostArray[greedy_index] << std::endl;

  std::cout << "**************************************************************************" << std::endl;
  std::cout << "LEAST VULNERABILITY (Reputation approach)" << std::endl;
  std::cout << "The best VM is VM# " << reputation_index << " with the Reputation value of " << max_reputation << std::endl;
  std::cout << "The computation value for this VM is: " << C_compute_j[reputation_index] << "." << std::endl;
  std::cout << "The network value for this VM is: " << N_network_j[reputation_index] << "." << std::endl;
  std::cout << "The reputation for this VM is: " << r_reputation_j[reputation_index] << "." << std::endl;
  std::cout << "The utility value for this VM is: " << U_utility_j[reputation_index] << std::endl;
  std::cout << "The cost for this VM is: " << netCostArray[reputation_index] << std::endl;

  std::cout << "**************************************************************************" << std::endl;
  std::cout << "LEAST COST (MC+DC approach)" << std::endl;
  std::cout << "The best VM is VM# " << leastCostVMIndex << " with the Cost value of " << leastCost_j << std::endl;
  std::cout << "The computation value for this VM is: " << C_compute_j[leastCostVMIndex] << "." << std::endl;
  std::cout << "The network value for this VM is: " << N_network_j[leastCostVMIndex] << "." << std::endl;
  std::cout << "The reputation for this VM is: " << r_reputation_j[leastCostVMIndex] << "." << std::endl;
  std::cout << "The utility value for this VM is: " << U_utility_j[leastCostVMIndex] << std::endl;
  std::cout << "The cost for this VM is: " << netCostArray[leastCostVMIndex] << std::endl;

  std::cout << "**************************************************************************" << std::endl;
  std::cout << "RANDOM APPROACH" << std::endl;
  srand (time(NULL));
  int index = rand() % 10 + 1;
  std::cout << "The randomly selected VM is VM#" << index << std::endl;
  std::cout << "The computation value for this VM is: " << C_compute_j[index] << "." << std::endl;
  std::cout << "The network value for this VM is: " << N_network_j[index] << "." << std::endl;
  std::cout << "The reputation for this VM is: " << r_reputation_j[index] << "." << std::endl;
  std::cout << "The utility value for this VM is: " << U_utility_j[index] << std::endl;
  std::cout << "The cost for this VM is: " << netCostArray[index] << std::endl;
}
