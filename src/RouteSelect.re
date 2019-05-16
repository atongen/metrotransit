open Belt;

open Belt.Result;

type state = {
  routes: list(Route.t)
};

type action =
  | LoadRoutes
  | LoadedRoutes(list(Route.t))
  | LoadRoutesFailed(string);

let component = ReasonReact.reducerComponent("RouteSelect");

let s = ReasonReact.string;

let filterRoutes = (routes, maybeProvider: option(Provider.t)) =>
  switch (maybeProvider) {
  | Some(p) => List.keep(routes, (route: Route.t) => route.providerId == p.id)
  | None => routes
  };

let menuItems = (routes, provider) => {
  let emptyOption = <MaterialUi.MenuItem key="" value=(`String("")) disabled=true />;
  let myRoutes = filterRoutes(routes, provider);
  let routeOptions =
    List.map(myRoutes, (route: Route.t) =>
      <MaterialUi.MenuItem key=route.id value=(`String(route.id))> (s(route.name)) </MaterialUi.MenuItem>
    );
  List.add(routeOptions, emptyOption);
};

let nativeMenuItems = (routes, provider) => {
  let emptyOption = <option key="" value="" disabled=true />;
  let myRoutes = filterRoutes(routes, provider);
  let routeOptions =
    List.map(myRoutes, (route: Route.t) => <option key=route.id value=route.id> (s(route.name)) </option>);
  List.add(routeOptions, emptyOption);
};

/* HERE */
let make =
    (
      ~route: option(Route.t),
      ~routes: list(Route.t),
      ~provider: option(Provider.t),
      ~setRoutes,
      ~setRoute,
      _childern,
    ) => {
  ...component,
  initialState: () => {routes},
  reducer: (action, _state) =>
    switch (action) {
    | LoadRoutes =>
      ReasonReact.SideEffects(
        (
          self =>
            Js.Promise.(
              ApiUri.loadRoutes()
              |> then_(result =>
                   switch (result) {
                   | Ok(routes) => resolve(self.send(LoadedRoutes(routes)))
                   | Error(err) => resolve(self.send(LoadRoutesFailed(err)))
                   }
                 )
              |> ignore
            )
        ),
      )
    | LoadedRoutes(routes) => ReasonReact.UpdateWithSideEffects({routes}, (_self => setRoutes(routes)))
    | LoadRoutesFailed(err) => ReasonReact.SideEffects(_self => Js.log("Error loading routes: " ++ err))
    },
  didMount: self => self.send(LoadRoutes),
  willReceiveProps: _self => {routes},
  render: self => {
      let routeChange = (evt, _el) => {
        let routeId = ReactEvent.Form.target(evt)##value;
        let route = List.getBy(self.state.routes, route => route.id == routeId);
        setRoute(route);
      };
      let value =
        switch (route) {
        | Some(route) => `String(route.id)
        | None => `String("")
        };
      let select =
        MaterialUi.(
          if (Util.isMobile()) {
            <Select native=true value onChange=routeChange> (nativeMenuItems(self.state.routes, provider)) </Select>;
          } else {
            <Select native=false value onChange=routeChange> (menuItems(self.state.routes, provider)) </Select>;
          }
        );
      <form autoComplete="off">
        MaterialUi.(
          <FormControl fullWidth=true> <InputLabel> (ReasonReact.string("Route")) </InputLabel> select </FormControl>
        )
      </form>
    }
};
