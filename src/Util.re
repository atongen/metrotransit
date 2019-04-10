open Belt;

open Belt.Result;

let squeeze = str => Js.String.replaceByRe([%re "/\\s+/g"], " ", str);

let unsafeParseJsonList = (str, decoder) => Json.(parseOrRaise(str) |> Decode.list(decoder));

let parseJsonList = (str, decoder) =>
  try (Ok(unsafeParseJsonList(str, decoder))) {
  | Json.ParseError(e) => Error("parse error: " ++ e)
  | Json.Decode.DecodeError(e) => Error("decode error: " ++ squeeze(e))
  };

let unsafeParseJson = (str, decoder) => Json.(parseOrRaise(str) |> decoder);

let parseJson = (str, decoder) =>
  try (Ok(unsafeParseJson(str, decoder))) {
  | Json.ParseError(e) => Error("parse error: " ++ e)
  | Json.Decode.DecodeError(e) => Error("decode error: " ++ squeeze(e))
  };

let encodeJsonList = (l, encoder) => Json.(l |> Encode.list(encoder) |> stringify);

let encodeJson = (j, encoder) => Json.(j |> encoder |> stringify);

let fetchUrl = url => Js.Promise.(Fetch.fetch(url) |> then_(Fetch.Response.text));

let getCachedUrl = url => Cache.getSetExpiringItem(url, () => fetchUrl(url), 30.0);

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

let ignoredStrs = ["-", "and"];

let truncateByToken = (str, numTokens) => {
    let rec aux = (l, acc) =>
        switch(l) {
        | [] => acc
        | [hd, ...tl] =>
            if (List.length(acc) >= numTokens) {
                acc
            } else {
                switch(hd) {
                | Some(v) =>
                    if (!List.some(ignoredStrs, s => s == v)) {
                        aux(tl, [v, ...acc])
                    } else {
                        aux(tl, acc)
                    }
                | None => aux(tl, acc)
                };
            };
        };
    let l = List.fromArray(Js.String.splitByRe([%re "/\\s+/"], String.trim(str)));
    aux(l, []) |> List.reverse |> String.concat(" ");
};
