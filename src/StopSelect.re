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

let s = ReasonReact.string;

let menuItems = stops =>
  List.map(stops, (stop: Stop.t) =>
    <MaterialUi.MenuItem key=stop.id value=(`String(stop.id))> (s(stop.name)) </MaterialUi.MenuItem>
  );

let nativeMenuItems = stops =>
  List.map(stops, (stop: Stop.t) => <option key=stop.id value=stop.id> (s(stop.name)) </option>);

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
              ApiUri.loadStops(route.id, direction.id)
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
      let select =
        MaterialUi.(
          if (Util.isMobile()) {
            <Select native=true value onChange=stopChange style> (nativeMenuItems(stops)) </Select>;
          } else {
            <Select native=false value onChange=stopChange style> (menuItems(stops)) </Select>;
          }
        );
      <form autoComplete="off">
        MaterialUi.(
          <FormControl fullWidth=true> <InputLabel> (s("Stop")) </InputLabel> select </FormControl>
        )
      </form>;
    },
};
