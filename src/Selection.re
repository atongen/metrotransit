open Belt;

let component = ReasonReact.statelessComponent("Selection");

let s = ReasonReact.string;

let makeEmptyDisplayName = (selections, emptyDisplayName) => if (Util.amIMobile) {
    ""
} else if (List.length(selections) == 0) {
    "None found!"
} else {
    emptyDisplayName;
};

let menuItems = (selections, emptyDisplayName, emptyDisabled) => {
  let emptyOption =
    <MaterialUi.MenuItem key="" value=(`String("")) disabled=emptyDisabled>
        (s(makeEmptyDisplayName(selections, emptyDisplayName)))
    </MaterialUi.MenuItem>;
  let selectOptions =
    List.map(selections, (selection: SelectOption.t) =>
      <MaterialUi.MenuItem key=selection.value value=(`String(selection.value))>
        (s(selection.displayName))
      </MaterialUi.MenuItem>
    );
  List.add(selectOptions, emptyOption);
};

let nativeMenuItems = (selections, emptyDisplayName, emptyDisabled) => {
  let emptyOption = <option key="" value="" disabled=emptyDisabled> (s(makeEmptyDisplayName(selections, emptyDisplayName))) </option>;
  let selectOptions =
    List.map(selections, (selection: SelectOption.t) =>
      <option key=selection.value value=selection.value> (s(selection.displayName)) </option>
    );
  List.add(selectOptions, emptyOption);
};

let toMaybeSelectOption = (maybeItem, f) => switch(maybeItem) {
  | Some(item) => Some(f(item))
  | None => None
}

let make =
    (
      ~emptyDisplayName="",
      ~emptyDisabled=true,
      ~label: string,
      ~item: option('a),
      ~items: list('a),
      ~toSelectOption: 'a => SelectOption.t,
      ~onChange,
      _childern,
    ) => {
  ...component,
  render: _self => {
    let selection = toMaybeSelectOption(item, toSelectOption);
    let value =
      switch (selection) {
      | Some(selection) => `String(selection.value)
      | None => `String("")
      };
    let selections = List.map(items, toSelectOption)
    let select =
      MaterialUi.(
        if (Util.amIMobile) {
          <Select native=true value onChange> (nativeMenuItems(selections, emptyDisplayName, emptyDisabled)) </Select>;
        } else {
          <Select native=false value onChange> (menuItems(selections, emptyDisplayName, emptyDisabled)) </Select>;
        }
      );
    <form autoComplete="off">
      MaterialUi.(<FormControl fullWidth=true> <InputLabel> (s(label)) </InputLabel> select </FormControl>)
    </form>;
  },
};
