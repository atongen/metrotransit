type t =
  | ProvidersUri
  | RoutesUri
  | DirectionsUri(Route.routeId)
  | StopsUri(Route.routeId, Direction.directionId)
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
  | TimepointDeparturesUri(routeId, directionId, stopId) => Printf.sprintf("/%s/%s/%s", routeId, directionId, stopId)
  | VehicleLocationsUri(routeId) => Printf.sprintf("/vehiclelocations/%s", routeId);

let toString = x => Printf.sprintf("%s://%s%s%s?%s", protocol, domain, basePath, path(x), params);

let loadProviders = () =>
  Js.Promise.(Util.getCachedUrl(toString(ProvidersUri)) |> then_(jsonStr => Provider.ofJson(jsonStr) |> resolve));

let loadRoutes = () =>
  Js.Promise.(Util.getCachedUrl(toString(RoutesUri)) |> then_(jsonStr => Route.ofJson(jsonStr) |> resolve));

let loadDirections = routeId =>
  Js.Promise.(
    Util.getCachedUrl(toString(DirectionsUri(routeId)))
    |> then_(jsonStr => Direction.ofJson(jsonStr) |> resolve)
  );

let loadStops = (routeId, directionId) =>
  Js.Promise.(
    Util.getCachedUrl(toString(StopsUri(routeId, directionId)))
    |> then_(jsonStr => Stop.ofJson(jsonStr) |> resolve)
  );

let loadTimepointDepartures = (routeId, directionId, stopId) =>
  Js.Promise.(
    Util.getCachedUrl(toString(TimepointDeparturesUri(routeId, directionId, stopId)))
    |> then_(jsonStr => Departure.ofJson(jsonStr) |> resolve)
  );

let loadVehicleLocations = routeId =>
  Js.Promise.(
    Util.getCachedUrl(toString(VehicleLocationsUri(routeId)))
    |> then_(jsonStr => Location.ofJson(jsonStr) |> resolve)
  );
