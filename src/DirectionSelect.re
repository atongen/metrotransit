open Belt;

open Belt.Result;

type state =
  | NotAsked
  | Loading
  | Failure(string)
  | Success(list(Direction.t));

type action =
  | LoadDirections
  | LoadedDirections(list(Direction.t))
  | LoadDirectionsFailed(string);

let component = ReasonReact.reducerComponent("DirectionSelect");

let s = ReasonReact.string;

let menuItems = directions =>
  List.map(directions, (direction: Direction.t) =>
    <MaterialUi.MenuItem key=direction.id value=(`String(direction.id))>
      (s(Util.capitalize(direction.name)))
    </MaterialUi.MenuItem>
  );

let nativeMenuItems = directions =>
  List.map(directions, (direction: Direction.t) =>
    <option key=direction.id value=direction.id> (s(Util.capitalize(direction.name))) </option>
  );

let make = (~selected: option(Direction.t), ~route: Route.t, ~setDirection, _childern) => {
  ...component,
  initialState: () => NotAsked,
  reducer: (action, _state) =>
    switch (action) {
    | LoadDirections =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
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
    | LoadedDirections(directions) => ReasonReact.Update(Success(directions))
    | LoadDirectionsFailed(err) => ReasonReact.Update(Failure(err))
    },
  didMount: self => self.send(LoadDirections),
  render: self =>
    switch (self.state) {
    | NotAsked => ReasonReact.null
    | Loading => <div> (ReasonReact.string("Loading directions...")) </div>
    | Failure(err) => <div> (ReasonReact.string("Something went wrong: " ++ err)) </div>
    | Success(directions) =>
      let directionChange = (evt, _el) => {
        let directionId = ReactEvent.Form.target(evt)##value;
        let direction = List.getBy(directions, direction => direction.id == directionId);
        setDirection(direction);
      };
      let value =
        switch (selected) {
        | Some(direction) => `String(direction.id)
        | None => `String("")
        };
      let select =
        MaterialUi.(
          if (Util.isMobile()) {
            <Select native=true value onChange=directionChange> (nativeMenuItems(directions)) </Select>;
          } else {
            <Select native=false value onChange=directionChange> (menuItems(directions)) </Select>;
          }
        );
      <form autoComplete="off">
        MaterialUi.(<FormControl fullWidth=true> <InputLabel> (s("Direction")) </InputLabel> select </FormControl>)
      </form>;
    },
};
