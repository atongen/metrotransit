open Belt;

open Belt.Result;

let directionUrl = (routeId: Route.routeId) =>
  Printf.sprintf("https://svc.metrotransit.org/NexTrip/Directions/%s?format=json", routeId);

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
      (ReasonReact.string(direction.name))
    </MaterialUi.MenuItem>
  );

let make = (~selected, ~route, ~setDirection, _childern) => {
  let directionChange = (evt, _el) => {
    let direction = ReactEvent.Form.target(evt)##value;
    setDirection(direction);
  };
  let url = directionUrl(route);
  {
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
                Util.getCachedUrl(url)
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
        let selectedStr =
          switch (selected) {
          | Some(s) => s
          | None => ""
          };
        MaterialUi.(
          <form autoComplete="off">
            <FormControl>
              <InputLabel> (ReasonReact.string("Direction")) </InputLabel>
              <Select value=(`String(selectedStr)) onChange=directionChange> (menuItems(directions)) </Select>
            </FormControl>
          </form>
        );
      },
  };
};
