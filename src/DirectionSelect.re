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

let menuItems = directions =>
  List.map(directions, (direction: Direction.t) =>
    <MaterialUi.MenuItem key=direction.id value=(`String(direction.id))>
      (ReasonReact.string(Util.capitalize(direction.name)))
    </MaterialUi.MenuItem>
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
              Util.getCachedUrl(ApiUri.(toString(DirectionsUri(route.id))))
              |> then_(jsonStr => Direction.ofJson(jsonStr) |> resolve)
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
      let style = ReactDOMRe.Style.make(~overflow="hidden", ());
      <form autoComplete="off">
        MaterialUi.(
          <FormControl fullWidth=true>
            <InputLabel> (ReasonReact.string("Direction")) </InputLabel>
            <Select value onChange=directionChange style> (menuItems(directions)) </Select>
          </FormControl>
        )
      </form>;
    },
};
