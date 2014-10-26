
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

   var maximumTableColumnsNumber = 10;
   var maximumTableRowsNumber = 20;

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

      this.solver = undefined;
      this.taskIsSolved = ko.observable(false);

      this.displayLayer = function(layer) {
         var currentLayer = this.solver.getLayer(layer);
         var plottingData = [];
         var x = 0.0;
         var xStep = 1 / this.solverDimensionsX();
         for (var i = 0; i < currentLayer.length; i++) {
            plottingData.push([x, currentLayer[i]]);
            x += xStep;
         }
         $.jqplot("plotOutput", [plottingData]);
      };

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

            this.solver = solverProvider.provide(this.solverSelectedDescriptor());
            this.solver.setTimeLimit(timeLimit);
            this.solver.setDimensions(dimensionsX, dimensionsT);
            this.solver.setInitialConditions(this.solverInitConditions());
            this.solver.leftBoundaryCondition(this.solverLeftConditions());
            this.solver.rightBoundaryCondition(this.solverRightConditions());
            this.solver.setRHSFunction(this.solverRHSFunction());
            this.solver.solve();
            this.taskIsSolved(true);
            this.displayLayer(0);
         }
         catch (e) {
            reportError("Solver initialization error: " + e.toString());
            return;
         }

         // debug only
         var layer = this.solver.getLayer(0);
         console.log(layer);
      };

      // View specific properties
      this.displayedTable = ko.observableArray(undefined);
      this.showingTable = false;

      this.prepareTableForDisplay = function() {
         this.displayedTable = []
         var k = this.startXIndex;
         for (var i = 0; i < maximumTableRowsNumber; i++) {
            this.displayedTable.push([]);
            var l = this.startTIndex;
            for (var j = 0; j < maximumTableColumnsNumber; j++) {
               this.displayedTable[i].push(this.resultTable[k][l]);
               l++;
            }
            k++;
         }
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
