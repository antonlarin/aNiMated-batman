
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

   // created solver objects dictionary
   var instancedSolvers = { }
   function createSolver(desciptor) {
      var solver = instancedSolvers[desciptor.name];
      if (solver)
         return solver;

      solver = solverProvider.provide(desciptor);
      instancedSolvers[desciptor.name] = solver;
      return solver;
   }


   // default solver parameters
   var defaultSolverTimeLimit       = 1;
   var defaultSolverDimensionsX     = 2;
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

      // View specific properties
      this.displayedTable = ko.observableArray(undefined);
      this.currentLayer   = ko.observable(0);

      this.showingTable = false;

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

      this.isFirstLayer = ko.computed(function() {
         return this.currentLayer == 0;
      }.bind(this));

      this.isLastLayer = ko.computed(function() {
         console.log(typeof this.currentLayer)
         return this.currentLayer == parseInt(this.solverDimensionsX());
      }.bind(this));

      this.nextLayer = function() {
         var n = this.currentLayer();
         this.currentLayer(n + 1);
      };

      this.previousLayer = function() {
         var n = this.currentLayer();
         this.currentLayer(n - 1);
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

            this.solver = createSolver(this.solverSelectedDescriptor());
            this.solver.setTimeLimit(timeLimit);
            this.solver.setDimensions(dimensionsX, dimensionsT);
            this.solver.setInitialConditions(this.solverInitConditions());
            this.solver.leftBoundaryCondition(this.solverLeftConditions());
            this.solver.rightBoundaryCondition(this.solverRightConditions());
            this.solver.setRHSFunction(this.solverRHSFunction());
            this.solver.solve();
            this.taskIsSolved(true);
            this.displayLayer(this.currentLayer());
         }
         catch (e) {
            reportError("Solver initialization error: " + e.toString());
            return;
         }

         // debug only
         var layer = this.solver.getLayer(0);
         console.log(layer);
      };

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
