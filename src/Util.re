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

let round = v =>
    if (v < 0.0) {
        ceil(v -. 0.5);
    } else {
        floor(v +. 0.5);
    };

let roundi = v => int_of_float(round(v));

let distanceOfTimeInWords = (fromTime, toTime) => {
    let sec = d => Js.Date.valueOf(d) /. 1000.0;
    let distanceInSeconds = sec(toTime) -. sec(fromTime);
    let rel =
      if (distanceInSeconds > 0.0) {
        "from now";
      } else {
        "ago";
      };
    let dis = abs_float(distanceInSeconds);
    let dim = dis /. 60.0;
    let r = roundi;
    let s = Printf.sprintf;
    let dot =
      if (dim < 1.0) {
        if (dis < 5.0) {
          "less than 5 seconds";
        } else if (5.0 <= dis && dis < 10.0) {
          "less than 10 seconds";
        } else if (10.0 <= dis && dis < 20.0) {
          "less than 20 seconds";
        } else if (20.0 <= dis && dis < 40.0) {
          "half a minute";
        } else {
          "less than a minute";
        };
      } else if (2.0 <= dim && dim < 45.0) {
        s("%d minutes", r(dim));
      } else if (45.0 <= dim && dim < 90.0) {
        "about 1 hour";
      } else if (90.0 <= dim && dim < 1440.0) {
        s("about %d hours", r(dim /. 60.0));
      } else if (1440.0 <= dim && dim < 2520.0) {
        "about 1 day";
      } else if (2520.0 <= dim && dim < 43200.0) {
        s("about %d days", r(dim /. 1440.0));
      } else if (43200.0 <= dim && dim < 86400.0) {
        s("about %d months", r(dim /. 43200.0));
      } else if (86400.0 <= dim && dim < 525600.0) {
        s("%d months", r(dim /. 43200.0));
      } else {
        let diy = dim /. 525949.2;
        let rounded = r(diy);
        let rem = diy -. float_of_int(rounded);
        if (rem < 0.25) {
          s("over %d years", rounded);
        } else if (0.25 <= rem && rem < 0.75) {
          s("about %d and one half years", rounded);
        } else {
          s("under %d years", rounded);
        };
      };
    s("%s %s", dot, rel);
  };
