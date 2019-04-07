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
  let r = Util.truncateByToken(route.name, 3);
  let d = Js.String.replace("bound", "", Util.capitalize(direction.name));
  let s = Util.truncateByToken(stop.name, 3);
  Printf.sprintf("%s - %s - %s", r, d, s);
};

let make = (maybeRoute: option(Route.t), maybeDirection: option(Direction.t), maybeStop: option(Stop.t)) =>
  switch (maybeRoute, maybeDirection, maybeStop) {
  | (Some(route), Some(direction), Some(stop)) =>
    Some({
      id: Printf.sprintf("%s-%s-%s", route.id, direction.id, stop.id),
      name: Printf.sprintf("%s - %s - %s", route.name, Util.capitalize(direction.name), stop.name),
      shortName: makeShortName(route, direction, stop),
      route: route.id,
      direction: direction.id,
      stop: stop.id,
    })
  | _ => None
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
