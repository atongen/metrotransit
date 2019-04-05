type action =
  | SetProvider(Provider.providerId)
  | SetRoute(Route.routeId)
  | SetDirection(Direction.directionId);

type state = {
  provider: option(Provider.providerId),
  route: option(Route.routeId),
  direction: option(Direction.directionId),
};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | SetProvider(provider) => ReasonReact.Update({provider: Some(provider), route: None, direction: None})
    | SetRoute(route) => ReasonReact.Update({...state, route: Some(route), direction: None})
    | SetDirection(direction) => ReasonReact.Update({...state, direction: Some(direction)})
    },
  initialState: () => {provider: None, route: None, direction: None},
  render: self => {
    let setProvider = provider => self.ReasonReact.send(SetProvider(provider));
    let setRoute = route => self.ReasonReact.send(SetRoute(route));
    let setDirection = direction => self.ReasonReact.send(SetDirection(direction));
    let directionSelect =
      switch (self.state.route) {
      | Some(route) => <DirectionSelect selected=self.state.direction route setDirection />
      | None => ReasonReact.null
      };
    <div>
      <ProviderSelect selected=self.state.provider setProvider />
      <RouteSelect selected=self.state.route provider=self.state.provider setRoute />
      directionSelect
    </div>;
  },
};
