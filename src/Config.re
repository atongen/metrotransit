open Belt;

type configId = string;

type configName = string;

type t = {
  id: configId,
  route: Route.t,
  routes: list(Route.t),
  direction: Direction.t,
  directions: list(Direction.t),
  stop: Stop.t,
  stops: list(Stop.t),
};

let name = config =>
  Printf.sprintf("%s - %s - %s", config.route.name, Util.capitalize(config.direction.name), config.stop.name);

let shortName = config => {
  let r = Util.truncateByToken(config.route.name, 2);
  let d = Js.String.replace("bound", "", Util.capitalize(config.direction.name));
  let s = Util.truncateByToken(config.stop.name, 2);
  Printf.sprintf("%s - %s - %s", r, d, s);
};

let makeId = (route: Route.t, direction: Direction.t, stop: Stop.t) =>
  Printf.sprintf("%s-%s-%s", route.id, direction.id, stop.id);

let make = (~routes=[], ~directions=[], ~stops=[], route: Route.t, direction: Direction.t, stop: Stop.t) => {
  id: makeId(route, direction, stop),
  route,
  routes,
  direction,
  directions,
  stop,
  stops,
};

let maybeMake = (maybeRoute: option(Route.t), maybeDirection: option(Direction.t), maybeStop: option(Stop.t)) =>
  switch (maybeRoute, maybeDirection, maybeStop) {
  | (Some(route), Some(direction), Some(stop)) => Some(make(route, direction, stop))
  | _ => None
  };

let load = (routeId: Route.routeId, directionId: Direction.directionId, stopId: Stop.stopId) => {
  let m = make;
  Js.Promise.(
    Result.(
      ApiUri.loadRoutes()
      |> then_(routeResult =>
           switch (routeResult) {
           | Ok(routes) =>
             let maybeRoute = List.getBy(routes, (route: Route.t) => route.id == routeId);
             switch (maybeRoute) {
             | Some(route) =>
               ApiUri.loadDirections(route.id)
               |> then_(directionResult =>
                    switch (directionResult) {
                    | Ok(directions) =>
                      let maybeDirection =
                        List.getBy(directions, (direction: Direction.t) => direction.id == directionId);
                      switch (maybeDirection) {
                      | Some(direction) =>
                        ApiUri.loadStops(route.id, direction.id)
                        |> then_(stopResult =>
                             switch (stopResult) {
                             | Ok(stops) =>
                               let maybeStop = List.getBy(stops, (stop: Stop.t) => stop.id == stopId);
                               switch (maybeStop) {
                               | Some(stop) => resolve(Ok(m(~routes, ~directions, ~stops, route, direction, stop)))
                               | None => resolve(Error("stop id not found"))
                               };
                             | Error(_err) as e => resolve(e)
                             }
                           )
                      | None => resolve(Error("direction id not found"))
                      };
                    | Error(_err) as e => resolve(e)
                    }
                  )
             | None => resolve(Error("route id not found"))
             };
           | Error(_err) as e => resolve(e)
           }
         )
    )
  );
};

let fromHash = hash => {
  let ar = Js.String.split("-", hash);
  if (Array.length(ar) == 3) {
    switch (ar[0], ar[1], ar[2]) {
    | (Some(routeId), Some(directionId), Some(stopId)) => load(routeId, directionId, stopId)
    | _ => Js.Promise.resolve(Belt.Result.Error("invalid config hash"))
    };
  } else {
    Js.Promise.resolve(Belt.Result.Error("invalid config hash"));
  };
};

let configRouteKey = "r";

let configDirectionKey = "d";

let configStopKey = "s";

/* HERE */
let decodeConfig = str => {
  let routeJson = Json.Decode.(str |> field(configRouteKey, option(string)));
  Json.Decode.{
    id: str |> field(configIdKey, string),
    name: str |> field(configNameKey, string),
    shortName: str |> field(configShortNameKey, string),
    routeId: str |> field(configRouteIdKey, string),
    route: None,
    routes: [],
    directionId: str |> field(configDirectionIdKey, string),
    direction: None,
    directions: [],
    stopId: str |> field(configStopIdKey, string),
    stop: None,
    stops: [],
  };
};

let encodeConfig = c =>
  Json.Encode.(
    object_([
      (configIdKey, string(c.id)),
      (configNameKey, string(c.name)),
      (configShortNameKey, string(c.shortName)),
      (configRouteIdKey, string(c.routeId)),
      (configDirectionIdKey, string(c.directionId)),
      (configStopIdKey, string(c.stopId)),
    ])
  );

let ofJson = str => Util.parseJson(str, decodeConfig);

let toJson = c => Util.encodeJson(c, encodeConfig);

let ofJsonList = str => Util.parseJsonList(str, decodeConfig);

let toJsonList = cList => Util.encodeJsonList(cList, encodeConfig);
