open Belt;

let component = ReasonReact.statelessComponent("ProviderSelect");

let s = ReasonReact.string;

let isMobile = Util.isMobile();

let (allValue, allName) =
  if (isMobile) {
    ("", "");
  } else {
    ("all", "All");
  };

let allProvider: Provider.t = {id: allValue, name: allName};

let menuItems = providers =>
  List.map(List.add(providers, allProvider), (provider: Provider.t) =>
    <MaterialUi.MenuItem key=provider.id value=(`String(provider.id))> (s(provider.name)) </MaterialUi.MenuItem>
  );

let nativeMenuItems = providers =>
  List.map(List.add(providers, allProvider), (provider: Provider.t) =>
    <option key=provider.id value=provider.id> (s(provider.name)) </option>
  );

let make = (~provider: option(Provider.t), ~providers: list(Provider.t), ~setProvider, _childern) => {
  ...component,
  render: self => {
    let providerChange = (evt, _el) => {
      let providerId = ReactEvent.Form.target(evt)##value;
      let provider = List.getBy(providers, provider => provider.id == providerId);
      setProvider(provider);
    };
    let value =
      switch (provider) {
      | Some(provider) => `String(provider.id)
      | None => `String(allValue)
      };
    let select =
      MaterialUi.(
        if (isMobile) {
          <Select native=true value onChange=providerChange> (nativeMenuItems(providers)) </Select>;
        } else {
          <Select native=false value onChange=providerChange> (menuItems(providers)) </Select>;
        }
      );
    <form autoComplete="off">
      MaterialUi.(<FormControl fullWidth=true> <InputLabel> (s("Provider")) </InputLabel> select </FormControl>)
    </form>;
  },
};
