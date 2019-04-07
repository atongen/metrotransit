type t =
  | ProvidersUri
  | RoutesUri
  | DirectionsUri(Route.routeId)
  | StopsUri(Route.routeId, Direction.directionId)
  | TimepointDeparturesUri(Route.routeId, Direction.directionId, Stop.stopId)
  | VehicleLocationsUri(Route.routeId);

let toString: t => string;
