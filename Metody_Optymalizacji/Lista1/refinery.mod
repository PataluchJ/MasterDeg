/* Jakub Pataluch 2023 */

set DistillationUnits;
set CrackingUnits;
set AllUnits;
set Products;

param Efficiency{Products, AllUnits};
param SulfurContent{AllUnits};
param SulfurLimit;
param ProductionCost{AllUnits};
param CrudeOilCost{DistillationUnits};

/* Help variables to get unit input */
var UnitInput{AllUnits} >= 0;

/* Variables for splitting distillate output to cracking unit or heavy fuel */ 
var distillateToCU{DistillationUnits} >= 0;
var distillateToHeavyFuel{DistillationUnits} >= 0;

s.t. distillateDestinationsAddsUpToProduction{du in DistillationUnits}:
    (distillateToCU[du] + distillateToHeavyFuel[du]) ==
    (UnitInput[du] * Efficiency["Distillat", du]);

/* Variables for splitting oil outputs between fuel types */
var oilToHomeFuel{AllUnits} >= 0; 
var oilToHeavyFuel{AllUnits} >= 0;

s.t. oilDestinationAddsUpToProduction{un in AllUnits}:
    oilToHomeFuel[un] + oilToHeavyFuel[un] == UnitInput[un] * Efficiency["Oil", un];

/* Setting units inputs */
s.t. distillateOutputOfB1ToCrackingUnitC1:
    distillateToCU["B1"] == UnitInput["C1"];
s.t. distillateOutputOfB2ToCrackingUnitC2:
    distillateToCU["B2"] == UnitInput["C2"];

/* Model specification */ 
/* As per diagram, oil from cracking unit can only go to home fuels. */
s.t. oilFromCrackingOnlyToHomeFuels{cu in CrackingUnits}:
    oilToHomeFuel[cu] == UnitInput[cu] * Efficiency["Oil", cu];

/* Minimal production reqs. */
s.t. engineFuelDemandMeet:
    sum{un in AllUnits}(UnitInput[un] * Efficiency["Gas", un]) >= 200000;

s.t. homeFuelDemandMeet:
    sum{un in AllUnits}(oilToHomeFuel[un]) >= 400000;

s.t. heavyFuelDemandMeet:
    sum{un in AllUnits}(UnitInput[un] * Efficiency["Remains", un]) +    # From remains
    sum{du in DistillationUnits}(distillateToHeavyFuel[du]) >= 250000;

/* Sulfur content quality requiraments meet. */
s.t. sulfurQuality: 
    sum{un in AllUnits}(oilToHomeFuel[un] * (SulfurContent[un]-SulfurLimit)) <= 0;

/* Min function */

minimize productionCost: 
    sum{un in AllUnits}(UnitInput[un] * ProductionCost[un]) +       # Production costs
    sum{du in DistillationUnits}(UnitInput[du] * CrudeOilCost[du]); # Crude oil costs

solve;

printf: "Solution:\n\tB1 = %f\n\tB2 = %f\nDTCU1 = %f DTCU2 = %f\nOTHoF1 = %f OTHoF2 = %f\nCost = %f\n", 
    UnitInput["B1"], UnitInput["B2"], distillateToCU["B1"], distillateToCU["B2"], 
    oilToHomeFuel["B1"], oilToHomeFuel["B2"],
    sum{un in AllUnits}(UnitInput[un] * ProductionCost[un]) + sum{du in DistillationUnits}(UnitInput[du] * CrudeOilCost[du]);
end; 
