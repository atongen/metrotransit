open Belt;

module ConfigComparator =
  Id.MakeComparable(
    {
      type t = Config.t;
      let cmp = Config.compare;
    },
  );

let emptyConfigs = Set.fromArray([||], ~id=(module ConfigComparator));

type action =
  | SetConfig(Config.t);

type state = {
  config: option(Config.t),
  configs: Set.t(Config.t, ConfigComparator.identity),
};

let updateConfigs = (configs, config) => Set.(add(remove(configs, config), config));

let lis = configs => {
  let l = Set.toList(configs);
  let names = List.map(l, (config: Config.t) => config.name);
  String.concat(", ", names);
};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | SetConfig(config) =>
      ReasonReact.Update({config: Some(config), configs: updateConfigs(state.configs, config)})
    },
  initialState: () => {config: None, configs: emptyConfigs},
  render: self => {
    let setConfig = config => self.ReasonReact.send(SetConfig(config));
    let configName =
      switch (self.state.config) {
      | Some(c) => c.name
      | None => "[NONE]"
      };
    <div>
      <ConfigSelect selected=self.state.config configs=(Set.toList(self.state.configs)) setConfig />
      <div> (ReasonReact.string(configName)) </div>
      <div> (ReasonReact.string(lis(self.state.configs))) </div>
    </div>;
  },
};
