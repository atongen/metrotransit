open Belt.Result;

let set = (storageKey, value) => {
  Dom.Storage.(localStorage |> setItem(storageKey, value));
};

let get = storageKey => Dom.Storage.(localStorage |> getItem(storageKey));

let del = storageKey => Dom.Storage.(localStorage |> removeItem(storageKey));

let configKey = "metrotransit-app-config";

let setConfig = config =>
    switch(config) {
    | Some(c) => {
        let json = Config.toJson(c);
        set(configKey, json);
    };
    | None => del(configKey)
};

let getConfig = () =>
  switch (get(configKey)) {
  | Some(json) => switch(Config.ofJson(json)) {
    | Ok(config) => Some(config)
    | Error(err) => {
        Js.log("Error getting selected config: " ++ err);
        None
    }
  }
  | None => None
  };

let configsKey = "metrotransit-app-configs";

let setConfigs = configs => {
  let json = Config.toJsonList(configs);
  set(configsKey, json);
};

let getConfigs = () =>
  switch (get(configsKey)) {
  | Some(json) => switch(Config.ofJsonList(json)) {
    | Ok(configs) => configs
    | Error(err) => {
        Js.log("Error getting selected config: " ++ err);
        []
    }
  }
  | None => []
  };
