type directionId = string;

type directionName = string;

type t = {
  id: directionId,
  name: directionName,
};

let make = (directionId, directionName) => {id: directionId, name: directionName};

let directionIdKey = "Value";

let directionNameKey = "Text";

let decodeDirection = str =>
  Json.Decode.{id: str |> field(directionIdKey, string), name: str |> field(directionNameKey, string)};

let encodeDirection = p =>
  Json.Encode.(object_([(directionNameKey, string(p.name)), (directionIdKey, string(p.id))]));

let ofJson = str => Util.parseJsonList(str, decodeDirection);

let toJson = pList => Util.encodeJsonList(pList, encodeDirection);
