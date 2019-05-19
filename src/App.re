open Belt;
open Belt.Result;

type action =
  | LoadConfig(Config.t)
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
    if (url.ReasonReact.Router.hash != "") {
        Js.Promise.(
            Config.fromHash(url.hash)
            |> then_(result => {
                switch(result) {
                | Ok(config: Config.t) =>
                    if (url.hash != config.id) {
                        self.ReasonReact.send(SetConfig(config))
                    } else {
                        switch(self.state.config) {
                        | Some(config) => if (url.hash != config.id) {
                            self.ReasonReact.send(SetConfig(config))
                          }
                        | None => self.ReasonReact.send(SetConfig(config))
                        }
                    }
                | Error(err) => Js.log("Error loading config from url: " ++ err);
                };
                resolve(())
            })
            |> ignore
        );
    } else {
        switch(ConfigStorage.getConfig()) {
        | Some(config) => self.send(LoadConfig(config));
        | None => ()
        }
    };

let loadState = () =>
  { config: None, configs: ConfigStorage.getConfigs() };

let persistState = self => {
    ConfigStorage.setConfig(self.ReasonReact.state.config)
    ConfigStorage.setConfigs(self.state.configs)
};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | LoadConfig(config) =>
      ReasonReact.SideEffects(
        (
          self =>
            Js.Promise.(
              Config.reload(config)
              |> then_(result =>
                   switch (result) {
                   | Ok(reloadedConfig) => resolve(self.send(SetConfig(reloadedConfig)))
                   | Error(err) => {
                       Js.log("Loading config failed: " ++ err);
                       resolve(self.send(SetConfig(config)));
                     }
                   }
                 )
              |> ignore
            )
        ),
      )
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
    let url = ReasonReact.Router.dangerouslyGetInitialUrl();
    urlChanged(self, url);
  },
  render: self => {
    let setConfig = config => self.ReasonReact.send(LoadConfig(config));
    let departureList = switch(self.state.config) {
    | Some(config) => <DepartureList config />
    | None => ReasonReact.null
    };
    <div>
        <ConfigSelect config=self.state.config configs=self.state.configs setConfig />
        departureList
    </div>;
  },
};
