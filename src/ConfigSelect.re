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

let makeState = (~providers=[], ~provider=None, maybeConfig: option(Config.t)) =>
  switch (maybeConfig) {
  | Some(config) => {
      providers,
      provider,
      routes: config.routes,
      route: Some(config.route),
      directions: config.directions,
      direction: Some(config.direction),
      stops: config.stops,
      stop: Some(config.stop),
      expanded: false,
    }
  | None => {
      providers,
      provider,
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
    | SetProvider(provider) => switch(provider, state.route) {
      | (Some(provider), Some(route)) => if (provider.id == route.providerId) {
        ReasonReact.Update({...state, provider: Some(provider)})
      } else {
        ReasonReact.Update({...state, provider: Some(provider), route: None, direction: None, stop: None})
      }
      | _e => ReasonReact.Update({...state, provider, route: None, direction: None, stop: None})
    }
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
    | SetRoute(route) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, route, direction: None, stop: None},
        (
          self =>
            switch (self.state.route) {
            | Some(route) => self.send(LoadDirections(route))
            | None => ()
            }
        ),
      )
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
    | SetDirection(direction) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, direction, stop: None},
        (
          self =>
            switch (self.state.route, self.state.direction) {
            | (Some(route), Some(direction)) => self.send(LoadStops(route, direction))
            | _e => ()
            }
        ),
      )
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
      | None => ReasonReact.Update({...state, stop: None, expanded: true})
      };
    | SetExpanded(expanded) => ReasonReact.Update({...state, expanded})
    },
  initialState: () => makeState(config),
  didMount: self => {
    self.send(LoadProviders);
    self.send(LoadRoutes);
  },
  willReceiveProps: self => makeState(~providers=self.state.providers, ~provider=self.state.provider, config),
  render: self => {
    let providerOnChange = (evt, _el) => {
      let providerId = ReactEvent.Form.target(evt)##value;
      let provider = List.getBy(self.state.providers, provider => provider.id == providerId);
      self.ReasonReact.send(SetProvider(provider));
    };
    let routeOnChange = (evt, _el) => {
      let routeId = ReactEvent.Form.target(evt)##value;
      let route = List.getBy(self.state.routes, route => route.id == routeId);
      self.ReasonReact.send(SetRoute(route));
    };
    let directionOnChange = (evt, _el) => {
      let directionId = ReactEvent.Form.target(evt)##value;
      let direction = List.getBy(self.state.directions, direction => direction.id == directionId);
      self.ReasonReact.send(SetDirection(direction));
    };
    let stopOnChange = (evt, _el) => {
      let stopId = ReactEvent.Form.target(evt)##value;
      let stop = List.getBy(self.state.stops, stop => stop.id == stopId);
      self.ReasonReact.send(SetStop(stop));
    };
    let toggleExpanded = (_f, _e) => self.send(SetExpanded(! self.state.expanded));
    let configOnChange = (evt, _el) => {
      let configId = ReactEvent.Form.target(evt)##value;
      let maybeConfig = List.getBy(configs, config => config.id == configId);
      switch (maybeConfig) {
      | Some(config) =>
        setConfig(config);
        self.ReasonReact.send(SetExpanded(false));
      | None => ()
      };
    };
    let panelSummary = makePanelSummary(config);
    let filteredRoutes =
      switch (self.state.provider) {
      | Some(provider) => List.keep(self.state.routes, (route: Route.t) => route.providerId == provider.id)
      | None => self.state.routes
      };
    MaterialUi.(
      <ExpansionPanel expanded=self.state.expanded onChange=toggleExpanded>
        panelSummary
        <ExpansionPanelDetails>
          <Grid container=true>
            <Grid item=true xs=V12>
              <Selection
                label="Previously Selected Departures"
                item=config
                items=configs
                toSelectOption=Config.toSelectOption
                onChange=configOnChange
              />
            </Grid>
            <Grid item=true xs=V12>
              <Typography variant=`H6> (s("Select New Departure")) </Typography>
              <Selection
                emptyDisplayName="All"
                emptyDisabled=false
                label="Provider"
                item=self.state.provider
                items=self.state.providers
                toSelectOption=Provider.toSelectOption
                onChange=providerOnChange
              />
              <Selection
                label="Route"
                item=self.state.route
                items=filteredRoutes
                toSelectOption=Route.toSelectOption
                onChange=routeOnChange
              />
              <Selection
                label="Direction"
                item=self.state.direction
                items=self.state.directions
                toSelectOption=Direction.toSelectOption
                onChange=directionOnChange
              />
              <Selection
                label="Stop"
                item=self.state.stop
                items=self.state.stops
                toSelectOption=Stop.toSelectOption
                onChange=stopOnChange
              />
            </Grid>
          </Grid>
        </ExpansionPanelDetails>
      </ExpansionPanel>
    );
  },
};
