open Belt;

type configId = string;

type configName = string;

type t = {
  id: configId,
  name: configName,
  shortName: string,
  route: Route.routeId,
  direction: Direction.directionId,
  stop: Stop.stopId,
};

let makeShortName = (route: Route.t, direction: Direction.t, stop: Stop.t) => {
  let r = Util.truncateByToken(route.name, 2);
  let d = Js.String.replace("bound", "", Util.capitalize(direction.name));
  let s = Util.truncateByToken(stop.name, 2);
  Printf.sprintf("%s - %s - %s", r, d, s);
};

let make = (route: Route.t, direction: Direction.t, stop: Stop.t) => {
  id: Printf.sprintf("%s-%s-%s", route.id, direction.id, stop.id),
  name: Printf.sprintf("%s - %s - %s", route.name, Util.capitalize(direction.name), stop.name),
  shortName: makeShortName(route, direction, stop),
  route: route.id,
  direction: direction.id,
  stop: stop.id,
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
                               | Some(stop) => resolve(Ok(m(route, direction, stop)))
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

let configIdKey = "i";

let configNameKey = "n";

let configShortNameKey = "sn";

let configRouteKey = "r";

let configDirectionKey = "d";

let configStopKey = "s";

let decodeConfig = str =>
  Json.Decode.{
    id: str |> field(configIdKey, string),
    name: str |> field(configNameKey, string),
    shortName: str |> field(configShortNameKey, string),
    route: str |> field(configRouteKey, string),
    direction: str |> field(configDirectionKey, string),
    stop: str |> field(configStopKey, string),
  };

let encodeConfig = c =>
  Json.Encode.(
    object_([
      (configIdKey, string(c.id)),
      (configNameKey, string(c.name)),
      (configShortNameKey, string(c.shortName)),
      (configRouteKey, string(c.route)),
      (configDirectionKey, string(c.direction)),
      (configStopKey, string(c.stop)),
    ])
  );

let ofJson = str => Util.parseJson(str, decodeConfig);

let toJson = c => Util.encodeJson(c, encodeConfig);

let ofJsonList = str => Util.parseJsonList(str, decodeConfig);

let toJsonList = cList => Util.encodeJsonList(cList, encodeConfig);
