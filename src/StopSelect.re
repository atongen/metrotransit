open Belt;

let component = ReasonReact.statelessComponent("StopSelect");

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

let make = (~stop: option(Stop.t), ~stops: list(Stop.t), ~setStop, _childern) => {
  ...component,
  render: _self => {
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
