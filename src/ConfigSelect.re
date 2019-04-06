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

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | SetProvider(provider) => ReasonReact.Update({provider, route: None, direction: None, stop: None})
    | SetRoute(route) => ReasonReact.Update({...state, route, direction: None, stop: None})
    | SetDirection(direction) => ReasonReact.Update({...state, direction, stop: None})
    | SetStop(stop) => ReasonReact.Update({...state, stop})
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
      | (Some(route), Some(direction)) => <StopSelect selected=self.state.stop route direction setStop />
      | _ => ReasonReact.null
      };
    let expandIcon = MaterialUi.(<Icon> (ReasonReact.string("expand_more")) </Icon>);
    MaterialUi.(
      <ExpansionPanel>
        <ExpansionPanelSummary expandIcon>
          <div> <Typography> (ReasonReact.string("Config")) </Typography> </div>
          <div> <Typography> (ReasonReact.string("xerces")) </Typography> </div>
        </ExpansionPanelSummary>
        <ExpansionPanelDetails>
          <div>
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
