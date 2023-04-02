/* Jakub Pataluch, 2023 */
set Sites;
set Types;

param Deficit{Sites, Types};
param Excess{Sites, Types};
param Distance{Sites, Sites};

param TransportationCost{Types};

var MoveTo{Types, Sites, Sites} >= 0;

s.t. transportNotMoreThanExcess{t in Types, source in Sites}: sum{dest in Sites} MoveTo[t, source, dest] <= Excess[source, t];
s.t. deficitForTypes2Meet{dest in Sites}: sum{source in Sites} MoveTo["Typ 2", source, dest] >= Deficit[dest, "Typ 2"];
s.t. deficitMeet{dest in Sites}: sum{t in Types, source in Sites} (MoveTo[t, source, dest]) == sum{t in Types} Deficit[dest, t];

minimize transportationCost: sum{t in Types} ( sum{source in Sites, dest in Sites} MoveTo[t,source,dest] * Distance[source,dest]*TransportationCost[t] ); 

solve;

/* Display solution in table */

for{s in Sites}{
        printf: " & \t%s", s;
}
printf: "\\\\ \\hline \n";

for{dst in Sites}{
        printf: "%s\t", dst;
        for{src in Sites}{
                printf: "& (%d,%d)\t", MoveTo["Typ 1", src,dst], MoveTo["Typ 2", src,dst];
        }
        printf: "\\\\ \\hline\n";
}

printf: "Koszt = %f", sum{t in Types} ( sum{source in Sites, dest in Sites} MoveTo[t,source,dest] * Distance[source,dest]* TransportationCost[t] ); 

end;