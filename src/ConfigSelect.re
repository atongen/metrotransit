open Belt;

open Belt.Result;

type action =
  | LoadProviders
  | SetProviders(list(Provider.t))
  | SetProvider(option(Provider.t))
  | LoadRoutes
  | SetRoutes(list(Route.t))
  | SetRoute(option(Route.t))
  | LoadDirections(Route.t)
  | SetDirections(list(Direction.t))
  | SetDirection(option(Direction.t))
  | LoadStops(Route.t, Direction.t)
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

let makeState = (maybeConfig: option(Config.t)) =>
  switch (maybeConfig) {
  | Some(config) => {
      providers: [],
      provider: None,
      routes: config.routes,
      route: Some(config.route),
      directions: config.directions,
      direction: Some(config.direction),
      stops: config.stops,
      stop: Some(config.stop),
      expanded: false,
    }
  | None => {
      providers: [],
      provider: None,
      routes: [],
      route: None,
      directions: [],
      direction: None,
      stops: [],
      stop: None,
      expanded: true,
    }
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
    | LoadProviders =>
      ReasonReact.SideEffects(
        (
          self =>
            Js.Promise.(
              ApiUri.loadProviders()
              |> then_(result =>
                   switch (result) {
                   | Ok(providers) => resolve(self.send(SetProviders(providers)))
                   | Error(err) => resolve(Js.log("Loading providers failed: " ++ err))
                   }
                 )
              |> ignore
            )
        ),
      )
    | SetProviders(providers) => ReasonReact.Update({...state, providers})
    | SetProvider(provider) => ReasonReact.Update({...state, provider, route: None, direction: None, stop: None})
    | LoadRoutes =>
      ReasonReact.SideEffects(
        (
          self =>
            Js.Promise.(
              ApiUri.loadRoutes()
              |> then_(result =>
                   switch (result) {
                   | Ok(routes) => resolve(self.send(SetRoutes(routes)))
                   | Error(err) => resolve(Js.log("Loading routes failed: " ++ err))
                   }
                 )
              |> ignore
            )
        ),
      )
    | SetRoutes(routes) => ReasonReact.Update({...state, routes})
    | SetRoute(route) => ReasonReact.UpdateWithSideEffects({...state, route, direction: None, stop: None}, self => switch(self.state.route) {
      | Some(route) => self.send(LoadDirections(route))
      | None => ()
    })
    | LoadDirections(route) =>
      ReasonReact.SideEffects(
        (
          self =>
            Js.Promise.(
              ApiUri.loadDirections(route.id)
              |> then_(result =>
                   switch (result) {
                   | Ok(directions) => resolve(self.send(SetDirections(directions)))
                   | Error(err) => resolve(Js.log("Loading directions failed: " ++ err))
                   }
                 )
              |> ignore
            )
        ),
      )
    | SetDirections(directions) => ReasonReact.Update({...state, directions})
    | SetDirection(direction) => ReasonReact.UpdateWithSideEffects({...state, direction, stop: None}, self => switch(self.state.route, self.state.direction) {
      | (Some(route), Some(direction)) => self.send(LoadStops(route, direction))
      | _e => ()
    })
    | LoadStops(route, direction) =>
      ReasonReact.SideEffects(
        (
          self =>
            Js.Promise.(
              ApiUri.loadStops(route.id, direction.id)
              |> then_(result =>
                   switch (result) {
                   | Ok(stops) => resolve(self.send(SetStops(stops)))
                   | Error(err) => resolve(Js.log("Loading stops failed: " ++ err))
                   }
                 )
              |> ignore
            )
        ),
      )
    | SetStops(stops) => ReasonReact.UpdateWithSideEffects({...state, stops}, self => ())
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
      | None => ReasonReact.Update({...state, stop: None, expanded: true})
      };
    | SetExpanded(expanded) => ReasonReact.Update({...state, expanded})
    },
  initialState: () => makeState(config),
  didMount: self => {
      self.send(LoadProviders);
      self.send(LoadRoutes);
  },
  willReceiveProps: _self => makeState(config),
  render: self => {
    let setProvider = provider => self.ReasonReact.send(SetProvider(provider));
    let setRoute = route => self.send(SetRoute(route));
    let setDirection = direction => self.send(SetDirection(direction));
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
              <ProviderSelect provider=self.state.provider providers=self.state.providers setProvider />
              <RouteSelect route=self.state.route routes=self.state.routes provider=self.state.provider setRoute />
              <DirectionSelect direction=self.state.direction directions=self.state.directions setDirection />
              <StopSelect stop=self.state.stop stops=self.state.stops setStop />
            </Grid>
          </Grid>
        </ExpansionPanelDetails>
      </ExpansionPanel>
    );
  },
};
