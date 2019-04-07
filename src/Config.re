type configId = string;

type configName = string;

type t = {
  id: configId,
  name: configName,
  route: Route.routeId,
  direction: Direction.directionId,
  stop: Stop.stopId,
};

let make = (maybeRoute: option(Route.t), maybeDirection: option(Direction.t), maybeStop: option(Stop.t)) =>
  switch (maybeRoute, maybeDirection, maybeStop) {
  | (Some(route), Some(direction), Some(stop)) =>
    Some({
      id: Printf.sprintf("%s-%s-%s", route.id, direction.id, stop.id),
      name: Printf.sprintf("%s - %s - %s", route.name, direction.name, stop.name),
      route: route.id,
      direction: direction.id,
      stop: stop.id,
    })
  | _ => None
  };

let configIdKey = "i";

let configNameKey = "n";

let configRouteKey = "r";

let configDirectionKey = "d";

let configStopKey = "s";

let decodeConfig = str =>
  Json.Decode.{
    id: str |> field(configIdKey, string),
    name: str |> field(configNameKey, string),
    route: str |> field(configRouteKey, string),
    direction: str |> field(configDirectionKey, string),
    stop: str |> field(configStopKey, string),
  };

let encodeConfig = c =>
  Json.Encode.(
    object_([
      (configIdKey, string(c.id)),
      (configNameKey, string(c.name)),
      (configRouteKey, string(c.route)),
      (configDirectionKey, string(c.direction)),
      (configStopKey, string(c.stop)),
    ])
  );

let ofJson = str => Util.parseJson(str, decodeConfig);

let toJson = c => Util.encodeJson(c, encodeConfig);

let ofJsonList = str => Util.parseJsonList(str, decodeConfig);

let toJsonList = cList => Util.encodeJsonList(cList, encodeConfig);
