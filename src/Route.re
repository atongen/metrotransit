type routeId = string;

type routeName = string;

type t = {
  id: routeId,
  name: routeName,
  providerId: Provider.providerId,
};

let make = (routeId, routeName, providerId) => {id: routeId, name: routeName, providerId};

let maybeMake = (maybeRouteId, maybeRouteName, maybeProviderId) =>
  switch (maybeRouteId, maybeRouteName, maybeProviderId) {
  | (Some(routeId), Some(routeName), Some(providerId)) => Some(make(routeId, routeName, providerId))
  | _ => None
  };

let routeIdKey = "Route";

let routeNameKey = "Description";

let routeProviderIdKey = "ProviderID";

let decodeRoute = str =>
  Json.Decode.{
    id: str |> field(routeIdKey, string),
    name: str |> field(routeNameKey, string),
    providerId: str |> field(routeProviderIdKey, string),
  };

let encodeRoute = r =>
  Json.Encode.(
    object_([
      (routeNameKey, string(r.name)),
      (routeProviderIdKey, string(r.providerId)),
      (routeIdKey, string(r.id)),
    ])
  );

let ofJson = str => Util.parseJsonList(str, decodeRoute);

let toJson = pList => Util.encodeJsonList(pList, encodeRoute);

let toSelectOption = route => SelectOption.make(route.name, route.id);
