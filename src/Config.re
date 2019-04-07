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

let compare = (c0: t, c1: t) => Pervasives.compare(c0.id, c1.id);
