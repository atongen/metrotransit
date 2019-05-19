open Belt;

let component = ReasonReact.statelessComponent("Selection");

let s = ReasonReact.string;

let makeEmptyDisplayName = emptyDisplayName => if (Util.amIMobile) {""} else {emptyDisplayName};

let menuItems = (selections, emptyDisplayName, emptyDisabled) => {
  let emptyOption =
    <MaterialUi.MenuItem key=emptyDisplayName value=(`String(emptyDisplayName)) disabled=emptyDisabled />;
  let selectOptions =
    List.map(selections, (selection: SelectOption.t) =>
      <MaterialUi.MenuItem key=selection.value value=(`String(selection.value))>
        (s(selection.displayName))
      </MaterialUi.MenuItem>
    );
  List.add(selectOptions, emptyOption);
};

let nativeMenuItems = (selections, emptyDisplayName, emptyDisabled) => {
  let emptyOption = <option key=emptyDisplayName value=emptyDisplayName disabled=emptyDisabled />;
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
    Js.log(selection);
    let value =
      switch (selection) {
      | Some(selection) => `String(selection.value)
      | None => `String("")
      };
    let selections = List.map(items, toSelectOption)
    Js.log(selections);
    let select =
      MaterialUi.(
        if (Util.amIMobile) {
          <Select native=true value onChange> (nativeMenuItems(selections, makeEmptyDisplayName(emptyDisplayName), emptyDisabled)) </Select>;
        } else {
          <Select native=false value onChange> (menuItems(selections, makeEmptyDisplayName(emptyDisplayName), emptyDisabled)) </Select>;
        }
      );
    <form autoComplete="off">
      MaterialUi.(<FormControl fullWidth=true> <InputLabel> (s(label)) </InputLabel> select </FormControl>)
    </form>;
  },
};
