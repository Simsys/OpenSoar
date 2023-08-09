set(_SOURCES
        Engine/Airspace/AbstractAirspace.cpp
        Engine/Airspace/Airspace.cpp
        Engine/Airspace/AirspaceAircraftPerformance.cpp
        Engine/Airspace/AirspaceAltitude.cpp
        Engine/Airspace/AirspaceCircle.cpp
        Engine/Airspace/AirspaceIntersectionVisitor.cpp
        Engine/Airspace/AirspaceIntersectSort.cpp
        Engine/Airspace/AirspacePolygon.cpp
        Engine/Airspace/Airspaces.cpp
        Engine/Airspace/AirspaceSorter.cpp
        Engine/Airspace/AirspacesTerrain.cpp
        Engine/Airspace/AirspaceWarning.cpp
        Engine/Airspace/AirspaceWarningConfig.cpp
        Engine/Airspace/AirspaceWarningManager.cpp
        # Engine/Airspace/Predicate/AirspacePredicate.cpp
        Engine/Airspace/Predicate/AirspacePredicateHeightRange.cpp
        Engine/Airspace/Predicate/OutsideAirspacePredicate.cpp
        Engine/Airspace/SoonestAirspace.cpp
        Engine/Contest/ContestManager.cpp
        Engine/Contest/Settings.cpp
        Engine/Contest/Solvers/AbstractContest.cpp
        Engine/Contest/Solvers/ContestDijkstra.cpp
        Engine/Contest/Solvers/Contests.cpp
        Engine/Contest/Solvers/Charron.cpp
        Engine/Contest/Solvers/DMStQuad.cpp
        Engine/Contest/Solvers/NetCoupe.cpp
        Engine/Contest/Solvers/OLCClassic.cpp
        Engine/Contest/Solvers/OLCFAI.cpp
        Engine/Contest/Solvers/OLCLeague.cpp
        Engine/Contest/Solvers/OLCPlus.cpp
        Engine/Contest/Solvers/OLCSISAT.cpp
        Engine/Contest/Solvers/OLCSprint.cpp
        Engine/Contest/Solvers/OLCTriangleRules.cpp  # branch cmake OLCTriangle ->OLCTriangleRules
        Engine/Contest/Solvers/WeglideDistance.cpp 
        Engine/Contest/Solvers/WeglideOR.cpp 
        Engine/Contest/Solvers/WeglideFree.cpp 
        Engine/Contest/Solvers/WeglideFAI.cpp 
        Engine/Contest/Solvers/TriangleContest.cpp 
        Engine/Contest/Solvers/Retrospective.cpp
        Engine/Contest/Solvers/TraceManager.cpp
        Engine/Contest/Solvers/XContestFree.cpp
        Engine/Contest/Solvers/XContestTriangle.cpp
        Engine/GlideSolvers/GlidePolar.cpp
        Engine/GlideSolvers/GlideResult.cpp
        Engine/GlideSolvers/GlideSettings.cpp
        Engine/GlideSolvers/GlideState.cpp
        Engine/GlideSolvers/GlueGlideState.cpp
        Engine/GlideSolvers/InstantSpeed.cpp
        Engine/GlideSolvers/MacCready.cpp
        Engine/Navigation/Aircraft.cpp
        Engine/Navigation/TraceHistory.cpp
        Engine/Route/AirspaceRoute.cpp
        Engine/Route/Config.cpp
        Engine/Route/FlatTriangleFan.cpp
        Engine/Route/FlatTriangleFanTree.cpp
        Engine/Route/ReachFan.cpp
        Engine/Route/RouteLink.cpp
        Engine/Route/RoutePlanner.cpp
        Engine/Route/RoutePolar.cpp
        Engine/Route/RoutePolars.cpp
        Engine/Route/TerrainRoute.cpp
        Engine/Task/AbstractTask.cpp
        Engine/Task/Computer/DistanceStatComputer.cpp
        Engine/Task/Computer/ElementStatComputer.cpp
        Engine/Task/Computer/IncrementalSpeedComputer.cpp
        Engine/Task/Computer/TaskStatsComputer.cpp
        Engine/Task/Computer/TaskVarioComputer.cpp
        Engine/Task/Computer/WindowStatsComputer.cpp
        Engine/Task/Factory/AATTaskFactory.cpp
        Engine/Task/Factory/AbstractTaskFactory.cpp
        Engine/Task/Factory/Create.cpp
        Engine/Task/Factory/FAIGoalTaskFactory.cpp
        Engine/Task/Factory/FAIORTaskFactory.cpp
        Engine/Task/Factory/FAITaskFactory.cpp
        Engine/Task/Factory/FAITriangleTaskFactory.cpp
        Engine/Task/Factory/MatTaskFactory.cpp
        Engine/Task/Factory/MixedTaskFactory.cpp
        Engine/Task/Factory/RTTaskFactory.cpp
        Engine/Task/Factory/TouringTaskFactory.cpp
        Engine/Task/ObservationZones/AnnularSectorZone.cpp
        Engine/Task/ObservationZones/Boundary.cpp
        Engine/Task/ObservationZones/CylinderZone.cpp
        Engine/Task/ObservationZones/KeyholeZone.cpp
        Engine/Task/ObservationZones/LineSectorZone.cpp
        Engine/Task/ObservationZones/ObservationZoneClient.cpp
        Engine/Task/ObservationZones/ObservationZonePoint.cpp
        Engine/Task/ObservationZones/SectorZone.cpp
        Engine/Task/ObservationZones/SymmetricSectorZone.cpp
        Engine/Task/Ordered/AATIsoline.cpp
        Engine/Task/Ordered/AATIsolineSegment.cpp
        Engine/Task/Ordered/FinishConstraints.cpp
        Engine/Task/Ordered/OrderedTask.cpp
        Engine/Task/Ordered/Points/AATPoint.cpp
        Engine/Task/Ordered/Points/ASTPoint.cpp
        Engine/Task/Ordered/Points/FinishPoint.cpp
        Engine/Task/Ordered/Points/IntermediatePoint.cpp
        Engine/Task/Ordered/Points/OrderedTaskPoint.cpp
        Engine/Task/Ordered/Points/StartPoint.cpp
        Engine/Task/Ordered/Settings.cpp
        Engine/Task/Ordered/SmartTaskAdvance.cpp
        Engine/Task/Ordered/StartConstraints.cpp
        Engine/Task/Ordered/TaskAdvance.cpp
        Engine/Task/PathSolvers/IsolineCrossingFinder.cpp
        Engine/Task/PathSolvers/TaskDijkstra.cpp
        Engine/Task/PathSolvers/TaskDijkstraMax.cpp
        Engine/Task/PathSolvers/TaskDijkstraMin.cpp
        Engine/Task/Points/SampledTaskPoint.cpp
        Engine/Task/Points/ScoredTaskPoint.cpp
        Engine/Task/Points/TaskLeg.cpp
        Engine/Task/Points/TaskPoint.cpp
        Engine/Task/Shapes/FAITriangleArea.cpp
        Engine/Task/Shapes/FAITrianglePointValidator.cpp
        Engine/Task/Shapes/FAITriangleRules.cpp
        Engine/Task/Shapes/FAITriangleSettings.cpp
        Engine/Task/Shapes/FAITriangleTask.cpp
        Engine/Task/Solvers/TaskBestMc.cpp
        Engine/Task/Solvers/TaskCruiseEfficiency.cpp
        Engine/Task/Solvers/TaskEffectiveMacCready.cpp
        Engine/Task/Solvers/TaskGlideRequired.cpp
        Engine/Task/Solvers/TaskMacCready.cpp
        Engine/Task/Solvers/TaskMacCreadyRemaining.cpp
        Engine/Task/Solvers/TaskMacCreadyTotal.cpp
        Engine/Task/Solvers/TaskMacCreadyTravelled.cpp
        Engine/Task/Solvers/TaskMinTarget.cpp
        Engine/Task/Solvers/TaskOptTarget.cpp
        Engine/Task/Solvers/TaskSolution.cpp
        Engine/Task/Solvers/TaskSolveTravelled.cpp
        Engine/Task/Stats/CommonStats.cpp
        Engine/Task/Stats/ElementStat.cpp
        Engine/Task/Stats/StartStats.cpp
        Engine/Task/Stats/TaskStats.cpp
        Engine/Task/TaskBehaviour.cpp
        Engine/Task/TaskManager.cpp
        Engine/Task/Unordered/AbortTask.cpp
        Engine/Task/Unordered/AlternateTask.cpp
        Engine/Task/Unordered/GotoTask.cpp
        Engine/Task/Unordered/UnorderedTask.cpp
        Engine/Task/Unordered/UnorderedTaskPoint.cpp
        Engine/ThermalBand/ThermalBand.cpp
        Engine/ThermalBand/ThermalEncounterBand.cpp
        Engine/ThermalBand/ThermalEncounterCollection.cpp
        Engine/ThermalBand/ThermalSlice.cpp
        Engine/Trace/Point.cpp
        Engine/Trace/Trace.cpp
        Engine/Trace/Vector.cpp
        Engine/Util/AircraftStateFilter.cpp
        Engine/Util/Gradient.cpp
        Engine/Waypoint/Waypoint.cpp
        Engine/Waypoint/Waypoints.cpp
#        Engine/Waypoint/WaypointVisitor.cpp
)

set(SCRIPT_FILES
    CMakeSource.cmake
)


