% Variables:
% i is SOURCE
% j is DESTINATION
% k is application

% Deployment Cost
DC(j, k) = R(j) / Rmax

% Migration Cost (migrating i to j)
SC(i, k) = (timeAtIdeal / timeAtSource) % Snapshot Cost at i
NC(i, j, k) = (timeMigrateAtMaxBW / timeMigrateAtActualBW) % Network Cost for migrating from i to j
tempMC = SC(i, k) + NC(i, j, k)
% minTempMCPossible = 0
% maxTempMCPossible = 2
MC(i, j, k) = (tempMC - minTempMCPossible) / (maxTempMCPossible - minTempMCPossible)

% Utility (of j)
r(j) = rand(0, 1)  % reputation of j
Ralloc(k) = ? % Resource that will be allocated to k, if this VM is chosen
Rmax(k) = ? % Max resource that k is fine with
U = r(j) * (Ralloc(k) / Rmax(k))

% Bid
B = max(U(j, k) - MC(i, j, k) - DC(j, k))