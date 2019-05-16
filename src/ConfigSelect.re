open Belt;

type action =
  | SetProviders(list(Provider.t))
  | SetProvider(option(Provider.t))
  | SetRoutes(list(Route.t))
  | SetRoute(option(Route.t))
  | SetDirections(list(Direction.t))
  | SetDirection(option(Direction.t))
  | SetStops(list(Stop.t))
  | SetStop(option(Stop.t))
  | SetExpanded(bool);

type state = {
  providers: list(Provider.t),
  provider: option(Provider.t),
  routes: list(Route.t),
  route: option(Route.t),
  directions: list(Direction.t),
  direction: option(Direction.t),
  stops: list(Stop.t),
  stop: option(Stop.t),
  expanded: bool,
};

let component = ReasonReact.reducerComponent("ConfigSelect");

let s = ReasonReact.string;

let menuItems = configs =>
  List.map(configs, (config: Config.t) =>
    <MaterialUi.MenuItem key=config.id value=(`String(config.id))>
      (s(Config.shortName(config)))
    </MaterialUi.MenuItem>
  );

let nativeMenuItems = configs =>
  List.map(configs, (config: Config.t) =>
    <option key=config.id value=config.id> (s(Config.shortName(config))) </option>
  );

let isEmpty =
  fun
  | Some(_) => false
  | None => true;

let makeInitialState = expanded => {
  providers: [],
  provider: None,
  routes: [],
  route: None,
  directions: [],
  direction: None,
  stops: [],
  stop: None,
  expanded,
};

let makePanelSummary = (config: option(Config.t)) => {
  let expandIcon = MaterialUi.(<Icon> (s("expand_more")) </Icon>);
  let (name, color) =
    switch (config) {
    | Some(c) => (Config.shortName(c), `Primary)
    | None => ("Select Departure", `Secondary)
    };
  MaterialUi.(
    <ExpansionPanelSummary expandIcon>
      <div> <Typography color> (s(name)) </Typography> </div>
    </ExpansionPanelSummary>
  );
};

let make = (~config: option(Config.t), ~configs: list(Config.t), ~setConfig, _children) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | SetProviders(providers) => ReasonReact.Update({...state, providers, route: None, direction: None, stop: None})
    | SetProvider(provider) => ReasonReact.Update({...state, provider, route: None, direction: None, stop: None})
    | SetRoutes(routes) => ReasonReact.Update({...state, routes, direction: None, stop: None})
    | SetRoute(route) => ReasonReact.Update({...state, route, direction: None, stop: None})
    | SetDirections(directions) => ReasonReact.Update({...state, directions, stop: None})
    | SetDirection(direction) => ReasonReact.Update({...state, direction, stop: None})
    | SetStops(stops) => ReasonReact.Update({...state, stops})
    | SetStop(stop) =>
      let maybeConfig =
        Config.maybeMake(
          ~routes=state.routes,
          ~directions=state.directions,
          ~stops=state.stops,
          state.route,
          state.direction,
          stop,
        );
      switch (maybeConfig) {
      | Some(config) =>
        ReasonReact.UpdateWithSideEffects({...state, stop, expanded: false}, (_state => setConfig(config)))
      | None => ReasonReact.Update(makeInitialState(true))
      };
    | SetExpanded(expanded) => ReasonReact.Update({...state, expanded})
    },
  initialState: () => makeInitialState(isEmpty(config)),
  willReceiveProps: self => {...self.state, expanded: isEmpty(config)},
  render: self => {
    let setProviders = providers => self.ReasonReact.send(SetProviders(providers));
    let setProvider = provider => self.ReasonReact.send(SetProvider(provider));
    let setRoutes = routes => self.send(SetRoutes(routes));
    let setRoute = route => self.send(SetRoute(route));
    let setDirections = directions => self.send(SetDirections(directions));
    let setDirection = direction => self.send(SetDirection(direction));
    let setStops = stops => self.send(SetStops(stops));
    let setStop = stop => self.send(SetStop(stop));
    let toggleExpanded = (_f, _e) => self.send(SetExpanded(! self.state.expanded));
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
          switch (config) {
          | Some(c) => `String(c.id)
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
    let panelSummary = makePanelSummary(config);
    MaterialUi.(
      <ExpansionPanel expanded=self.state.expanded onChange=toggleExpanded>
        panelSummary
        <ExpansionPanelDetails>
          <Grid container=true>
            <Grid item=true xs=V12> configSelect </Grid>
            <Grid item=true xs=V12>
              <Typography variant=`H6> (s("Select New Departure")) </Typography>
              <ProviderSelect provider=self.state.provider providers=self.state.providers setProviders setProvider />
              <RouteSelect
                route=self.state.route
                routes=self.state.routes
                provider=self.state.provider
                setRoutes
                setRoute
              />
              <DirectionSelect
                direction=self.state.direction
                directions=self.state.directions
                route=self.state.route
                setDirections
                setDirection
              />
              <StopSelect
                stop=self.state.stop
                stops=self.state.stops
                route=self.state.route
                direction=self.state.direction
                setStops
                setStop
              />
            </Grid>
          </Grid>
        </ExpansionPanelDetails>
      </ExpansionPanel>
    );
  },
};
