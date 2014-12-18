
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
   var defaultSolverDimensionsX     = 15;
   var defaultSolverDimensionsT     = 460;
   var defaultSolverInitConditions  = "sin(2*pi*x)";
   var defaultSolverLeftConditions  = "0";
   var defaultSolverRightConditions = "0";
   var defaultSolverRHSFunction     = "0";
   var defaultSampleFunction        = "exp(-4*pi^2*t)*sin(2*pi*x)";
   var defaultSolverDescriptor = getDefaultSolverDescriptor();

   var maximumTableColumnsNumber = 10;
   var maximumTableRowsNumber = 10;
   var maxPlotPoints = 200;

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

      this.solver          = undefined;
      this.taskIsSolved    = ko.observable(false);
      this.errorNorm       = ko.observable(undefined);
      this.sampleFunction  = ko.observable(defaultSampleFunction);

      // View specific properties
      this.showingTable    = ko.observable(false);
      this.displayedTable  = ko.observableArray(undefined);
      this.currentLayer    = ko.observable(0);
      this.startTIndex     = ko.observable(0);
      this.startXIndex     = ko.observable(0);

      this.resultTable = null;
      this.plot = null;
      this.maxTableValue = 0;

      this.displayLayer = function(layer) {
         if (this.plot !== null) {
            this.plot.destroy();
            this.plot = null;
         }
         var currentLayerData = this.solver.getLayer(layer);
         var plottingData = [];
         var x = 0.0;
         var xStep = 1 / this.solverDimensionsX();
         var plottingStep;
         var layerValue = 0;
         if (currentLayerData.length <= maxPlotPoints)
            plottingStep = 1;
         else
            plottingStep = Math.ceil(currentLayerData.length / maxPlotPoints);

         for (var i = 0; i < currentLayerData.length - 1; i += plottingStep) {
            layerValue = currentLayerData[i]
            plottingData.push([x, layerValue]);
            x += plottingStep * xStep;
         }
         layerValue = currentLayerData[currentLayerData.length - 1];
         plottingData.push([1, layerValue]);

         this.plot = $.jqplot("plotOutput", [plottingData], {
            axes: { 
               yaxis: { 
                  min: this.minTableValue - 0.2,
                  max: this.maxTableValue + 0.2 
               },
               xaxis: {
                  min: -0.1,
                  max: 1.1
               }
            },
            seriesDefaults: {
               markerOptions: {
                  show: false
               }
            }
         });
      };

      this.isFirstLayer = ko.pureComputed(function() {
         return this.currentLayer() <= 0;
      }, this);

      this.isLastLayer = ko.pureComputed(function() {
         return this.currentLayer() >= parseInt(this.solverDimensionsT());
      }, this);

      this.changeLayer = function(shift) {
         var n = this.currentLayer();
         this.currentLayer(n + shift);
         if (this.taskIsSolved())
            this.displayLayer(this.currentLayer());
      };

      this.goToLayers = function() {
         if (this.showingTable())
            this.showingTable(false);
      };

      this.goToTable = function() {
         if (!this.showingTable())
            this.showingTable(true);
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

            this.updateDisplayedTable(this.solver.getTable());
            this.taskIsSolved(true);
            this.displayLayer(this.currentLayer());
            var layer = this.solver.getLayer(1);
            var layerNorm = 0;
            var layerNormSquared = 0;
            for (var i = 0; i < layer.length; i++)
               layerNormSquared += layer[i] * layer[i];
            layerNorm = Math.sqrt(layerNormSquared);

            alert("Problem solved.\nX nodes: ".concat(
                  (dimensionsX + 1).toString().concat(
                  "\nT nodes: ".concat(
                  (dimensionsT + 1).toString().concat(
                  "\nSecond layer's norm: ".concat(
                  layerNorm.toString()))))));
         }
         catch (e) {
            reportError("Solver initialization error: " + e.toString());
            return;
         }
      };

      this.updateDisplayedTable = function(table) {
         this.resultTable = table;
         this.updateMaxAndMinTableValues();
         this.prepareTableForDisplay();
      };

      this.prepareTableForDisplay = function() {
         this.displayedTable([]);
         var k = parseInt(this.startXIndex());
         var l = parseInt(this.startTIndex());
         var xBegin = Math.max(k, 0);
         var tBegin = Math.max(l, 0);
         var xEnd = Math.min(k + maximumTableColumnsNumber,
                             parseInt(this.solverDimensionsX()) + 1);
         var tEnd = Math.min(l + maximumTableRowsNumber,
                             parseInt(this.solverDimensionsT()) + 1);
         for (var i = tBegin; i < tEnd; i++) {
            this.displayedTable.push(ko.observableArray([]));
            for (var j = xBegin; j < xEnd; j++) {
               this.displayedTable()[i - tBegin].push(this.resultTable[i][j].toExponential(4));
               l++;
            }
            k++;
         }
      };

      this.updateMaxAndMinTableValues = function() {
         this.maxTableValue = this.solver.getMaxTableValue();
         this.minTableValue = this.solver.getMinTableValue();
      };

      this.compareWithExactSolution = function() {
         if (this.sampleFunction() === "")
            $(".ui.small.fluid.action.input").addClass("error");
         else {
            $(".ui.small.fluid.action.input").removeClass("error");
            this.errorNorm(this.solver.getErrorNorm(this.sampleFunction()));

            alert("Norm of error: " + this.errorNorm().toExponential(6));
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
