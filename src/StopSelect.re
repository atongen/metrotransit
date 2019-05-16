open Belt;

open Belt.Result;

type state =
  | NotAsked
  | Loading
  | Failure(string)
  | Success(list(Stop.t));

type action =
  | LoadStops(Route.routeId, Direction.directionId)
  | LoadedStops(list(Stop.t))
  | LoadStopsFailed(string);

let component = ReasonReact.reducerComponent("StopSelect");

let s = ReasonReact.string;

let menuItems = stops => {
  let emptyOption = <MaterialUi.MenuItem key="" value=(`String("")) disabled=true />;
  let stopOptions =
    List.map(stops, (stop: Stop.t) =>
      <MaterialUi.MenuItem key=stop.id value=(`String(stop.id))> (s(stop.name)) </MaterialUi.MenuItem>
    );
  List.add(stopOptions, emptyOption);
};

let nativeMenuItems = stops => {
  let emptyOption = <option key="" value="" disabled=true />;
  let stopOptions = List.map(stops, (stop: Stop.t) => <option key=stop.id value=stop.id> (s(stop.name)) </option>);
  List.add(stopOptions, emptyOption);
};

let make =
    (
      ~stop: option(Stop.t),
      ~route: option(Route.t),
      ~direction: option(Direction.t),
      ~stops: list(Stop.t),
      ~setStops,
      ~setStop,
      _childern,
    ) => {
  ...component,
  initialState: () => Success(stops),
  reducer: (action, _state) =>
    switch (action) {
    | LoadStops(routeId, directionId) =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self =>
            Js.Promise.(
              ApiUri.loadStops(routeId, directionId)
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
    | LoadedStops(stops) => ReasonReact.UpdateWithSideEffects(Success(stops), (_self => setStops(stops)))
    | LoadStopsFailed(err) => ReasonReact.Update(Failure(err))
    },
  didMount: self =>
    switch (route, direction) {
    | (Some(r), Some(d)) => self.send(LoadStops(r.id, d.id))
    | _ => ()
    },
  render: self =>
    switch (self.state) {
    | NotAsked => ReasonReact.null
    | Loading => Util.typography("Loading stops...")
    | Failure(err) => Util.typography("Something went wrong: " ++ err)
    | Success(stops) =>
      let stopChange = (evt, _el) => {
        let stopId = ReactEvent.Form.target(evt)##value;
        let stop = List.getBy(stops, stop => stop.id == stopId);
        setStop(stop);
      };
      let value =
        switch (stop) {
        | Some(stop) => `String(stop.id)
        | None => `String("")
        };
      let select =
        MaterialUi.(
          if (Util.isMobile()) {
            <Select native=true value onChange=stopChange> (nativeMenuItems(stops)) </Select>;
          } else {
            <Select native=false value onChange=stopChange> (menuItems(stops)) </Select>;
          }
        );
      <form autoComplete="off">
        MaterialUi.(<FormControl fullWidth=true> <InputLabel> (s("Stop")) </InputLabel> select </FormControl>)
      </form>;
    },
};
