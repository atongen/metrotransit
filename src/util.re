open Belt.Result;

let squeeze = str => Js.String.replaceByRe([%re "/\\s+/g"], " ", str);

let unsafeParseJsonList = (str, decoder) => Json.(parseOrRaise(str) |> Decode.list(decoder));

let parseJsonList = (str, decoder) =>
  try (Ok(unsafeParseJsonList(str, decoder))) {
  | Json.ParseError(e) => Error("parse error: " ++ e)
  | Json.Decode.DecodeError(e) => Error("decode error: " ++ squeeze(e))
  };

let encodeJsonList = (l, encoder) => Json.(l |> Encode.list(encoder) |> stringify);
