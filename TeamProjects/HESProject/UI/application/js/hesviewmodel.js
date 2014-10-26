var solverProvider = require("solver");

function hesViewModel() {
  var self = this;

  self.solversList = solverProvider.getSolverDescriptions();
  self.currentSolver = solverProvider.provide(self.solversList[0]);

  self.rhsFunctionExpr = ko.observable("0");
  self.icFunctionExpr = ko.observable("99*x+1");
  self.lbcFunctionExpr = ko.observable("1");
  self.rbcFunctionExpr = ko.observable("100");
  self.timeLimit = ko.observable("1");
  self.availableSolvers = ko.observableArray([ "Explicit", "Implicit" ]);

  self.solve = function() {
    self.currentSolver.setTimeLimit(parseFloat(self.timeLimit()));
    self.currentSolver.setInitialConditions(self.icFunctionExpr());
    self.currentSolver.leftBoundaryCondition(self.lbcFunctionExpr());
    self.currentSolver.rightBoundaryCondition(self.rbcFunctionExpr());
    self.currentSolver.setRHSFunction(self.rhsFunctionExpr());
    self.currentSolver.solve();
    console.log("Solved!");
    $.jqplot("output",  [[[1, 2],[3,5.12],[5,13.1],[7,33.6],[9,85.9],[11,219.9]]]);
  };

  self.showingTable = false;
  self.goToTable = function() {
    self.showingTable = true;
  }
  self.goToLayers = function() {
    self.showingTable = false;
  }
};

$(document).ready(function() {
  ko.applyBindings(new hesViewModel());
});
