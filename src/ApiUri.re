type t =
  | ProvidersUri
  | RoutesUri
  | DirectionsUri(Route.routeId)
  | StopsUri(Route.routeId, Direction.directionId)
  | DeparturesUri(Stop.stopId)
  | TimepointDeparturesUri(Route.routeId, Direction.directionId, Stop.stopId)
  | VehicleLocationsUri(Route.routeId);

let protocol = "https";

let domain = "svc.metrotransit.org";

let basePath = "/nextrip";

let params = "format=json";

let path =
  fun
  | ProvidersUri => "/providers"
  | RoutesUri => "/routes"
  | DirectionsUri(routeId) => Printf.sprintf("/directions/%s", routeId)
  | StopsUri(routeId, directionId) => Printf.sprintf("/stops/%s/%s", routeId, directionId)
  | DeparturesUri(stopId) => Printf.sprintf("/%s", stopId)
  | TimepointDeparturesUri(routeId, directionId, stopId) => Printf.sprintf("/%s/%s/%s", routeId, directionId, stopId)
  | VehicleLocationsUri(routeId) => Printf.sprintf("/vehiclelocations/%s", routeId);

let toString = x => Printf.sprintf("%s://%s%s%s%s", protocol, domain, basePath, path(x), params);
