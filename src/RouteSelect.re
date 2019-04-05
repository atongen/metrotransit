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

let menuItems = (routes, provider) => {
  let myRoutes =
    switch (provider) {
    | Some(p) => List.keep(routes, (route: Route.t) => route.providerId == p)
    | None => routes
    };
  List.map(myRoutes, (route: Route.t) =>
    <MaterialUi.MenuItem key=route.id value=(`String(route.id))>
      (ReasonReact.string(route.name))
    </MaterialUi.MenuItem>
  );
};

let make = (~selected, ~provider, ~setRoute, _childern) => {
  let routeChange = (evt, _el) => {
    let route = ReactEvent.Form.target(evt)##value;
    setRoute(route);
  };
  {
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
      | Loading => <div> (ReasonReact.string("Loading routes...")) </div>
      | Failure(err) => <div> (ReasonReact.string("Something went wrong: " ++ err)) </div>
      | Success(routes) =>
        let selectedStr =
          switch (selected) {
          | Some(s) => s
          | None => ""
          };
        MaterialUi.(
          <form autoComplete="off">
            <FormControl>
              <InputLabel> (ReasonReact.string("Route")) </InputLabel>
              <Select value=(`String(selectedStr)) onChange=routeChange> (menuItems(routes, provider)) </Select>
            </FormControl>
          </form>
        );
      },
  };
};
