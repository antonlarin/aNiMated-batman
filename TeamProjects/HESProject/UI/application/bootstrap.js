// application loading started...

var solverProvider = require("solver")
var solverDescriptions = solverProvider.getSolverDescriptions();
var implicitSolverDescriptor = solverDescriptions[0];
var explicitSolverDescriptor = solverDescriptions[1];

var solver = solverProvider.provide(explicitSolverDescriptor);
console.log("Solver initialized");
console.log(explicitSolverDescriptor);
console.log(solver);

solver.setDimensions(200, 100000);
solver.setRHSFunction("cos(t)*sin(x)");
solver.setInitialConditions("sinh(x)");
solver.leftBoundaryCondition("t^2");
solver.rightBoundaryCondition("sqrt(t)");
solver.solve();

var layer = solver.getLayer(0);
console.log(layer);
