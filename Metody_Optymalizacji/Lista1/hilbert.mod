/* Jakub Pataluch 2023 */
param N;

set Index := {1..N};

param A{i in Index,j in Index} := 1 / (i + j-1);
param B{i in Index} := sum{j in Index} (1 / (i + j - 1));
param C{i in Index} := sum{j in Index} (1 / (i + j - 1));

var X{Index} >= 0;

s.t. eq_constraint{i in Index}: sum{j in Index} (A[i,j] * X[j]) == B[i];

minimize cost : sum{i in Index} C[i] * X[i];

solve;

printf: "Error for n = %i: %f \n",N, (sqrt(sum{i in Index}(abs(1-X[i])**2)))/(sqrt(sum{i in Index}(1)));
end;