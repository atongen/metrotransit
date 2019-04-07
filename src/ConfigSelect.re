open Belt;

type action =
  | SetProvider(option(Provider.t))
  | SetRoute(option(Route.t))
  | SetDirection(option(Direction.t))
  | SetStop(option(Stop.t))
  | ToggleExpanded;

type state = {
  provider: option(Provider.t),
  route: option(Route.t),
  direction: option(Direction.t),
  stop: option(Stop.t),
  expanded: bool,
};

let component = ReasonReact.reducerComponent("ConfigSelect");

let menuItems = configs =>
  List.map(configs, (config: Config.t) =>
    <MaterialUi.MenuItem key=config.id value=(`String(config.id))>
      (ReasonReact.string(config.name))
    </MaterialUi.MenuItem>
  );

let isEmpty =
  fun
  | Some(_) => false
  | None => true;

let make = (~selected: option(Config.t), ~configs: list(Config.t), ~setConfig, _children) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | SetProvider(provider) => ReasonReact.Update({...state, provider, route: None, direction: None, stop: None})
    | SetRoute(route) => ReasonReact.Update({...state, route, direction: None, stop: None})
    | SetDirection(direction) => ReasonReact.Update({...state, direction, stop: None})
    | SetStop(stop) =>
      let maybeConfig = Config.make(state.route, state.direction, stop);
      switch (maybeConfig) {
      | Some(config) =>
        ReasonReact.UpdateWithSideEffects({...state, stop, expanded: false}, (_state => setConfig(config)))
      | None => ReasonReact.Update({provider: None, route: None, direction: None, stop: None, expanded: true})
      }
    | ToggleExpanded => ReasonReact.Update({...state, expanded: ! state.expanded})
    },
  initialState: () => {provider: None, route: None, direction: None, stop: None, expanded: isEmpty(selected)},
  render: self => {
    let setProvider = provider => self.ReasonReact.send(SetProvider(provider));
    let setRoute = route => self.send(SetRoute(route));
    let setDirection = direction => self.send(SetDirection(direction));
    let setStop = stop => self.send(SetStop(stop));
    let toggleExpanded = (_f, _e) => self.send(ToggleExpanded);
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
    let selectedName =
      switch (selected) {
      | Some(config) => config.shortName
      | None => "NONE"
      };
    let configChange = (evt, _el) => {
      let configId = ReactEvent.Form.target(evt)##value;
      let maybeConfig = List.getBy(configs, config => config.id == configId);
      switch (maybeConfig) {
      | Some(config) =>
        setConfig(config);
        self.ReasonReact.send(ToggleExpanded);
      | None => ()
      };
    };
    let configSelect =
      if (List.length(configs) > 1) {
        let value =
          switch (selected) {
          | Some(config) => `String(config.id)
          | None => `String("")
          };
        MaterialUi.(
          <form autoComplete="off">
            <FormControl>
              <InputLabel> (ReasonReact.string("Previously Selected Departures")) </InputLabel>
              <Select value onChange=configChange> (menuItems(configs)) </Select>
            </FormControl>
          </form>
        );
      } else {
        ReasonReact.null;
      };
    MaterialUi.(
      <ExpansionPanel expanded=self.state.expanded onChange=toggleExpanded>
        <ExpansionPanelSummary expandIcon>
          <div> <Typography> (ReasonReact.string("Departure")) </Typography> </div>
          <div> <Typography> (ReasonReact.string(selectedName)) </Typography> </div>
        </ExpansionPanelSummary>
        <ExpansionPanelDetails>
          <div>
            configSelect
            <Typography variant=`H6> (ReasonReact.string("Select New Departure")) </Typography>
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
