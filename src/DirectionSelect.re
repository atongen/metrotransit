open Belt;

let component = ReasonReact.statelessComponent("DirectionSelect");

let s = ReasonReact.string;

let menuItems = directions => {
  let emptyOption = <MaterialUi.MenuItem key="" value=(`String("")) disabled=true />;
  let directionOptions =
    List.map(directions, (direction: Direction.t) =>
      <MaterialUi.MenuItem key=direction.id value=(`String(direction.id))>
        (s(Util.capitalize(direction.name)))
      </MaterialUi.MenuItem>
    );
  List.add(directionOptions, emptyOption);
};

let nativeMenuItems = directions => {
  let emptyOption = <option key="" value="" disabled=true />;
  let directionOptions =
    List.map(directions, (direction: Direction.t) =>
      <option key=direction.id value=direction.id> (s(Util.capitalize(direction.name))) </option>
    );
  List.add(directionOptions, emptyOption);
};

let make = (~direction: option(Direction.t), ~directions: list(Direction.t), ~setDirection, _childern) => {
  ...component,
  render: _self => {
    let directionChange = (evt, _el) => {
      let directionId = ReactEvent.Form.target(evt)##value;
      let direction = List.getBy(directions, direction => direction.id == directionId);
      setDirection(direction);
    };
    let value =
      switch (direction) {
      | Some(direction) => `String(direction.id)
      | None => `String("")
      };
    let select =
      MaterialUi.(
        if (Util.isMobile()) {
          <Select native=true value onChange=directionChange> (nativeMenuItems(directions)) </Select>;
        } else {
          <Select native=false value onChange=directionChange> (menuItems(directions)) </Select>;
        }
      );
    <form autoComplete="off">
      MaterialUi.(<FormControl fullWidth=true> <InputLabel> (s("Direction")) </InputLabel> select </FormControl>)
    </form>;
  },
};
