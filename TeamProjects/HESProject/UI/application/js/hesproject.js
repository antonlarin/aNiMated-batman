
var hesproject = (function() {
   
   // jQuery trigger to model error reporting
   function reportError(errObj) {
      $(window).trigger("modelError", errObj);
   }

   try { var solverProvider = require("solver"); }
   catch (e) 
   { 
      // stop intialization if solver loading failed
      reportError("Can't load solver module: " + e.toString());
      return; 
   }
   

   function loadSolverDescriptors() {
      try { 
         return solverProvider.getSolverDescriptions(); 
      }
      catch (e) { 
         reportError(e); 
      }
      return null;
   }

   function getDefaultSolverDescriptor() {
      var descriptors = solverProvider.getSolverDescriptions();
      if (descriptors.length == 0) {
         reportError("Can't find solver descriptors.");
         return null;
      }
      return descriptors[0];
   }


   // default solver parameters
   var defaultSolverTimeLimit       = 1;
   var defaultSolverDimensionsX     = 10;
   var defaultSolverDimensionsT     = 100;
   var defaultSolverInitConditions  = "0";
   var defaultSolverLeftConditions  = "0";
   var defaultSolverRightConditions = "0";
   var defaultSolverRHSFunction     = "0";
   var defaultSolverDescriptor = getDefaultSolverDescriptor();

   function MainModel() {
      this.solverDescriptors        = loadSolverDescriptors();
      this.solverSelectedDescriptor = ko.observable(defaultSolverDescriptor);
      this.solverTimeLimit          = ko.observable(defaultSolverTimeLimit);
      this.solverDimensionsX        = ko.observable(defaultSolverDimensionsX);
      this.solverDimensionsT        = ko.observable(defaultSolverDimensionsT);
      this.solverInitConditions     = ko.observable(defaultSolverInitConditions);
      this.solverLeftConditions     = ko.observable(defaultSolverLeftConditions);
      this.solverRightConditions    = ko.observable(defaultSolverRightConditions);
      this.solverRHSFunction        = ko.observable(defaultSolverRHSFunction);
      this.resultTable              = ko.observableArray(undefined);

      this.solveClick = function() {
         if (this.solverSelectedDescriptor == null) {
            reportError("Solver not selected.");
            return;
         }

         // set native solver parameters from model
         try {
            var timeLimit   = parseFloat(this.solverTimeLimit());
            var dimensionsX = parseInt(this.solverDimensionsX());
            var dimensionsT = parseInt(this.solverDimensionsT());
            console.log(timeLimit + ", " + dimensionsT + ", " + dimensionsX);

            var solver = solverProvider.provide(this.solverSelectedDescriptor());
            solver.setTimeLimit(timeLimit);
            solver.setDimensions(dimensionsX, dimensionsT);
            solver.setInitialConditions(this.solverInitConditions());
            solver.leftBoundaryCondition(this.solverLeftConditions());
            solver.rightBoundaryCondition(this.solverRightConditions());
            solver.setRHSFunction(this.solverRHSFunction());
            solver.solve();
         }
         catch (e) {
            reportError("Solver initialization error: " + e.toString());
            return;
         }

         // debug only
         var layer = solver.getLayer(0);
         console.log(layer);
         this.resultTable(solver.getTable());
      };
   }


   var modelInstance = null;
   function getMainModel() {
      if (modelInstance == null)
         modelInstance = new MainModel();

      return modelInstance;
   }

   return {
      getMainModel: getMainModel
   };

})();
