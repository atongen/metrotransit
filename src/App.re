type action =
  | SetProvider(Provider.providerId)
  | SetRoute(Route.routeId);

type state = {
    provider: option(Provider.providerId),
    route: option(Route.routeId),
};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | SetProvider(provider) => ReasonReact.Update({...state, provider: Some(provider)})
    | SetRoute(route) => ReasonReact.Update({...state, route: Some(route)})
    },
  initialState: () => {
      provider: None,
      route: None,
  },
  render: self => {
    let setProvider = provider => self.ReasonReact.send(SetProvider(provider));
    let setRoute = route => self.ReasonReact.send(SetRoute(route));
    <div>
        <ProviderSelect selected=self.state.provider setProvider />
        <RouteSelect selected=self.state.route provider=self.state.provider setRoute />
    </div>
  },
};
