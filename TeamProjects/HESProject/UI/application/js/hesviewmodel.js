var solverStorage = require("solver");

function hesViewModel() {
  var self = this;

  self.rhsFunctionExpr = ko.observable("0");
  self.icFunctionExpr = ko.observable("99*x+1");
  self.lbcFunctionExpr = ko.observable("1");
  self.rbcFunctionExpr = ko.observable("100");
  self.timeLimit = ko.observable("1");
  self.availableSolvers = ko.observableArray([ "Explicit", "Implicit" ]);

  self.solve = function() {
    console.log("Solve!")
  };
};

$(document).ready(function() {
  ko.applyBindings(new hesViewModel());
});
