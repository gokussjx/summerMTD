% Variables:
% i is SOURCE
% j is DESTINATION
% k is application

% Deployment Cost
Rk = {C, S(j), N}
DC(j) = R(j) / Rmax

% Migration Cost (migrating i to j)
SC(i) = (timeAtIdeal / timeAtSource) % Snapshot Cost at i
NC(i, j) = (timeMigrateAtMaxBW / timeMigrateAtActualBW) % Network Cost for migrating from i to j
tempMC = SC(i) + NC(i, j)
% minTempMCPossible = 0
% maxTempMCPossible = 2
MC(i, j) = (tempMC - minTempMCPossible) / (maxTempMCPossible - minTempMCPossible)

% Utility (of j)
r(j) = rand(0, 1)  % reputation of j
Ralloc(k) = ? % Resource that will be allocated to k, if this VM is chosen
Rmax(k) = ? % Max resource that k is fine with
U(j) = r(j) * (Ralloc(k) / Rmax(k))

% Bid
B = max(U(j) - MC(i, j) - DC(j))