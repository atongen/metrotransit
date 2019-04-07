open Belt;

type action =
  | SetProvider(option(Provider.t))
  | SetRoute(option(Route.t))
  | SetDirection(option(Direction.t))
  | SetStop(option(Stop.t));

type state = {
  provider: option(Provider.t),
  route: option(Route.t),
  direction: option(Direction.t),
  stop: option(Stop.t),
};

let component = ReasonReact.reducerComponent("ConfigSelect");

let displayName = (maybeRoute: option(Route.t), maybeDirection: option(Direction.t), maybeStop: option(Stop.t)) =>
  switch (maybeRoute, maybeDirection, maybeStop) {
  | (Some(route), Some(direction), Some(stop)) =>
    Printf.sprintf("%s %s %s", route.name, direction.name, stop.name)
  | _ => ""
  };

let menuItems = configs =>
  List.map(configs, (config: Config.t) =>
    <MaterialUi.MenuItem key=config.id value=(`String(config.id))>
      (ReasonReact.string(config.name))
    </MaterialUi.MenuItem>
  );

let make = (~selected: option(Config.t), ~configs: list(Config.t), ~setConfig, _children) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | SetProvider(provider) => ReasonReact.Update({provider, route: None, direction: None, stop: None})
    | SetRoute(route) => ReasonReact.Update({...state, route, direction: None, stop: None})
    | SetDirection(direction) => ReasonReact.Update({...state, direction, stop: None})
    | SetStop(stop) =>
      let newState = {...state, stop};
      let maybeConfig = Config.make(newState.route, newState.direction, newState.stop);
      switch(maybeConfig) {
      | Some(config) => setConfig(config)
      | None => ()
      }
      ReasonReact.Update(newState);
    },
  initialState: () => {provider: None, route: None, direction: None, stop: None},
  render: self => {
    let setProvider = provider => self.ReasonReact.send(SetProvider(provider));
    let setRoute = route => self.ReasonReact.send(SetRoute(route));
    let setDirection = direction => self.ReasonReact.send(SetDirection(direction));
    let setStop = stop => self.ReasonReact.send(SetStop(stop));
    let directionSelect =
      switch (self.state.route) {
      | Some(route) => <DirectionSelect selected=self.state.direction route setDirection />
      | None => ReasonReact.null
      };
    let stopSelect =
      switch (self.state.route, self.state.direction) {
      | (Some(r), Some(d)) => <StopSelect selected=self.state.stop route=r direction=d setStop />
      | _ => ReasonReact.null
      };
    let expandIcon = MaterialUi.(<Icon> (ReasonReact.string("expand_more")) </Icon>);
    let configChange = (evt, _el) => {
      let configId = ReactEvent.Form.target(evt)##value;
      let maybeConfig = List.getBy(configs, config => config.id == configId);
      switch (maybeConfig) {
      | Some(config) => setConfig(config)
      | None => ()
      };
    };
    let configSelect =
      if (List.length(configs) > 0) {
        let value =
          switch (selected) {
          | Some(config) => `String(config.id)
          | None => `String("")
          };
        <form autoComplete="off">
          MaterialUi.(
            <FormControl>
              <InputLabel> (ReasonReact.string("My Stop")) </InputLabel>
              <Select value onChange=configChange> (menuItems(configs)) </Select>
            </FormControl>
          )
        </form>;
      } else {
        ReasonReact.null;
      };
    MaterialUi.(
      <ExpansionPanel>
        <ExpansionPanelSummary expandIcon>
          <div> <Typography> (ReasonReact.string("Select Stop")) </Typography> </div>
          <div>
            <Typography>
              (ReasonReact.string(displayName(self.state.route, self.state.direction, self.state.stop)))
            </Typography>
          </div>
        </ExpansionPanelSummary>
        <ExpansionPanelDetails>
          <div>
            configSelect
            <ProviderSelect selected=self.state.provider setProvider />
            <RouteSelect selected=self.state.route provider=self.state.provider setRoute />
            directionSelect
            stopSelect
          </div>
        </ExpansionPanelDetails>
      </ExpansionPanel>
    );
  },
};
