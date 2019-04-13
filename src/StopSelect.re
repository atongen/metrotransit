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

let make = (~selected: option(Stop.t), ~route: Route.t, ~direction: Direction.t, ~setStop, _childern) => {
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
              Util.getCachedUrl(ApiUri.(toString(StopsUri(route.id, direction.id))))
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
    | Loading => Util.typography("Loading providers...")
    | Failure(err) => Util.typography("Something went wrong: " ++ err)
    | Success(stops) =>
      let stopChange = (evt, _el) => {
        let stopId = ReactEvent.Form.target(evt)##value;
        let stop = List.getBy(stops, stop => stop.id == stopId);
        setStop(stop);
      };
      let value =
        switch (selected) {
        | Some(stop) => `String(stop.id)
        | None => `String("")
        };
      let style = ReactDOMRe.Style.make(~overflow="hidden", ());
      <form autoComplete="off">
        MaterialUi.(
          <FormControl fullWidth=true>
            <InputLabel> (ReasonReact.string("Stop")) </InputLabel>
            <Select value onChange=stopChange style> (menuItems(stops)) </Select>
          </FormControl>
        )
      </form>;
    },
};
