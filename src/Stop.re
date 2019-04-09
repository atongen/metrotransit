type stopId = string;

type stopName = string;

type t = {
  id: stopId,
  name: stopName,
};

let make = (stopId, stopName) => {id: stopId, name: stopName};

let stopIdKey = "Value";

let stopNameKey = "Text";

let decodeStop = str => Json.Decode.{id: str |> field(stopIdKey, string), name: str |> field(stopNameKey, string)};

let encodeStop = p => Json.Encode.(object_([(stopNameKey, string(p.name)), (stopIdKey, string(p.id))]));

let ofJson = str => Util.parseJsonList(str, decodeStop);

let toJson = pList => Util.encodeJsonList(pList, encodeStop);
