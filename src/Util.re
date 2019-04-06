open Belt;

open Belt.Result;

let squeeze = str => Js.String.replaceByRe([%re "/\\s+/g"], " ", str);

let unsafeParseJsonList = (str, decoder) => Json.(parseOrRaise(str) |> Decode.list(decoder));

let parseJsonList = (str, decoder) =>
  try (Ok(unsafeParseJsonList(str, decoder))) {
  | Json.ParseError(e) => Error("parse error: " ++ e)
  | Json.Decode.DecodeError(e) => Error("decode error: " ++ squeeze(e))
  };

let encodeJsonList = (l, encoder) => Json.(l |> Encode.list(encoder) |> stringify);

let fetchUrl = url => Js.Promise.(Fetch.fetch(url) |> then_(Fetch.Response.text));

let getCachedUrl = url => Cache.getSetExpiringItem(url, () => fetchUrl(url), 3600.0);

let compactMap = (f, l) => {
    let rec aux = (l, acc) => {
        switch(l) {
        | [] => acc
        | [hd, ...tl] => switch(hd) {
        | Some(v) => aux(tl, [f(v), ...acc])
        | None => aux(tl, acc)
        }
        }
    };
    aux(l, []);
};

let capitalize = str => {
    let aux = l => {
        compactMap(s => {
            open Js.String;
            if (length(s) == 1) {
                toUpperCase(s);
            } else {
                let len = length(s);
                let first = substring(~from=0, ~to_=1, s)
                let rest = substring(~from=1, ~to_=len, s)
                Printf.sprintf("%s%s", toUpperCase(first), toLowerCase(rest))
            }
        }, l);
    };
    aux(List.fromArray(Js.String.splitByRe([%re "/\\s+/"], String.trim(str))))
    |> String.concat(" ");
};