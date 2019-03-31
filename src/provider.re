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

let of_json = str => str |> Json.Decode.list(decodeProvider);

let encodeProvider = p => {
  open! Json.Encode;
  object_([(providerIdKey, string(p.id)), (providerNameKey, string(p.name))]);
};

let to_json = pList => {
  open! Json.Encode;
  pList |> list(encodeProvider);
};
