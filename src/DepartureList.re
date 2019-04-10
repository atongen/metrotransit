open Belt;

open Belt.Result;

type requestState =
  | NotAsked
  | Loading
  | Failure(string)
  | Success;

type state = {
  requestState,
  timerId: ref(option(Js.Global.intervalId)),
  departures: list(Departure.t),
  lastUpdated: Js.Date.t,
};

type action =
  | LoadDepartures
  | LoadedDepartures(list(Departure.t))
  | LoadDeparturesFailed(string);

let intervalSeconds = 30;

let component = ReasonReact.reducerComponent("DepartureList");

let make = (~config: Config.t, _childern) => {
  ...component,
  initialState: () => {requestState: NotAsked, timerId: ref(None), departures: [], lastUpdated: Js.Date.make()},
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
    | LoadedDepartures(departures) =>
      ReasonReact.Update({...state, requestState: Success, departures, lastUpdated: Js.Date.make()})
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
    | Failure(err) => <div> (ReasonReact.string("Something went wrong: " ++ err)) </div>
    | Loading
    | Success =>
      <div>
        <p> (ReasonReact.string("Last updated: " ++ Js.Date.toLocaleString(self.state.lastUpdated))) </p>
        <ul>
          (
            ReasonReact.array(
              Array.map(List.toArray(self.state.departures), departure =>
                <li key=(Departure.id(departure))> (ReasonReact.string(Departure.toString(departure))) </li>
              ),
            )
          )
        </ul>
      </div>
    },
};
