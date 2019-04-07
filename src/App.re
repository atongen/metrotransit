open Belt;

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

let loadState = () => {config: ConfigStorage.getConfig(), configs: ConfigStorage.getConfigs()};

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
        persistState,
      )
    },
  initialState: () => loadState(),
  render: self => {
    let setConfig = config => self.ReasonReact.send(SetConfig(config));
    <div> <ConfigSelect selected=self.state.config configs=self.state.configs setConfig /> </div>;
  },
};
