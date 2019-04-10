open Belt;

open Belt.Result;

type requestState =
  | NotAsked
  | Loading
  | Failure(string)
  | Success(list(Departure.t));

type state = {
  requestState,
  timerId: ref(option(Js.Global.intervalId)),
};

type action =
  | LoadDepartures
  | LoadedDepartures(list(Departure.t))
  | LoadDeparturesFailed(string);

let intervalSeconds = 30;

let component = ReasonReact.reducerComponent("DepartureList");

let make = (~config: Config.t, _childern) => {
  ...component,
  initialState: () => {requestState: NotAsked, timerId: ref(None)},
  reducer: (action, state) =>
    switch (action) {
    | LoadDepartures =>
      ReasonReact.UpdateWithSideEffects(
        {...state, requestState: Loading},
        (
          self =>
            Js.Promise.(
              Util.fetchUrl(ApiUri.(toString(TimepointDeparturesUri(config.route, config.direction, config.stop))))
              |> then_(jsonStr => Departure.ofJson(jsonStr) |> resolve)
              |> then_(result =>
                   switch (result) {
                   | Ok(departures) => resolve(self.send(LoadedDepartures(departures)))
                   | Error(err) => resolve(self.send(LoadDeparturesFailed(err)))
                   }
                 )
              |> ignore
            )
        ),
      )
    | LoadedDepartures(departures) => ReasonReact.Update({...state, requestState: Success(departures)})
    | LoadDeparturesFailed(err) => ReasonReact.Update({...state, requestState: Failure(err)})
    },
  didMount: self => {
    self.state.timerId := Some(Js.Global.setInterval(() => self.send(LoadDepartures), intervalSeconds * 1000));
    self.send(LoadDepartures);
  },
  willUnmount: self =>
    switch (self.state.timerId) {
    | {contents: Some(id)} => Js.Global.clearInterval(id)
    | _ => ()
    },
  render: self =>
    switch (self.state.requestState) {
    | NotAsked => ReasonReact.null
    | Loading => <div> (ReasonReact.string("Loading departures...")) </div>
    | Failure(err) => <div> (ReasonReact.string("Something went wrong: " ++ err)) </div>
    | Success(departures) =>
      <ul>
        (
          ReasonReact.array(
            Array.map(List.toArray(departures), departure =>
              <li key=(Departure.id(departure))> (ReasonReact.string(Departure.toString(departure))) </li>
            ),
          )
        )
      </ul>
    },
};
