let valueKey = key => key ++ "-value";

let expirationKey = key => key ++ "-expires";

let setExpiringItemAsOf = (cacheKey, value, expiresIn, asOf) => {
  let expiration = string_of_float(asOf +. expiresIn);
  open Dom.Storage;
  localStorage |> setItem(expirationKey(cacheKey), expiration);
  localStorage |> setItem(valueKey(cacheKey), value);
};

let setExpiringItem = (key, value, expiresIn) => setExpiringItemAsOf(key, value, expiresIn, Js.Date.now());

let getValueItem = cacheKey => Dom.Storage.(localStorage |> getItem(valueKey(cacheKey)));

let getExpiringItemAsOf = (cacheKey, asOf) =>
  switch (Dom.Storage.(localStorage |> getItem(expirationKey(cacheKey)))) {
  | Some(expiration) =>
    let expiresAt = float_of_string(expiration);
    if (expiresAt > asOf) {
      None;
    } else {
      getValueItem(cacheKey);
    };
  | None => None
  };

let getExpiringItem = cacheKey => getExpiringItemAsOf(cacheKey, Js.Date.now());

/* make this a promise so we can use it seamlessly with Fetch */
let getSetExpiringItemAsOf = (cacheKey, f, expiresIn, asOf) =>
  switch (getExpiringItemAsOf(cacheKey, asOf)) {
  | Some(value) => Some(value)
  | None =>
    switch (f()) {
    | Some(value) =>
      setExpiringItemAsOf(cacheKey, value, expiresIn, asOf);
      Some(value);
    | None => None
    }
  };

let getSetExpiringItem = (cacheKey, f, expiresIn) => getSetExpiringItemAsOf(cacheKey, f, expiresIn, Js.Date.now());
