type providerId = string;

type providerName = string;

type t = {
  id: providerId,
  name: providerName,
};

let providerIdKey = "Value";

let providerNameKey = "Text";

let decodeProvider = str =>
  Json.Decode.{id: str |> field(providerIdKey, string), name: str |> field(providerNameKey, string)};

let encodeProvider = p =>
  Json.Encode.(object_([(providerNameKey, string(p.name)), (providerIdKey, string(p.id))]));

let ofJson = str => Util.parseJsonList(str, decodeProvider);

let toJson = pList => Util.encodeJsonList(pList, encodeProvider);
