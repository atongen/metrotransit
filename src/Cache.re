let valueKey = cacheKey => cacheKey ++ "-value";

let expirationKey = cacheKey => cacheKey ++ "-expires";

let setExpiringItemAsOf = (cacheKey, value, expiresIn, asOf) => {
  let expiration = Js.Float.toString(asOf +. expiresIn);
  open Dom.Storage;
  localStorage |> setItem(expirationKey(cacheKey), expiration);
  localStorage |> setItem(valueKey(cacheKey), value);
  value;
};

let setExpiringItem = (key, value, expiresIn) => setExpiringItemAsOf(key, value, expiresIn, Js.Date.now());

let getValueItem = cacheKey => Dom.Storage.(localStorage |> getItem(valueKey(cacheKey)));

let getExpiringItemAsOf = (cacheKey, asOf) =>
  switch (Dom.Storage.(localStorage |> getItem(expirationKey(cacheKey)))) {
  | Some(expiration) =>
    let expiresAt = float_of_string(expiration);
    if (expiresAt < asOf) {
      None;
    } else {
      getValueItem(cacheKey);
    };
  | None => None
  };

let getExpiringItem = cacheKey => getExpiringItemAsOf(cacheKey, Js.Date.now());

let getSetExpiringItemAsOf = (cacheKey, f, expiresIn, asOf) =>
  switch (getExpiringItemAsOf(cacheKey, asOf)) {
  | Some(value) => Js.Promise.resolve(value)
  | None => Js.Promise.(f() |> then_(value => setExpiringItemAsOf(cacheKey, value, expiresIn, asOf) |> resolve))
  };

let getSetExpiringItem = (cacheKey, f, expiresIn) => getSetExpiringItemAsOf(cacheKey, f, expiresIn, Js.Date.now());
