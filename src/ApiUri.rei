let loadProviders: unit => Js.Promise.t(Belt.Result.t(list(Provider.t), string));

let loadRoutes: unit => Js.Promise.t(Belt.Result.t(list(Route.t), string));

let loadDirections: Direction.directionId => Js.Promise.t(Belt.Result.t(list(Direction.t), string));

let loadStops: (Route.routeId, Direction.directionId) => Js.Promise.t(Belt.Result.t(list(Stop.t), string));

let loadTimepointDepartures:
  (Route.routeId, Direction.directionId, Stop.stopId) => Js.Promise.t(Belt.Result.t(list(Departure.t), string));

let loadVehicleLocations: Route.routeId => Js.Promise.t(Belt.Result.t(list(Location.t), string));
