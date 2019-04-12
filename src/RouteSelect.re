open Belt;

open Belt.Result;

type state =
  | NotAsked
  | Loading
  | Failure(string)
  | Success(list(Route.t));

type action =
  | LoadRoutes
  | LoadedRoutes(list(Route.t))
  | LoadRoutesFailed(string);

let component = ReasonReact.reducerComponent("RouteSelect");

let menuItems = (routes, provider: option(Provider.t)) => {
  let myRoutes =
    switch (provider) {
    | Some(p) => List.keep(routes, (route: Route.t) => route.providerId == p.id)
    | None => routes
    };
  List.map(myRoutes, (route: Route.t) =>
    <MaterialUi.MenuItem key=route.id value=(`String(route.id))>
      (ReasonReact.string(route.name))
    </MaterialUi.MenuItem>
  );
};

let make = (~selected: option(Route.t), ~provider: option(Provider.t), ~setRoute, _childern) => {
  ...component,
  initialState: () => NotAsked,
  reducer: (action, _state) =>
    switch (action) {
    | LoadRoutes =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self =>
            Js.Promise.(
              Util.getCachedUrl(ApiUri.(toString(RoutesUri)))
              |> then_(jsonStr => Route.ofJson(jsonStr) |> resolve)
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
    | LoadedRoutes(routes) => ReasonReact.Update(Success(routes))
    | LoadRoutesFailed(err) => ReasonReact.Update(Failure(err))
    },
  didMount: self => self.send(LoadRoutes),
  render: self =>
    switch (self.state) {
    | NotAsked => ReasonReact.null
    | Loading => Util.typography("Loading providers...")
    | Failure(err) => Util.typography("Something went wrong: " ++ err)
    | Success(routes) =>
      let routeChange = (evt, _el) => {
        let routeId = ReactEvent.Form.target(evt)##value;
        let route = List.getBy(routes, route => route.id == routeId);
        setRoute(route);
      };
      let value =
        switch (selected) {
        | Some(route) => `String(route.id)
        | None => `String("")
        };
      <form autoComplete="off">
        MaterialUi.(
          <FormControl fullWidth=true>
            <InputLabel> (ReasonReact.string("Route")) </InputLabel>
            <Select value onChange=routeChange> (menuItems(routes, provider)) </Select>
          </FormControl>
        )
      </form>;
    },
};
