open Belt;

open Belt.Result;

type state =
  | NotAsked
  | Loading
  | Failure(string)
  | Success(list(Stop.t));

type action =
  | LoadStops
  | LoadedStops(list(Stop.t))
  | LoadStopsFailed(string);

let component = ReasonReact.reducerComponent("StopSelect");

let menuItems = stops =>
  List.map(stops, (stop: Stop.t) =>
    <MaterialUi.MenuItem key=stop.id value=(`String(stop.id))> (ReasonReact.string(stop.name)) </MaterialUi.MenuItem>
  );

let make = (~selected, ~route, ~direction, ~setStop, _childern) => {
  let stopChange = (evt, _el) => {
    let stop = ReactEvent.Form.target(evt)##value;
    setStop(stop);
  };
  {
    ...component,
    initialState: () => NotAsked,
    reducer: (action, _state) =>
      switch (action) {
      | LoadStops =>
        ReasonReact.UpdateWithSideEffects(
          Loading,
          (
            self =>
              Js.Promise.(
                Util.getCachedUrl(ApiUri.(toString(StopsUri(route, direction))))
                |> then_(jsonStr => Stop.ofJson(jsonStr) |> resolve)
                |> then_(result =>
                     switch (result) {
                     | Ok(stops) => resolve(self.send(LoadedStops(stops)))
                     | Error(err) => resolve(self.send(LoadStopsFailed(err)))
                     }
                   )
                |> ignore
              )
          ),
        )
      | LoadedStops(stops) => ReasonReact.Update(Success(stops))
      | LoadStopsFailed(err) => ReasonReact.Update(Failure(err))
      },
    didMount: self => self.send(LoadStops),
    render: self =>
      switch (self.state) {
      | NotAsked => ReasonReact.null
      | Loading => <div> (ReasonReact.string("Loading stops...")) </div>
      | Failure(err) => <div> (ReasonReact.string("Something went wrong: " ++ err)) </div>
      | Success(stops) =>
        let value =
          switch (selected) {
          | Some(s) => `String(s)
          | None => `String("")
          };
        MaterialUi.(
          <form autoComplete="off">
            <FormControl>
              <InputLabel> (ReasonReact.string("Stop")) </InputLabel>
              <Select value onChange=stopChange> (menuItems(stops)) </Select>
            </FormControl>
          </form>
        );
      },
  };
};
