open Belt;

open Belt.Result;

type state = {
  route: option(Route.t),
  directions: list(Direction.t),
};

type action =
  | LoadDirections(Route.t)
  | LoadedDirections(list(Direction.t))
  | LoadDirectionsFailed(string);

let component = ReasonReact.reducerComponent("DirectionSelect");

let s = ReasonReact.string;

let menuItems = directions => {
  let emptyOption = <MaterialUi.MenuItem key="" value=(`String("")) disabled=true />;
  let directionOptions =
    List.map(directions, (direction: Direction.t) =>
      <MaterialUi.MenuItem key=direction.id value=(`String(direction.id))>
        (s(Util.capitalize(direction.name)))
      </MaterialUi.MenuItem>
    );
  List.add(directionOptions, emptyOption);
};

let nativeMenuItems = directions => {
  let emptyOption = <option key="" value="" disabled=true />;
  let directionOptions =
    List.map(directions, (direction: Direction.t) =>
      <option key=direction.id value=direction.id> (s(Util.capitalize(direction.name))) </option>
    );
  List.add(directionOptions, emptyOption);
};

let make =
    (
      ~direction: option(Direction.t),
      ~directions: list(Direction.t),
      ~route: option(Route.t),
      ~setDirections,
      ~setDirection,
      _childern,
    ) => {
  ...component,
  initialState: () => {route, directions},
  reducer: (action, state) =>
    switch (action) {
    | LoadDirections(route) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, route: Some(route)},
        (
          self =>
            Js.Promise.(
              ApiUri.loadDirections(route.id)
              |> then_(result =>
                   switch (result) {
                   | Ok(directions) => resolve(self.send(LoadedDirections(directions)))
                   | Error(err) => resolve(self.send(LoadDirectionsFailed(err)))
                   }
                 )
              |> ignore
            )
        ),
      )
    | LoadedDirections(directions) =>
      ReasonReact.UpdateWithSideEffects({...state, directions}, (_self => setDirections(directions)))
    | LoadDirectionsFailed(err) => ReasonReact.SideEffects((_self => Js.log("Error loading directions: " ++ err)))
    },
  didMount: self =>
    switch (route) {
    | Some(r) => self.send(LoadDirections(r))
    | None => ()
    },
  willReceiveProps: _self => {route, directions},
  didUpdate: ({oldSelf, newSelf}) =>
    switch (oldSelf.state.route, newSelf.state.route) {
    | (Some(oldRoute), Some(newRoute)) =>
      if (oldRoute.id != newRoute.id) {
        newSelf.send(LoadDirections(newRoute));
      }
    | (None, Some(newRoute)) => newSelf.send(LoadDirections(newRoute))
    | (Some(_oldRoute), None) => ()
    | (None, None) => ()
    },
  render: self => {
    let directionChange = (evt, _el) => {
      let directionId = ReactEvent.Form.target(evt)##value;
      let direction = List.getBy(self.state.directions, direction => direction.id == directionId);
      setDirection(direction);
    };
    let value =
      switch (direction) {
      | Some(direction) => `String(direction.id)
      | None => `String("")
      };
    let select =
      MaterialUi.(
        if (Util.isMobile()) {
          <Select native=true value onChange=directionChange> (nativeMenuItems(self.state.directions)) </Select>;
        } else {
          <Select native=false value onChange=directionChange> (menuItems(self.state.directions)) </Select>;
        }
      );
    <form autoComplete="off">
      MaterialUi.(<FormControl fullWidth=true> <InputLabel> (s("Direction")) </InputLabel> select </FormControl>)
    </form>;
  },
};
