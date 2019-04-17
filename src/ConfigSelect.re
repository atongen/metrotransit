open Belt;

type action =
  | SetProvider(option(Provider.t))
  | SetRoute(option(Route.t))
  | SetDirection(option(Direction.t))
  | SetStop(option(Stop.t))
  | SetExpanded(bool);

type state = {
  provider: option(Provider.t),
  route: option(Route.t),
  direction: option(Direction.t),
  stop: option(Stop.t),
  expanded: bool,
};

let component = ReasonReact.reducerComponent("ConfigSelect");

let s = ReasonReact.string;

let menuItems = configs =>
  List.map(configs, (config: Config.t) =>
    <MaterialUi.MenuItem key=config.id value=(`String(config.id))> (s(config.shortName)) </MaterialUi.MenuItem>
  );

let nativeMenuItems = configs =>
  List.map(configs, (config: Config.t) => <option key=config.id value=config.id> (s(config.shortName)) </option>);

let isEmpty =
  fun
  | Some(_) => false
  | None => true;

let makePanelSummary = (selected: option(Config.t)) => {
  let expandIcon = MaterialUi.(<Icon> (s("expand_more")) </Icon>);
  let (name, color) =
    switch (selected) {
    | Some(config) => (config.shortName, `Primary)
    | None => ("Select Departure", `Secondary)
    };
  MaterialUi.(
    <ExpansionPanelSummary expandIcon>
      <div> <Typography color> (s(name)) </Typography> </div>
    </ExpansionPanelSummary>
  );
};

let make = (~selected: option(Config.t), ~configs: list(Config.t), ~setConfig, _children) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | SetProvider(provider) => ReasonReact.Update({...state, provider, route: None, direction: None, stop: None})
    | SetRoute(route) => ReasonReact.Update({...state, route, direction: None, stop: None})
    | SetDirection(direction) => ReasonReact.Update({...state, direction, stop: None})
    | SetStop(stop) =>
      let maybeConfig = Config.maybeMake(state.route, state.direction, stop);
      switch (maybeConfig) {
      | Some(config) =>
        ReasonReact.UpdateWithSideEffects({...state, stop, expanded: false}, (_state => setConfig(config)))
      | None => ReasonReact.Update({provider: None, route: None, direction: None, stop: None, expanded: true})
      };
    | SetExpanded(expanded) => ReasonReact.Update({...state, expanded})
    },
  initialState: () => {provider: None, route: None, direction: None, stop: None, expanded: isEmpty(selected)},
  willReceiveProps: self => {...self.state, expanded: isEmpty(selected)},
  render: self => {
    let setProvider = provider => self.ReasonReact.send(SetProvider(provider));
    let setRoute = route => self.send(SetRoute(route));
    let setDirection = direction => self.send(SetDirection(direction));
    let setStop = stop => self.send(SetStop(stop));
    let toggleExpanded = (_f, _e) => self.send(SetExpanded(! self.state.expanded));
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
    let configChange = (evt, _el) => {
      let configId = ReactEvent.Form.target(evt)##value;
      let maybeConfig = List.getBy(configs, config => config.id == configId);
      switch (maybeConfig) {
      | Some(config) =>
        setConfig(config);
        self.ReasonReact.send(SetExpanded(false));
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
        let select =
          if (Util.isMobile()) {
            <MaterialUi.Select native=true value onChange=configChange>
              (nativeMenuItems(configs))
            </MaterialUi.Select>;
          } else {
            <MaterialUi.Select native=false value onChange=configChange> (menuItems(configs)) </MaterialUi.Select>;
          };
        MaterialUi.(
          <form autoComplete="off">
            <FormControl fullWidth=true>
              <InputLabel> (s("Previously Selected Departures")) </InputLabel>
              select
            </FormControl>
          </form>
        );
      } else {
        ReasonReact.null;
      };
    let panelSummary = makePanelSummary(selected);
    MaterialUi.(
      <ExpansionPanel expanded=self.state.expanded onChange=toggleExpanded>
        panelSummary
        <ExpansionPanelDetails>
          <Grid container=true>
            <Grid item=true xs=V12> configSelect </Grid>
            <Grid item=true xs=V12>
              <Typography variant=`H6> (s("Select New Departure")) </Typography>
              <ProviderSelect selected=self.state.provider setProvider />
              <RouteSelect selected=self.state.route provider=self.state.provider setRoute />
              directionSelect
              stopSelect
            </Grid>
          </Grid>
        </ExpansionPanelDetails>
      </ExpansionPanel>
    );
  },
};
