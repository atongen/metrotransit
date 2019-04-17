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
  config: Config.t,
  departures: list(Departure.t),
  lastUpdated: option(Js.Date.t),
  currentTime: Js.Date.t,
};

type action =
  | Tick
  | LoadedDepartures(list(Departure.t))
  | LoadDeparturesFailed(string);

let updateIntervalSeconds = 30;

let tickIntervalSeconds = 1;

let s = ReasonReact.string;

let component = ReasonReact.reducerComponent("DepartureList");

let renderDepartureLi = (departure: Departure.t, asOf) => {
  /*Js.log(Departure.toString(departure));*/
  let timeStr =
    switch (Departure.parseLocalTime(departure.time)) {
    | Some(t) => Js.Date.toLocaleTimeString(t) ++ ", " ++ Util.distanceOfTimeInWords(asOf, t)
    | None => departure.text
    };
  let (icon, color) =
    if (departure.actual) {
      ("alarm", `Secondary);
    } else {
      ("schedule", `Inherit);
    };
  let key = Departure.id(departure);
  MaterialUi.(
    <ListItem key>
      <Avatar> <Icon color> (s(icon)) </Icon> </Avatar>
      <ListItemText primary=(s(departure.name)) secondary=(s(timeStr)) />
    </ListItem>
  );
};

let renderDepartureList = (departures, asOf) =>
  if (List.length(departures) == 0) {
    MaterialUi.(<Typography variant=`Body1> (s("No departures found!")) </Typography>);
  } else {
    <MaterialUi.List>
      (List.map(departures, departure => renderDepartureLi(departure, asOf)) |> List.toArray |> ReasonReact.array)
    </MaterialUi.List>;
  };

let makeLastUpdatedStr = (lastUpdated, currentTime) =>
  switch (lastUpdated) {
  | Some(t) =>
    "Last updated: " ++ Js.Date.toLocaleTimeString(t) ++ ", " ++ Util.distanceOfTimeInWords(currentTime, t)
  | None => "Current time: " ++ Js.Date.toLocaleTimeString(currentTime)
  };

let diffSec = (d0, d1) => {
  let s = d => Js.Date.valueOf(d) /. 1000.0;
  s(d0) -. s(d1);
};

let shouldUpdate = (state, now) =>
  switch (state.requestState, state.lastUpdated) {
  | (NotAsked, _) => true
  | (_, None) => true
  | (_, Some(t)) => diffSec(now, t) >= float_of_int(updateIntervalSeconds)
  };

let make = (~config: Config.t, _childern) => {
  ...component,
  initialState: () => {
    requestState: NotAsked,
    timerId: ref(None),
    departures: [],
    lastUpdated: None,
    currentTime: Js.Date.make(),
    config,
  },
  reducer: (action, state) =>
    switch (action) {
    | Tick =>
      let now = Js.Date.make();
      if (shouldUpdate(state, now)) {
        ReasonReact.UpdateWithSideEffects(
          {...state, currentTime: now, requestState: Loading},
          (
            self => {
              let c = self.state.config;
              Js.Promise.(
                ApiUri.loadTimepointDepartures(c.route, c.direction, c.stop)
                |> then_(result =>
                     switch (result) {
                     | Ok(departures) => resolve(self.send(LoadedDepartures(departures)))
                     | Error(err) => resolve(self.send(LoadDeparturesFailed(err)))
                     }
                   )
                |> ignore
              );
            }
          ),
        );
      } else {
        ReasonReact.Update({...state, currentTime: now});
      };
    | LoadedDepartures(departures) =>
      ReasonReact.Update({
        ...state,
        requestState: Success,
        departures,
        lastUpdated: Some(Js.Date.make()),
        currentTime: Js.Date.make(),
      })
    | LoadDeparturesFailed(err) =>
      ReasonReact.Update({...state, lastUpdated: None, currentTime: Js.Date.make(), requestState: Failure(err)})
    },
  didMount: self => {
    self.state.timerId := Some(Js.Global.setInterval(() => self.send(Tick), tickIntervalSeconds * 1000));
    self.send(Tick);
  },
  willReceiveProps: self => {...self.state, lastUpdated: None, currentTime: Js.Date.make(), config},
  willUnmount: self =>
    switch (self.state.timerId) {
    | {contents: Some(id)} => Js.Global.clearInterval(id)
    | _ => ()
    },
  render: self =>
    switch (self.state.requestState) {
    | NotAsked => ReasonReact.null
    | Failure(err) =>
      <div>
        <MaterialUi.Typography variant=`Body1> (s("Something went wrong: " ++ err)) </MaterialUi.Typography>
      </div>
    | Loading
    | Success =>
      let departureList = renderDepartureList(self.state.departures, self.state.currentTime);
      let lastUpdated = makeLastUpdatedStr(self.state.lastUpdated, self.state.currentTime);
      <div> <MaterialUi.Typography variant=`Body1> (s(lastUpdated)) </MaterialUi.Typography> departureList </div>;
    },
};
