open Belt;

open Belt.Result;

type state =
  | NotAsked
  | Loading
  | Failure(string)
  | Success(list(Provider.t));

type action =
  | LoadProviders
  | LoadedProviders(list(Provider.t))
  | LoadProvidersFailed(string);

let component = ReasonReact.reducerComponent("ProviderSelect");

let s = ReasonReact.string;

let allProvider: Provider.t = {id: "all", name: "All"};

let menuItems = providers =>
  List.map(List.add(providers, allProvider), (provider: Provider.t) =>
    <MaterialUi.MenuItem key=provider.id value=(`String(provider.id))> (s(provider.name)) </MaterialUi.MenuItem>
  );

let nativeMenuItems = providers =>
  List.map(List.add(providers, allProvider), (provider: Provider.t) =>
    <option key=provider.id value=provider.id> (s(provider.name)) </option>
  );

let make = (~selected: option(Provider.t), ~setProvider, _childern) => {
  ...component,
  initialState: () => NotAsked,
  reducer: (action, _state) =>
    switch (action) {
    | LoadProviders =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self =>
            Js.Promise.(
              ApiUri.loadProviders()
              |> then_(result =>
                   switch (result) {
                   | Ok(providers) => resolve(self.send(LoadedProviders(providers)))
                   | Error(err) => resolve(self.send(LoadProvidersFailed(err)))
                   }
                 )
              |> ignore
            )
        ),
      )
    | LoadedProviders(providers) => ReasonReact.Update(Success(providers))
    | LoadProvidersFailed(err) => ReasonReact.Update(Failure(err))
    },
  didMount: self => self.send(LoadProviders),
  render: self =>
    switch (self.state) {
    | NotAsked => ReasonReact.null
    | Loading => Util.typography("Loading providers...")
    | Failure(err) => Util.typography("Something went wrong: " ++ err)
    | Success(providers) =>
      let providerChange = (evt, _el) => {
        let providerId = ReactEvent.Form.target(evt)##value;
        let provider = List.getBy(providers, provider => provider.id == providerId);
        setProvider(provider);
      };
      let value =
        switch (selected) {
        | Some(provider) => `String(provider.id)
        | None => `String("all")
        };
      let style = ReactDOMRe.Style.make(~overflow="hidden", ());
      let select =
        MaterialUi.(
          if (Util.isMobile()) {
            <Select native=true value onChange=providerChange style> (nativeMenuItems(providers)) </Select>;
          } else {
            <Select native=false value onChange=providerChange style> (menuItems(providers)) </Select>;
          }
        );
      <form autoComplete="off">
        MaterialUi.(<FormControl fullWidth=true> <InputLabel> (s("Provider")) </InputLabel> select </FormControl>)
      </form>;
    },
};
