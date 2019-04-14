open Belt;
open Belt.Result;

type action =
  | SetConfig(Config.t);

type state = {
  config: option(Config.t),
  configs: list(Config.t),
};

let maxConfigsSize = 5;

let updateConfigs = (configs, config: Config.t) => {
    let removed = List.keep(configs, (c: Config.t) => c.id != config.id)
    let truncated = switch(List.take(removed, maxConfigsSize - 1)) {
    | Some(t) => t
    | None => removed
    };
    List.add(truncated, config);
};

let urlChanged = (self, url) =>
    switch(self.ReasonReact.state.config) {
    | Some(config) =>
        if (url.ReasonReact.Router.hash != "") {
            Js.Promise.(
                Config.fromHash(url.hash)
                |> then_(result =>
                    switch(result) {
                    | Ok(config: Config.t) =>
                        if (url.hash != config.id) {
                            resolve(self.ReasonReact.send(SetConfig(config)))
                        } else {
                            resolve(())
                        }
                    | Error(err) => resolve(Js.log("Error loading config from url: " ++ err));
                    }
                )
                |> ignore
            );
        } else {
            Js.log("not updating")
        };
    | None => ()
    };

let loadState = () =>
  { config: ConfigStorage.getConfig(), configs: ConfigStorage.getConfigs() };

let persistState = self => {
    ConfigStorage.setConfig(self.ReasonReact.state.config)
    ConfigStorage.setConfigs(self.state.configs)
};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | SetConfig(config) =>
      ReasonReact.UpdateWithSideEffects(
        {config: Some(config), configs: updateConfigs(state.configs, config)},
        self => {
            ReasonReact.Router.push("#" ++ config.id)
            persistState(self)
        }
      )
    },
  initialState: loadState,
  didMount: self => {
    let token = ReasonReact.Router.watchUrl(url => urlChanged(self, url))
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(token));
  },
  render: self => {
    let setConfig = config => self.ReasonReact.send(SetConfig(config));
    let departureList = switch(self.state.config) {
    | Some(config) => <DepartureList config />
    | None => ReasonReact.null
    };
    <div>
        <ConfigSelect selected=self.state.config configs=self.state.configs setConfig />
        departureList
    </div>;
  },
};
